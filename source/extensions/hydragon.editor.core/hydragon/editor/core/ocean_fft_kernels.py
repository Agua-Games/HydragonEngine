# =============================================================================
# ocean_fft_kernels.py
# GPU implementation of the spectral ocean pipeline (NVIDIA Warp).
#
# The maths lives in `ocean_spectrum`, which is the single source of truth for
# the wave spectrum.  This module only performs the per-frame work: time
# evolution, the transforms, and packing the result into the payload the
# renderer consumes.
#
# Measured facts about Warp 1.14 in this build that shaped the design.  Each one
# was established by experiment (`scratch/warp_fft_probe.py`,
# `scratch/tile_dynamic_probe.py`) and each one is a trap if forgotten:
#
#   1. `wp.tile_fft` / `wp.tile_ifft` are UNNORMALISED.  A 2D inverse therefore
#      needs an explicit division by N^2, or the surface comes out N^2 times too
#      tall.
#   2. `wp.tile_fft` requires at least TWO elements per thread, i.e.
#      `block_dim <= fft_size / 2`.  With ept = 1 it refuses to compile.
#   3. `block_dim` is baked into the compiled module and every kernel in a module
#      is validated against it, so mixing block sizes in one module fails with an
#      error that names an unrelated kernel.  BLOCK_DIM is passed explicitly to
#      EVERY launch here for that reason.
#   4. Tile shapes must be COMPILE-TIME constants ("Tile functions require shape
#      to be a compile time constant").  A module-level Python int DOES work as a
#      tile shape, so one module can hold a specialised kernel per resolution -
#      but the tile path is still unusable, for reasons in 5.
#   5. `wp.tile_fft` is a minefield in this build: a 1D tile is rejected
#      ("argument must be a tile with at least 2 dimensions"), `block_dim >= fft
#      size` is rejected at ept=1, a multi-block grid produces WRONG results
#      (measured, not inferred), and `block_dim = 256` failed at load with a
#      missing symbol.  Only a single block covering the whole array behaves.
#      Hence the hand-written radix-2 transform, which takes a runtime length and
#      was verified against numpy to 3e-7 at three resolutions.
#   6. Operations that span the whole block (like tile ops) may not sit inside a
#      conditional; they must be unconditional.
#   7. `wp.launch` does not validate that written arrays appear in `outputs=`.
#      Getting it wrong is silent, so the arrays are filed deliberately.
#   8. A `@wp.kernel` cannot call another `@wp.kernel`; shared device code must be
#      a `@wp.func`.
#   9. `wp.launch` costs about 50 us of PYTHON overhead per call on the
#      development machine.  The transform is therefore launch-bound, not
#      FLOP-bound: reducing precision or arithmetic would change nothing.  The
#      levers that do work are the thread indexing (measured 14x, applied here)
#      and, beyond that, capturing the frame into a CUDA graph
#      (`wp.ScopedCapture`) so the launches replay as one.
#
# No Kit imports: this module is importable and testable headlessly.
# =============================================================================

from __future__ import annotations

import math
from typing import Dict, Optional

try:
    import numpy as np

    HAS_NUMPY = True
except ImportError:  # pragma: no cover
    np = None
    HAS_NUMPY = False

try:
    import warp as wp

    HAS_WARP = True
except ImportError:  # pragma: no cover - only outside Kit and the test harness
    wp = None
    HAS_WARP = False

from . import ocean_spectrum

if HAS_WARP:
    # `set_module_options` applies to the module of its CALLER, so this governs
    # the kernels defined below and nothing else.  Autodiff is off because none
    # of this is differentiated, and the mathdx FFT is left enabled so that the
    # option is on if a tiled path is ever added.
    wp.set_module_options({"enable_backward": False, "enable_mathdx_fft": True})


#: Threads per block for EVERY launch in this module (see trap 3 above).
#: Deliberately not tuned for the row transform, which runs one thread per row
#: and so is latency- rather than occupancy-bound.
BLOCK_DIM = 256

