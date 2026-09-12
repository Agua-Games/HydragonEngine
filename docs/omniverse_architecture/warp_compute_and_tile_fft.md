# Warp compute in Kit: launch cost, tile FFT, and how to test it headlessly

Platform notes for running NVIDIA Warp compute inside Omniverse Kit. Established while building the
spectral ocean (`hydragon/editor/core/ocean_spectrum.py`, `ocean_fft_kernels.py`).

Established on **Warp 1.14.0** (`omni.warp.core-1.14.0+wx64`, `omni.warp-1.14.0`), Kit 110,
Python 3.12, **RTX 3060 Ti (8 GiB, sm_86)**, CUDA 12.9 / driver 13.3. Warp changes fast — re-verify.

---

## 1. The measurements in one table

A 256x256 complex 2D transform, all variants numerically identical:

| approach | time | launches |
|---|---|---|
| one thread per row, `dim=(n,)`, `block_dim=256` | 6.00 ms | 1 |
| one thread per row, `dim=(n,)`, `block_dim=1` | 1.31 ms | 1 |
| one butterfly per thread, one launch per stage | 0.43 ms | 9 |
| **`wp.tile_fft`, literal tile shape, multi-block** | **0.094 ms** | **1** |

**[verified]** Two conclusions, and the second is the surprising one:

1. **Thread indexing was worth 14x**, from identical arithmetic. One thread per row gives a grid of
   only *n* threads; one butterfly per thread gives *n^2/2*.
2. **The pipeline is launch-bound, not FLOP-bound.** A `wp.launch` costs about **50 us of Python
   overhead** each on this machine. The "per-stage" variant above costs 0.43 ms, which is 9 launches
   at roughly 48 us — the arithmetic is essentially free. Reducing precision, packing data more
   densely, or using fewer bytes per sample therefore **cannot** help; only reducing the number of
   launches, or the cost of each, does.

---

## 2. `wp.tile_fft` rules, all measured

The tile API is the fast path (cuFFTDx via libmathdx — `wp.config.enable_mathdx_fft` defaults to
**True**) but it is narrow. Every constraint below was hit, not read.

| rule | failure if violated |
|---|---|
| tile shapes must be **compile-time constants** | `Tile functions require shape to be a compile time constant.` |
| a tile **must have at least 2 dimensions** | `tile_fft() argument must be a tile with at least 2 dimensions, got 1D` |
| at least **2 elements per thread**: `block_dim <= fft_size / 2` | `tile_fft() requires at least 2 elements per thread, but got ept=1 (fft_size=128, block_dim=128). Reduce block_dim to at most 64 for this FFT size.` |
| **the tile shape must be a literal, not a `wp.constant`** | **[verified] silently WRONG RESULTS**, no error |
| a single block covering several rows | **[verified] silently WRONG RESULTS** |
| `block_dim = 16` with a 128-wide tile (ept=8) | `KeyError: '..._cuda_kernel_forward_smem_bytes'` at load |
| tile ops may not sit inside a conditional | generated CUDA fails to compile (`expected a "}"`) |

**The literal-versus-constant finding is the one that costs the most time to rediscover.** The form

```python
TILE = wp.constant(128)      # <-- poisons the module: every tile kernel in it goes wrong
@wp.kernel
def fft_rows(a):
    i, j = wp.tid()
    t = wp.tile_load(a, shape=(1, TILE), offset=(i, 0))
    wp.tile_fft(t)
    wp.tile_store(a, t, offset=(i, 0))
```

compiled, ran, and produced results off by 100% relative error, while the identical kernel with a
**literal** `shape=(1, 128)` matched `numpy.fft.fft2` to 1.4e-07. Both were in a module that also
contained a `wp.constant`-shaped tile kernel. Whether the constant itself or the *mixture* is to
blame is **[inferred]**; what is verified is that literals work and a `wp.constant` shape does not.

Working form, multi-block, one row per block:

```python
@wp.kernel
def fft_rows_128(a: wp.array2d(dtype=wp.vec2f)):
    i, j = wp.tid()
    t = wp.tile_load(a, shape=(1, 128), offset=(i, 0))
    wp.tile_fft(t)
    wp.tile_store(a, t, offset=(i, 0))

wp.launch_tiled(fft_rows_128, dim=[128], inputs=[a], block_dim=64)
```

and the batched form, four rows per block (`tile_fft` treats leading dimensions as independent
batches):

```python
    t = wp.tile_load(a, shape=(4, 128), offset=(i * 4, 0))
wp.launch_tiled(fft_batch4, dim=[32], inputs=[a], block_dim=64)   # 32 blocks x 4 rows = 128 rows
```

**Consequence for an engine:** because the shape must be a literal, and because of the
one-FFT-size-per-module constraint below, the tile path needs **one Python module per supported
resolution**. That is workable but it makes adding a resolution a code change plus a new file, and it
is why this repository does NOT use the tile path.

### THE BLOCKER: one FFT size per module **[verified]**

A module containing tile-FFT kernels for **more than one FFT size** produces **silently wrong
results from every one of them**. Measured: `ifft_rows_128` and `ifft_rows_256` in one module both
came out at ~100% relative error against `numpy.fft.ifft2`, with no error and no warning. A module
with only `ifft_rows_512` was also wrong, so the failure is not about 128 specifically.

The working probes all had exactly ONE fft size per module (a `128` shape and a `(4, 128)` shape both
fit that rule, and both were correct). The failing probes all had several sizes, or mixed a
`wp.constant` shape with literals. **This is [inferred] as the mechanism** — what is verified is the
outcome, twice, in two separate probes.