#: Side of the averaged height grid the host-side wave query reads.  This is
#: the SIDE, not the downsample factor: the factor is resolution / HOST_GRID.
#: Conflating the two gives a 2x2 host grid for a 128x128 patch, which is
#: useless for a wave query and still looks like a plausible number.
HOST_GRID = 64

#: Layout of the packed payload.  It is exactly one RGBA float texel per grid
#: cell, which is the payload shape the dynamic-texture path takes.
CHANNEL_HEIGHT = 0
CHANNEL_NORMAL_X = 1
CHANNEL_NORMAL_Z = 2
CHANNEL_FOAM = 3
CHANNELS = 4

TWO_PI = 6.283185307179586


# -----------------------------------------------------------------------------
# Device helpers
# -----------------------------------------------------------------------------

if HAS_WARP:

    @wp.func
    def _complex_mul(a: wp.vec2f, b: wp.vec2f) -> wp.vec2f:
        """(a + bi)(c + di) as a 2-vector.  First component real, second imaginary."""
        return wp.vec2f(a[0] * b[0] - a[1] * b[1], a[0] * b[1] + a[1] * b[0])

    @wp.func
    def _twiddle(index: int, size: int, sign: float) -> wp.vec2f:
        """e^{i * sign * 2*pi * index / size}, recomputed rather than accumulated.

        Accumulating the twiddle by repeated multiplication drifts over the log2(N)
        stages, and the drift shows up as a low-frequency error across the whole
        patch rather than as local noise.
        """
        angle = sign * 6.283185307179586 * float(index) / float(size)
        return wp.vec2f(wp.cos(angle), wp.sin(angle))

    @wp.func
    def _mirror_index(index: int, size: int) -> int:
        """Index of the -k cell.  Plain `-i` does not wrap in Warp (C semantics)."""
        return (size - index) % size

    # -------------------------------------------------------------------------
    # Time evolution
    # -------------------------------------------------------------------------

    @wp.kernel
    def evolve_spectrum(
        h0: wp.array2d(dtype=wp.vec2f),
        ht: wp.array2d(dtype=wp.vec2f),
        kx: wp.array(dtype=wp.float32),
        kz: wp.array(dtype=wp.float32),
        time: float,
        gravity: float,
    ):
        """Two-term deep-water evolution, which is Hermitian for any h0.

            h(k,t) = h0(k) e^{i w t} + conj(h0(-k)) e^{-i w t},   w = sqrt(g|k|)

        Both terms are required.  Keeping only the first leaves a field whose
        inverse transform is complex, so taking the real part discards about half
        the energy and produces a surface that is not the one the spectrum asked
        for.
        """
        i, j = wp.tid()
        n = kx.shape[0]

        k = wp.sqrt(kx[i] * kx[i] + kz[j] * kz[j])
        omega = wp.sqrt(gravity * k) * time
        c = wp.cos(omega)
        s = wp.sin(omega)

        a = h0[i, j]
        q = h0[_mirror_index(i, n), _mirror_index(j, n)]

        # (a + conj(q) rotated) expanded so the trig is evaluated once.
        re = (a[0] + q[0]) * c - (a[1] + q[1]) * s
        im = (a[1] - q[1]) * c + (a[0] - q[0]) * s
        ht[i, j] = wp.vec2f(re, im)

    @wp.kernel
    def apply_spectral_factor(
        source: wp.array2d(dtype=wp.vec2f),
        target: wp.array2d(dtype=wp.vec2f),
        kx: wp.array(dtype=wp.float32),
        kz: wp.array(dtype=wp.float32),
        mode: int,
        choppiness: float,
    ):
        """Multiply by the factor that turns a height spectrum into a displacement.

            mode 0: identity, the height field itself
            mode 1: -i kx / |k|, the horizontal displacement along X
            mode 2: -i kz / |k|, the horizontal displacement along Z

        Displacing the surface horizontally is what sharpens wave crests.  Without
        it the surface still moves, but the crests stay symmetric and the
        Jacobian used for foam never compresses, so foam never appears.

        `choppiness` scales the two displacement modes and NOT the identity mode -
        it must not touch the height field.  The whole thing is captured into a
        CUDA graph, so the value is frozen per capture, which is correct because a
        choppiness change rebuilds the solver and captures a new graph.
        """
        i, j = wp.tid()

        if mode == 0:
            # Identity.  This branch must COPY: falling through to the multiply
            # below would scale the height spectrum by a weight of zero and
            # silently flatten the whole ocean.
            target[i, j] = source[i, j]
            return

        k = wp.sqrt(kx[i] * kx[i] + kz[j] * kz[j])
        weight = float(0.0)
        if mode == 1:
            weight = kx[i]
        else:
            weight = kz[j]
        if k > 1.0e-4:
            weight = weight / k
        weight = weight * choppiness

        a = source[i, j]
        # (a + bi) * (0 - w i) = (b w, -a w)
        target[i, j] = wp.vec2f(a[1] * weight, -a[0] * weight)

    # -------------------------------------------------------------------------
    # Transforms
    # -------------------------------------------------------------------------

    @wp.func
    def _reverse_bits(value: int, bits: int) -> int:
        result = int(0)
        remaining = value
        step = int(0)
        while step < bits:
            result = (result << 1) | (remaining & 1)
            remaining = remaining >> 1
            step += 1
        return result

    @wp.kernel
    def permute_axis1(a: wp.array2d(dtype=wp.vec2f), n: int, bits: int):
        """Bit-reversal permutation along index 1.  Swaps each pair once (r > j)."""
        i, j = wp.tid()
        r = _reverse_bits(j, bits)
        if r > j:
            tmp = a[i, j]
            a[i, j] = a[i, r]
            a[i, r] = tmp

    @wp.kernel
    def permute_axis0(a: wp.array2d(dtype=wp.vec2f), n: int, bits: int):
        """Bit-reversal permutation along index 0."""
        i, j = wp.tid()
        r = _reverse_bits(i, bits)
        if r > i:
            tmp = a[i, j]
            a[i, j] = a[r, j]
            a[r, j] = tmp

    @wp.kernel
    def butterfly_axis1(a: wp.array2d(dtype=wp.vec2f), n: int, size: int, sign: float):
        """One butterfly per thread for one stage, along index 1.

        Thread (i, b) takes butterfly b of row i.  Spreading one butterfly per
        thread instead of one row per thread is what makes the transform fast:
        measured on the development GPU at N=256, one row per thread costs 6.0 ms
        per pass, a grid of single-thread blocks costs 1.3 ms, and this costs
        0.43 ms - a 14x difference from indexing alone, with identical maths.
        """
        i, b = wp.tid()
        half = size >> 1
        group = b // half
        j = b - group * half
        i0 = group * size + j
        i1 = i0 + half
        w = _twiddle(j, size, sign)
        lo = a[i, i0]
        hi = _complex_mul(w, a[i, i1])
        a[i, i0] = wp.vec2f(lo[0] + hi[0], lo[1] + hi[1])
        a[i, i1] = wp.vec2f(lo[0] - hi[0], lo[1] - hi[1])

    @wp.kernel
    def butterfly_axis0(a: wp.array2d(dtype=wp.vec2f), n: int, size: int, sign: float):
        """One butterfly per thread for one stage, along index 0.

        Doing the second axis in place, rather than transposing twice, removes
        two launches per transform and keeps the accesses coalesced because the
        fastest-varying thread coordinate is the column.
        """
        b, j = wp.tid()
        half = size >> 1
        group = b // half
        m = b - group * half
        i0 = group * size + m
        i1 = i0 + half
        w = _twiddle(m, size, sign)
        lo = a[i0, j]
        hi = _complex_mul(w, a[i1, j])
        a[i0, j] = wp.vec2f(lo[0] + hi[0], lo[1] + hi[1])
        a[i1, j] = wp.vec2f(lo[0] - hi[0], lo[1] - hi[1])

    # -------------------------------------------------------------------------
    # Output
    # -------------------------------------------------------------------------

    @wp.kernel
    def extract_component(
        source: wp.array2d(dtype=wp.vec2f),
        target: wp.array2d(dtype=wp.float32),
        scale: float,
        component: int,
    ):
        """Take one part of a transform result and apply the normalisation.

        `scale` is 1/N^2: two unnormalised inverse passes multiply the result by
        N^2 in total, so the surface would otherwise be N^2 times too tall.

        `component` is 0 for the real part - a height, or an x displacement - and
        1 for the imaginary part, which is how `pack_displacements` carries the z
        displacement in the same transform.
        """
        i, j = wp.tid()
        if component == 0:
            target[i, j] = source[i, j][0] * scale
        else:
            target[i, j] = source[i, j][1] * scale

    @wp.kernel
    def pack_surface(
        height: wp.array2d(dtype=wp.float32),
        disp_x: wp.array2d(dtype=wp.float32),
        disp_z: wp.array2d(dtype=wp.float32),
        packed: wp.array2d(dtype=wp.vec4),
        cell: float,
        foam_threshold: float,
        foam_bias: float,
    ):
        """Build one RGBA texel per grid cell: (height, normal.x, normal.z, foam).

        normal.y is omitted because it is non-negative by construction and can be
        recovered in the shader as sqrt(1 - x^2 - z^2); that keeps the payload a
        single vec4, which is the shape the dynamic-texture path takes.

        Index 0 runs along world X, index 1 along world Z.  The finite
        differences wrap, because the field is periodic by construction and a
        non-wrapping difference would draw a seam of false normals down two edges
        of every patch.
        """
        i, j = wp.tid()
        n = height.shape[0]
        ip = (i + 1) % n
        im = (i + n - 1) % n
        jp = (j + 1) % n
        jm = (j + n - 1) % n
        inv = 1.0 / (2.0 * cell)

        # dP/dX = (1 + dDx/dX, dh/dX, dDz/dX)
        txx = 1.0 + (disp_x[ip, j] - disp_x[im, j]) * inv
        txy = (height[ip, j] - height[im, j]) * inv
        txz = (disp_z[ip, j] - disp_z[im, j]) * inv

        # dP/dZ = (dDx/dZ, dh/dZ, 1 + dDz/dZ)
        tzx = (disp_x[i, jp] - disp_x[i, jm]) * inv
        tzy = (height[i, jp] - height[i, jm]) * inv
        tzz = 1.0 + (disp_z[i, jp] - disp_z[i, jm]) * inv

        # n = cross(dP/dZ, dP/dX), which yields +Y for an undisplaced surface.
        nx = tzy * txz - tzz * txy
        ny = tzz * txx - tzx * txz
        nz = tzx * txy - tzy * txx

        # ny before normalisation IS the Jacobian of the horizontal displacement:
        # it is the measure of how much the surface has been compressed, and
        # therefore where foam belongs.
        jacobian = ny
        length = wp.sqrt(nx * nx + ny * ny + nz * nz)
        if length > 1.0e-12:
            nx = nx / length
            ny = ny / length
            nz = nz / length
        # A negative Y means the surface folded over itself; face it at the sky.
        if ny < 0.0:
            nx = -nx
            nz = -nz

        foam = (foam_threshold - jacobian) / foam_bias
        foam = wp.clamp(foam, 0.0, 1.0)

        packed[i, j] = wp.vec4(height[i, j], nx, nz, foam)

    @wp.kernel
    def subsample_height(
        height: wp.array2d(dtype=wp.float32),
        small: wp.array2d(dtype=wp.float32),
        factor: int,
    ):
        """Box-average the height field down for the host-side wave query.

        Copying the full field back every frame would be wasteful, and a single
        sample would be the wrong answer anyway: buoyancy wants the mean height
        over the submerged footprint, which is what an average gives.
        """
        i, j = wp.tid()
        total = float(0.0)
        di = int(0)
        while di < factor:
            dj = int(0)
            while dj < factor:
                total += height[i * factor + di, j * factor + dj]
                dj += 1
            di += 1
        small[i, j] = total / float(factor * factor)


# -----------------------------------------------------------------------------
# Device selection
# -----------------------------------------------------------------------------


def preferred_device() -> str:
    """The device the pipeline should run on.

    This mirrors `effects_controller`, the only other Warp user in the engine: use
    a CUDA device when there is one, otherwise fall back to the CPU.  Hard-coding
    "cuda" would raise on a machine without a GPU rather than degrade.
    """
    if not HAS_WARP:
        raise RuntimeError("warp is not available")
    if wp.get_cuda_devices():
        return "cuda:0"
    return "cpu"


# -----------------------------------------------------------------------------
# Solver
# -----------------------------------------------------------------------------


class OceanSolver:
    """Device-side state and per-frame pipeline for one ocean patch.

    Owns every buffer, so a patch can be built, stepped and released without
    touching anything shared.  Nothing here reads or writes a USD stage: the
    caller owns the stage, this owns only the numbers.
    """

    def __init__(
        self,
        parameters: ocean_spectrum.OceanParameters,
        foam_threshold: float = 0.85,
        foam_bias: float = 0.35,
        device: Optional[str] = None,
        use_graph: bool = True,
    ):
        if not HAS_WARP:
            raise RuntimeError("OceanSolver requires warp")
        if not HAS_NUMPY:
            raise RuntimeError("OceanSolver requires numpy")
        if foam_bias <= 0.0:
            raise ValueError(f"foam_bias must be positive, got {foam_bias}")

        self.parameters = parameters
        self.foam_threshold = float(foam_threshold)
        self.foam_bias = float(foam_bias)
        self.device = device or preferred_device()

        resolution = parameters.resolution
        if resolution & (resolution - 1):
            raise ValueError(f"resolution must be a power of two, got {resolution}")
        if resolution < ocean_spectrum.MIN_RESOLUTION:
            raise ValueError(
                f"resolution must be at least {ocean_spectrum.MIN_RESOLUTION}"
            )

        self.resolution = resolution
        self._closed = False
        self._use_graph = bool(use_graph)

        kx, kz = ocean_spectrum.wavenumbers(resolution, parameters.patch_size)
        spectrum = ocean_spectrum.initial_spectrum(parameters)

        self.kx = wp.array(kx.astype(np.float32), dtype=wp.float32, device=self.device)
        self.kz = wp.array(kz.astype(np.float32), dtype=wp.float32, device=self.device)
        self.h0 = wp.array2d(
            np.ascontiguousarray(spectrum.astype(np.complex64)).view(np.float32)
            .reshape(resolution, resolution, 2),
            dtype=wp.vec2f,
            device=self.device,
        )

        self.ht = wp.zeros((resolution, resolution), dtype=wp.vec2f, device=self.device)
        self.work = wp.zeros((resolution, resolution), dtype=wp.vec2f, device=self.device)
        self._bits = int(math.log2(resolution))

        self.height = wp.zeros(
            (resolution, resolution), dtype=wp.float32, device=self.device
        )
        self.disp_x = wp.zeros(
            (resolution, resolution), dtype=wp.float32, device=self.device
        )
        self.disp_z = wp.zeros(
            (resolution, resolution), dtype=wp.float32, device=self.device
        )
        self.packed = wp.zeros(
            (resolution, resolution), dtype=wp.vec4, device=self.device
        )

        host_side = min(HOST_GRID, resolution)
        if resolution % host_side != 0:
            raise ValueError(
                f"resolution {resolution} must be a multiple of {host_side} so the "
                "host-side grid is an exact average"
            )
        self.host_factor = resolution // host_side
        self.small_height = wp.zeros(
            (host_side, host_side), dtype=wp.float32, device=self.device
        )
        self.host_grid = host_side

        # Captured lazily on the first step, once every kernel has been compiled.
        self._graph = None
        self._graph_ready = False
        # Why capture failed, if it did.  Empty means "no attempt yet".
        self._graph_error = ""

        self.last_time = None

    @property
    def graph_captured(self) -> bool:
        """Whether the frame is being replayed from a CUDA graph.

        False is a supported state - it means the body is launched kernel by
        kernel, which is correct and slower.  Exposed so a test can assert that
        capture succeeded rather than silently measuring the slow path.
        """
        return self._graph is not None

    @property
    def graph_error(self) -> str:
        """The reason the last capture attempt failed, or "" if none did.

        A failed capture leaves the solver CORRECT but roughly an order of
        magnitude slower, so it must not be swallowed: a caller that measures a
        frame time needs to know which path it measured.
        """
        return self._graph_error

    # -- pipeline -------------------------------------------------------------

    def _transform_axis1(self, field, n: int, sign: float) -> None:
        wp.launch(
            permute_axis1,
            dim=(n, n),
            inputs=[field, n, self._bits],
            block_dim=BLOCK_DIM,
        )
        size = 2
        while size <= n:
            wp.launch(
                butterfly_axis1,
                dim=(n, n // 2),
                inputs=[field, n, size, sign],
                block_dim=BLOCK_DIM,
            )
            size <<= 1

    def _transform_axis0(self, field, n: int, sign: float) -> None:
        wp.launch(
            permute_axis0,
            dim=(n, n),
            inputs=[field, n, self._bits],
            block_dim=BLOCK_DIM,
        )
        size = 2
        while size <= n:
            wp.launch(
                butterfly_axis0,
                dim=(n // 2, n),
                inputs=[field, n, size, sign],
                block_dim=BLOCK_DIM,
            )
            size <<= 1

    def _transform_2d(self, field, sign: float) -> None:
        """In-place 2D transform of both axes, UNNORMALISED.

        Both axes are transformed in place.  The column pass used to be done by
        transposing twice, which cost two extra launches per transform; doing the
        butterflies along axis 0 directly is equally coalesced because the
        fastest-varying thread coordinate is the column.
        """
        n = self.resolution
        self._transform_axis1(field, n, sign)
        self._transform_axis0(field, n, sign)

    def _inverse_transform_2d(self, field) -> None:
        """In-place 2D inverse transform, leaving the result UNNORMALISED."""
        self._transform_2d(field, 1.0)

    def forward_transform_2d(self, field) -> None:
        """In-place 2D FORWARD transform.  Exposed so tests can verify against a
        reference implementation rather than trusting the pipeline end to end."""
        self._transform_2d(field, -1.0)

    def run(self, time: float) -> None:
        """Advance the field to `time` and rebuild the packed payload.

        The time evolution carries the per-frame scalar and is launched normally;
        everything after it is a pure function of the buffers and is replayed from
        a captured CUDA graph, so the frame costs two launches instead of sixty.
        """
        if self._closed:
            raise RuntimeError("this solver has been released")

        if not self._graph_ready:
            self._graph_ready = True
            self._build_graph()

        resolution = self.resolution
        wp.launch(
            evolve_spectrum,
            dim=(resolution, resolution),
            inputs=[
                self.h0,
                self.ht,
                self.kx,
                self.kz,
                float(time),
                self.parameters.gravity,
            ],
            block_dim=BLOCK_DIM,
        )

        if self._graph is not None:
            wp.capture_launch(self._graph)
        else:
            self._run_body()

        self.last_time = float(time)

    def _run_body(self) -> None:
        """Everything downstream of the time evolution, as one replayable unit.

        Three inverse transforms: the height, and the two horizontal displacements
        that sharpen the crests.

        Two is possible - pack both displacements into one complex transform, x in
        the real part and z in the imaginary part, since the transform is linear
        and each summand is individually real.  It was implemented, measured and
        REJECTED: the imaginary channel comes back with about five times the
        relative error of the real one.  Measured at N=256, disp_z 1.9% against
        disp_x 0.39%, because the two fields share one float32 traversal and the
        smaller one loses more of its digits.  With the CUDA graph already
        removing 90% of the frame, buying roughly 0.08 ms for four orders of
        magnitude of accuracy on a physical field is the wrong trade.  Do not
        re-propose it without a measurement that says otherwise.

        (Packing the height with a displacement is not even possible: the
        identity factor and -i kx/|k| combine through the factor of i into the
        purely real scalar (1 + kx/|k|), which collapses to one field.)
        """
        resolution = self.resolution
        scale = 1.0 / float(resolution * resolution)

        for component in (0, 1, 2):
            wp.launch(
                apply_spectral_factor,
                dim=(resolution, resolution),
                inputs=[
                    self.ht,
                    self.work,
                    self.kx,
                    self.kz,
                    component,
                    self.parameters.choppiness,
                ],
                block_dim=BLOCK_DIM,
            )
            self._inverse_transform_2d(self.work)
            target = (self.height, self.disp_x, self.disp_z)[component]
            wp.launch(
                extract_component,
                dim=(resolution, resolution),
                inputs=[self.work, target, scale, 0],
                block_dim=BLOCK_DIM,
            )

        wp.launch(
            pack_surface,
            dim=(resolution, resolution),
            inputs=[
                self.height,
                self.disp_x,
                self.disp_z,
                self.packed,
                self.parameters.cell_size,
                self.foam_threshold,
                self.foam_bias,
            ],
            block_dim=BLOCK_DIM,
        )
        wp.launch(
            subsample_height,
            dim=(self.host_grid, self.host_grid),
            inputs=[self.height, self.small_height, self.host_factor],
            block_dim=BLOCK_DIM,
        )

    def _build_graph(self) -> None:
        """Compile everything, then capture the body into a CUDA graph.

        The frame is dominated by Python-side launch cost - about 50 us per
        `wp.launch`, measured - not by arithmetic, so collapsing ~60 launches into
        one replay is worth far more than any change to the maths.  This is also
        why reducing precision would achieve nothing.

        Capture freezes the kernel ARGUMENTS, which is exactly why the time scalar
        is not in here: it changes every frame and would be baked in at whatever
        value happened to be current when the graph was recorded.
        """
        if not self._use_graph:
            return
        if not self.device.startswith("cuda"):
            # Graph capture is a CUDA feature.  On the CPU fallback the body is
            # still correct, just launched one kernel at a time.
            return

        try:
            # Warm up outside the capture so no compilation happens inside it.
            self._run_body()
            self.synchronize()

            with wp.ScopedCapture() as capture:
                self._run_body()
            self._graph = capture.graph
        except Exception as exc:  # noqa: BLE001 - a missing graph is a slowdown, not a failure
            self._graph = None
            self._graph_error = f"{type(exc).__name__}: {exc}"

    # -- output ---------------------------------------------------------------

    def synchronize(self) -> None:
        """Block until the outstanding launches have finished."""
        wp.synchronize_device(self.device)

    def packed_numpy(self):
        """The packed payload as an (N, N, 4) float32 array, for tests and export."""
        self.synchronize()
        return self.packed.numpy().reshape(self.resolution, self.resolution, CHANNELS)

    def host_height_numpy(self):
        """The averaged height grid the gameplay systems query against."""
        self.synchronize()
        return self.small_height.numpy()

    def height_numpy(self):
        self.synchronize()
        return self.height.numpy()

    def displacements_numpy(self):
        """The two horizontal displacement fields, for accuracy measurements."""
        self.synchronize()
        return self.disp_x.numpy(), self.disp_z.numpy()

    # -- lifecycle ------------------------------------------------------------

    def release(self) -> None:
        """Drop every device allocation.

        Warp frees on garbage collection of the array, so this only needs to drop
        the references.  It exists so that a caller removing a patch does not
        depend on a later collection cycle to reclaim the device memory.
        """
        self._closed = True
        self._graph = None
        for name in (
            "kx", "kz", "h0", "ht", "work",
            "height", "disp_x", "disp_z", "packed", "small_height",
        ):
            setattr(self, name, None)

    def describe(self) -> Dict[str, object]:
        """A small summary for logs and the property panel."""
        return {
            "resolution": self.resolution,
            "patch_size": self.parameters.patch_size,
            "cell_size": self.parameters.cell_size,
            "wind_speed": self.parameters.wind_speed,
            "significant_height": self.parameters.target_significant_height,
            "device": self.device,
            "frames": math.floor(self.last_time or 0.0) if self.last_time else 0,
        }