Add to that: `block_dim` is module-wide and `block_dim == fft_size` is rejected (ept=1), so a
module's block_dim must be at most half of its smallest fft size. Only **one** size per module
survives both rules, which makes the tile path a per-resolution Python file.

### What it costs when it works

The steady-state cost is excellent, but the **first call in a session compiles and loads the module**,
and that is measured in **seconds, not milliseconds**: 5.5 s for a module with two tile-FFT kernels,
7.6 s for one with a 512-wide kernel. Warp caches the result on disk
(`%LOCALAPPDATA%\NVIDIA\warp\Cache\<version>`), so only the first ever run pays it — but the first
time an artist creates an ocean, it is a multi-second stall.

### Verdict for an animated field

**Do not use `wp.tile_fft` for the ocean.** The measured envelope is too narrow (one FFT size per
Python module), the first-use cost is seconds, and the failure mode is wrong numbers rather than an
exception. A hand-written radix-2 transform takes a runtime size, serves every resolution from one
module, was verified to 3e-7 against numpy at three resolutions, and measured 2.87 ms per frame for
the whole pipeline at 256x256 on the development GPU. When more speed is needed, the next lever is
**CUDA graph capture** (`wp.ScopedCapture`), which cuts the launch count without touching the
transform.

### `wp.launch_tiled` thread mapping **[verified]**

`wp.launch_tiled(kernel, dim=[n], block_dim=b)` creates a grid of *n* blocks of *b* threads, and
`wp.tid()` returns `(block_index, thread_index)`. Measured for `dim=[128], block_dim=64`: the first
`tid` ranged over `[0, 127]`, the second over `[0, 63]`.

Warning that costs an hour: **`block_dim` is baked into the compiled module, and every kernel in a
module is validated against it.** Launching a 16-thread tile kernel in a module that also holds a
`tile_fft` kernel fails with an error that *names the FFT kernel*, not the one you launched. Pass one
`block_dim` to every launch in a module.

---

## 3. Other Warp facts worth knowing before writing kernels

- **A `@wp.kernel` cannot call another `@wp.kernel`.** The error is
  `Could not find function ... user functions must be annotated with a @wp.func decorator to be called from a kernel.`
  Shared device code must be `@wp.func`. This is the single most common structural mistake when
  porting a CPU pipeline.
- **`wp.zeros` inside a kernel is not a device allocation.** It is `fixedarray_t`, a stack array whose
  shape must be a **constant**, and it cannot escape the function. A per-thread array of runtime
  length is impossible; put the data in global memory instead.
- **`wp.launch` does not validate `inputs` vs `outputs`.** They are concatenated to match the
  signature, so a written array passed in `inputs=` is silently accepted. File them deliberately or
  you lose dependency tracking.
- **`wp.randf` takes a `uint32`** and is not differentiable. To get a reproducible field, draw the
  randoms on the host with numpy and upload them: a device generator's stream cannot be matched
  against a CPU reference, which makes the whole pipeline untestable.
- `wp.init()` is not strictly required (most public entry points call it), but calling it explicitly
  is the convention this engine already used in `effects_controller`.
- `wp.launch` has a `record_cmd: bool = True` default, which is what makes CUDA capture work.
- **Graph capture exists**: `wp.ScopedCapture()` / `wp.capture_begin()` / `wp.capture_end()` /
  `wp.capture_launch(graph)`. There is no `wp.capture_available()`. **[inferred]** Because capture
  freezes kernel arguments, a per-frame time value has to travel through a device buffer rather than
  as a scalar argument.

### Triton is not a shortcut

`wp.tile_fft` with `enable_mathdx_gemm`/`enable_mathdx_fft` is the sanctioned fast path. There is no
`wp.fft` module and no user-facing batched FFT in `warp/__init__.py` **[verified]** — if the tile
path is unusable, the transform has to be hand-written.

---

## 4. Testing Warp headlessly, outside Kit

`_build/target-deps/python/python.exe` has **neither numpy nor warp**. Both ship inside the Kit
extension cache and can be bootstrapped, which makes the entire compute pipeline testable with a
plain interpreter — no editor, no stage, no PLAY. This is how every number in this document was
obtained.

```python
import os, sys
EXTS = os.path.join(REPO_ROOT, "_build", "windows-x86_64", "release", "extscache")

# numpy ships inside the Kit pip prebundle
for name in os.listdir(EXTS):
    if name.startswith("omni.kit.pip_archive-"):
        sys.path.insert(0, os.path.join(EXTS, name, "pip_prebundle"))   # numpy 2.3.1

# the Warp library ships as its own extension, with native DLLs that need their siblings
WARP = next(os.path.join(EXTS, n) for n in os.listdir(EXTS) if n.startswith("omni.warp.core-"))
sys.path.insert(0, WARP)
os.add_dll_directory(WARP)

import warp as wp
wp.init()          # prints the device list, including the CUDA device and its compute capability
```

`os.add_dll_directory` is mandatory: `warp.dll` and `warp-clang.dll` sit next to the package and will
not resolve from `sys.path` alone.

**A GPU is available in this configuration**, so kernel correctness and performance can both be
measured from a plain terminal. The full ocean pipeline is verified this way to ~1e-6 relative
against a numpy reference, at three resolutions and three times.

### What to check first in a new Warp module

1. Does the kernel compile at all? `wp.launch` raises with the generated CUDA source on failure.
2. Is the tile shape a literal? (Section 2.)
3. Compare against numpy before trusting anything. For an unnormalised transform pair,
   `ifft(fft(x)) == N * x` in 1D and `N^2 * x` in 2D — **`wp.tile_fft` and `wp.tile_ifft` are both
   unnormalised** **[verified]**, so a 2D inverse needs an explicit `1/N^2` or the result is `N^2`
   times too large.
