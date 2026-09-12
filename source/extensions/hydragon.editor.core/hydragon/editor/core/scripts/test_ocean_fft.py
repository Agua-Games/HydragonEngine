# =============================================================================
# test_ocean_fft.py
# Headless checks for the spectral ocean pipeline.
#
# Sections 1-6 exercise `ocean_spectrum` alone and need only numpy.
# Sections 7+ compare the GPU pipeline in `ocean_fft_kernels` against that
# reference, and additionally need Warp and a CUDA device.
#
# Run:
#   _build\target-deps\python\python.exe ^
#     source\extensions\hydragon.editor.core\hydragon\editor\core\scripts\test_ocean_fft.py
#
# The interpreter shipped in _build\target-deps has no numpy and no warp, so the
# bootstrap below adds the Kit prebundle and the Warp core extension to
# sys.path.  That is the same trick `test_volume_bounds.py` uses for pxr.
# =============================================================================

import os
import sys

_HERE = os.path.dirname(os.path.abspath(__file__))


def _find_repo_root(start: str) -> str:
    """Walk upwards until the directory that contains `_build` is found."""
    current = start
    while True:
        if os.path.isdir(os.path.join(current, "_build")):
            return current
        parent = os.path.dirname(current)
        if parent == current:
            raise RuntimeError(f"could not locate the repository root above {start}")
        current = parent


_ROOT = _find_repo_root(_HERE)
_EXTS_CACHE = os.path.join(_ROOT, "_build", "windows-x86_64", "release", "extscache")


def _find_extension(prefix: str) -> str:
    for name in os.listdir(_EXTS_CACHE):
        if name.startswith(prefix):
            return os.path.join(_EXTS_CACHE, name)
    raise RuntimeError(f"extension {prefix}* not found in {_EXTS_CACHE}")


# numpy ships inside the Kit pip prebundle; warp ships inside omni.warp.core.
sys.path.insert(0, os.path.join(_find_extension("omni.kit.pip_archive-"), "pip_prebundle"))
_WARP_CORE = _find_extension("omni.warp.core-")
sys.path.insert(0, _WARP_CORE)
os.add_dll_directory(_WARP_CORE)

_EXT_DIR = os.path.join(_ROOT, "source", "extensions", "hydragon.editor.core")
sys.path.insert(0, _EXT_DIR)

import numpy as np  # noqa: E402

from hydragon.editor.core import ocean_spectrum as ocean  # noqa: E402

# The GPU half is optional: the padics and the spectrum maths run anywhere, the
# kernels need Warp and a device.
try:
    from hydragon.editor.core import ocean_fft_kernels as kernels  # noqa: E402

    WARP_AVAILABLE = kernels.HAS_WARP
except Exception as _warp_error:  # noqa: BLE001
    kernels = None
    WARP_AVAILABLE = False
    _WARP_IMPORT_ERROR = _warp_error


# -----------------------------------------------------------------------------
# Harness - same shape as the other scripts in this folder
# -----------------------------------------------------------------------------

FAILURES = []
CHECKS = [0]


def _mirror(field: "np.ndarray") -> "np.ndarray":
    """Index the (-k) cell of a periodic grid: cell 0 maps to 0, cell j to N-j.

    Note that this is NOT a plain `[::-1]` reversal.  Reversing maps j to N-1-j,
    which is off by one cell and is exactly the kind of mistake that makes a
    Hermitian-symmetry check pass on a broken field.
    """
    index = -np.arange(field.shape[0]) % field.shape[0]
    return field[np.ix_(index, index)]


def alignment_direction(kx_grid: "np.ndarray", kz_grid: "np.ndarray", wind_direction: float = 0.0) -> "np.ndarray":
    """cos of the angle between each wave vector and the wind (sign kept)."""
    k = np.sqrt(kx_grid * kx_grid + kz_grid * kz_grid)
    safe = np.where(k > 0.0, k, 1.0)
    return (kx_grid * np.cos(wind_direction) + kz_grid * np.sin(wind_direction)) / safe


def check(label: str, condition: bool, detail: str = "") -> None:
    CHECKS[0] += 1
    if not condition:
        FAILURES.append(label if not detail else f"{label} ({detail})")
    print(f"  [{'ok' if condition else 'FAIL'}] {label}" + (f" :: {detail}" if detail else ""))


def section(title: str) -> None:
    print(f"\n=== {title} ===")


# -----------------------------------------------------------------------------
# 1. Wavenumbers
# -----------------------------------------------------------------------------


def test_wavenumbers() -> None:
    section("1. Wavenumber axes")

    resolution = 64
    patch_size = 1024.0
    kx, kz = ocean.wavenumbers(resolution, patch_size)

    check("kx has one entry per sample", kx.size == resolution, f"got {kx.size}")
    check("kz matches kx", np.allclose(kx, kz))

    fundamental = 2.0 * np.pi / patch_size
    check(
        "first non-DC frequency is one patch wavelength",
        np.isclose(kx[1], fundamental),
        f"{kx[1]:.6g} vs {fundamental:.6g}",
    )
    check("DC frequency is exactly zero", kx[0] == 0.0, f"{kx[0]}")
    check(
        "the second half holds NEGATIVE frequencies",
        kx[resolution - 1] < 0.0 and np.isclose(kx[resolution - 1], -fundamental),
        f"{kx[resolution - 1]:.6g}",
    )
    check(
        "the axis is symmetric about DC",
        np.allclose(kx[1 : resolution // 2], -kx[resolution - 1 : resolution // 2 : -1]),
    )
    # The prototype this replaces flipped the sign of the negative half.  If
    # that ever comes back, the check above fails.
    check(
        "no sign folding: the negative half stays negative",
        np.all(kx[resolution // 2 + 1 :] < 0.0),
    )
    check(
        "wavelength of the fundamental spans the whole patch",
        np.isclose(2.0 * np.pi / kx[1], patch_size),
        f"{2.0 * np.pi / kx[1]:.4f}",
    )


# -----------------------------------------------------------------------------
# 2. Phillips spectrum
# -----------------------------------------------------------------------------


def test_phillips_spectrum() -> None:
    section("2. Phillips spectrum")

    parameters = ocean.OceanParameters(resolution=128, patch_size=1000.0)
    kx, kz = ocean.wavenumbers(parameters.resolution, parameters.patch_size)
    power = ocean.phillips_spectrum(
        kx,
        kz,
        wind_speed=parameters.wind_speed,
        wind_direction=parameters.wind_direction,
    )

    check("spectrum is finite everywhere", bool(np.all(np.isfinite(power))))
    check("spectrum is never negative", bool(np.all(power >= 0.0)))
    check(
        "spectrum is zero at the DC cell",
        power[0, 0] == 0.0,
        f"{power[0, 0]}",
    )
    check(
        "spectrum has measurable energy",
        float(power.max()) > 0.0,
        f"max {power.max():.6g}",
    )

    # The low-frequency cutoff is the whole point of the exponential term.  A
    # constant exponent (-1, from -(kL)/(kL)) deletes it and lets 1/k^4 blow the
    # longest waves up without bound.
    _, _, k = ocean.wavenumber_grid(kx, kz)
    peak_value = float(power.max())
    peak_k = float(k[np.unravel_index(np.argmax(power), power.shape)])
    length_scale = parameters.wind_speed**2 / ocean.GRAVITY
    # d/dk of exp(-1/(k^2 L^2)) / k^4 vanishes at k = 1 / (L * sqrt(2)).
    check(
        "energy peaks at the analytic k = 1/(L*sqrt(2))",
        np.isclose(peak_k * length_scale, 1.0 / np.sqrt(2.0), rtol=0.1),
        f"k*L = {peak_k * length_scale:.4f}, expected {1.0/np.sqrt(2.0):.4f}",
    )
    high_k = k > 3.0 / length_scale
    check(
        "the shortest waves carry less energy than the peak",
        bool(np.any(high_k)) and float(power[high_k].max()) < peak_value,
        f"{float(power[high_k].max()):.6g} vs peak {peak_value:.6g}",
    )
    # The longest waves must be suppressed by the exponential, not amplified by
    # 1/k^4.  Without the cutoff this is the largest value on the grid.
    low_k = (k > 0.0) & (k < 0.25 / length_scale)
    check(
        "the exponential suppresses the longest waves",
        bool(np.any(low_k)) and float(power[low_k].max()) < peak_value,
        f"{float(power[low_k].max()):.6g} vs peak {peak_value:.6g}",
    )

    # Directionality: full energy along the wind, none across it.  Note there is
    # deliberately NO assertion that upwind cells are empty: a real-valued
    # surface has a power spectrum symmetric under k -> -k, so P(-k) == P(k) is
    # required and the direction of travel lives in the phase.
    _, _, k_grid = ocean.wavenumber_grid(kx, kz)
    kx_grid, kz_grid, _ = ocean.wavenumber_grid(kx, kz)

    mirrored = _mirror(power)
    check(
        "the power spectrum is symmetric under k -> -k",
        np.allclose(power, mirrored, rtol=1.0e-12, atol=0.0),
        f"max relative asymmetry "
        f"{np.abs((power - mirrored) / np.where(power > 0, power, 1.0)).max():.3g}",
    )
    # Without this, sigma_k != sigma_{-k} and the field's statistics stop
    # matching the spectrum that was requested.
    check(
        "sigma is symmetric under k -> -k",
        np.allclose(np.sqrt(power), np.sqrt(mirrored), rtol=1.0e-12, atol=0.0),
    )

    across = np.abs(alignment_direction(kx_grid, kz_grid)) < 1.0e-9
    check(
        "energy is exactly zero across the wind",
        bool(np.all(power[across & (k_grid > 0.0)] == 0.0)),
    )

    downwind = (kx_grid > 0.0) & (np.abs(kz_grid) < 0.25 * kx_grid)
    crosswind = (np.abs(kx_grid) < 0.25 * np.abs(kz_grid)) & (k_grid > 0.0)
    check(
        "energy along the wind exceeds energy across it",
        bool(downwind.any() and crosswind.any() and power[downwind].max() > power[crosswind].max()),
    )

    # Rotation invariance: turning the wind in x and z together must not change
    # the total energy, only where it sits.  The tolerance is loose on purpose:
    # a square lattice is not rotationally symmetric, so rotating the wind
    # samples the directional term at different cells and the sum shifts by a
    # lattice artefact, not by an error in the spectrum.
    other = ocean.phillips_spectrum(kx, kz, wind_direction=np.pi / 3.0)
    check(
        "total energy is invariant under wind rotation",
        np.isclose(float(power.sum()), float(other.sum()), rtol=1.0e-3),
        f"{power.sum():.6g} vs {other.sum():.6g}",
    )
    check(
        "the energy distribution does move with the wind",
        not np.allclose(power, other),
    )


# -----------------------------------------------------------------------------
# 3. Time evolution and Hermitian symmetry
# -----------------------------------------------------------------------------


def test_evolve_is_hermitian() -> None:
    section("3. Time evolution")

    parameters = ocean.OceanParameters(resolution=64, patch_size=500.0)
    kx, kz = ocean.wavenumbers(parameters.resolution, parameters.patch_size)
    spectrum = ocean.initial_spectrum(parameters)

    check(
        "the initial spectrum is not Hermitian",
        not np.allclose(spectrum, np.conjugate(_mirror(spectrum))),
    )

    for time in (0.0, 0.25, 1.0, 7.5, -3.0):
        evolved = ocean.evolve(spectrum, kx, kz, time)
        mirrored = np.conjugate(_mirror(evolved))
        check(
            f"evolved field is Hermitian at t={time}",
            np.allclose(evolved, mirrored, atol=1.0e-9),
            f"max deviation {np.abs(evolved - mirrored).max():.3g}",
        )

    # Hermitian symmetry is exactly the condition that the inverse transform is
    # real.  An imaginary residue in the height field means the two-term sum is
    # missing a term.
    evolved = ocean.evolve(spectrum, kx, kz, 2.0)
    inverse = np.fft.ifft2(evolved)
    check(
        "the inverse transform is real to machine precision",
        float(np.abs(inverse.imag).max()) < 1.0e-12,
        f"max |Im| = {np.abs(inverse.imag).max():.3g}",
    )

    # The dispersion relation must be symmetric in k, otherwise the Hermitian
    # identity breaks.  omega = sqrt(g|k|) is symmetric; omega = g*k would not be.
    check(
        "dispersion is symmetric: omega(k) == omega(-k)",
        np.isclose(
            np.sqrt(ocean.GRAVITY * 2.5), np.sqrt(ocean.GRAVITY * abs(-2.5))
        ),
    )

    # Different times must give different surfaces, or nothing is animating.
    a = ocean.height_field(ocean.evolve(spectrum, kx, kz, 0.0))
    b = ocean.height_field(ocean.evolve(spectrum, kx, kz, 1.0))
    check("the surface changes with time", not np.allclose(a, b))

    # The total spectral energy is NOT instantaneously conserved.  Expanding
    # |h0(k)e^{iwt} + conj(h0(-k))e^{-iwt}|^2 leaves a cross term
    # 2*Re(h0(k)conj(h0(-k))e^{2iwt}) which oscillates: energy sloshes between
    # the k and -k components, and that sloshing is exactly what makes wave
    # groups form.  Only the EXPECTATION is invariant.  What a solver must
    # guarantee is that a single realisation stays bounded and does not drift.
    times = [0.0, 1.0, 5.0, 20.0, 100.0, 400.0]
    energies = [
        float(np.sum(np.abs(ocean.evolve(spectrum, kx, kz, t)) ** 2)) for t in times
    ]
    check(
        "spectral energy is invariant in expectation",
        np.isclose(float(np.mean(energies)), energies[0], rtol=0.25),
        f"mean {np.mean(energies):.6g} vs t=0 {energies[0]:.6g}",
    )
    check(
        "spectral energy stays bounded over time",
        max(energies) / min(energies) < 3.0,
        f"spread {min(energies):.6g}..{max(energies):.6g}",
    )
    first_half = float(np.mean(energies[:3]))
    second_half = float(np.mean(energies[3:]))
    check(
        "spectral energy does not drift over time",
        np.isclose(first_half, second_half, rtol=0.20),
        f"t<20 {first_half:.6g} vs t>20 {second_half:.6g}",
    )
    # And the surface itself must stay at a plausible amplitude for a long run.
    long_run = [
        4.0 * float(ocean.height_field(ocean.evolve(spectrum, kx, kz, t)).std())
        for t in (10.0, 60.0, 300.0)
    ]
    check(
        "the surface amplitude does not grow without bound",
        max(long_run) / min(long_run) < 1.5,
        f"Hs {min(long_run):.4f}..{max(long_run):.4f} m",
    )


# -----------------------------------------------------------------------------
# 4. Initial spectrum reproducibility
# -----------------------------------------------------------------------------


def test_initial_spectrum() -> None:
    section("4. Initial spectrum")

    one = ocean.OceanParameters(resolution=64, seed=7)
    two = ocean.OceanParameters(resolution=64, seed=7)
    other = ocean.OceanParameters(resolution=64, seed=8)

    a = ocean.initial_spectrum(one)
    b = ocean.initial_spectrum(two)
    c = ocean.initial_spectrum(other)

    check("the same seed reproduces the same field", np.array_equal(a, b))
    check("a different seed gives a different field", not np.array_equal(a, c))
    check("the field is complex with both parts populated",
          bool(np.any(a.real != 0.0) and np.any(a.imag != 0.0)))
    check("the field is finite", bool(np.all(np.isfinite(a))))

    # Variance of the amplitude should track the spectrum: E[|h0|^2] = P, so the
    # mean of |h0|^2 over the grid approaches A times the mean of the shape.
    kx, kz = ocean.wavenumbers(one.resolution, one.patch_size)
    power = ocean.phillips_spectrum(
        kx, kz, wind_speed=one.wind_speed,
        amplitude=ocean.calibrated_amplitude(one, kx, kz),
    )
    expect = float(np.mean(power))
    got = float(np.mean(np.abs(a) ** 2))
    check(
        "amplitude variance follows the spectrum within sampling noise",
        np.isclose(got, expect, rtol=0.5),
        f"got {got:.6g} expected {expect:.6g}",
    )


# -----------------------------------------------------------------------------
# 5. Normals, Jacobian and foam
# -----------------------------------------------------------------------------


def test_normals() -> None:
    section("5. Surface normals, Jacobian and foam")

    resolution = 64
    cell = 1.0
    flat = np.zeros((resolution, resolution))
    normal, jacobian = ocean.surface_normals_and_jacobian(flat, flat, flat, cell)

    check(
        "an undisplaced surface gives exactly +Y",
        np.allclose(normal, np.array([0.0, 1.0, 0.0])),
        f"corner {normal[0, 0]}",
    )
    check(
        "an undisplaced surface has unit Jacobian",
        np.allclose(jacobian, 1.0),
        f"range [{jacobian.min()}, {jacobian.max()}]",
    )
    check(
        "no foam without compression",
        float(ocean.foam_mask(jacobian).max()) == 0.0,
    )

    # A pure slope must tilt the normal by the arctangent of the slope.  The ramp
    # varies along array index 1, which is world Z, so the tilt lands on the Z
    # component of the normal.
    slope = 0.5
    xs = np.arange(resolution) * cell
    ramp = np.tile(slope * xs, (resolution, 1))
    normal, jacobian = ocean.surface_normals_and_jacobian(ramp, flat, flat, cell)
    expected_ny = 1.0 / np.sqrt(1.0 + slope**2)
    expected_nz = -slope / np.sqrt(1.0 + slope**2)
    check(
        "a ramp tilts the normal by atan(slope)",
        np.allclose(
            normal[resolution // 2, resolution // 2],
            [0.0, expected_ny, expected_nz],
            atol=1.0e-6,
        ),
        f"got {normal[resolution//2, resolution//2]}",
    )
    # And along the other axis, to pin the association in both directions.
    ramp_x = np.tile((slope * xs)[:, None], (1, resolution))
    normal_x, _ = ocean.surface_normals_and_jacobian(ramp_x, flat, flat, cell)
    check(
        "a ramp along axis 0 tilts the normal in X, not Z",
        np.allclose(
            normal_x[resolution // 2, resolution // 2],
            [-slope / np.sqrt(1.0 + slope**2), expected_ny, 0.0],
            atol=1.0e-6,
        ),
        f"got {normal_x[resolution//2, resolution//2]}",
    )

    # Real field: normals must be unit length, face up, and foam must be bounded.
    parameters = ocean.OceanParameters(resolution=resolution, patch_size=float(resolution))
    kx, kz = ocean.wavenumbers(parameters.resolution, parameters.patch_size)
    spectrum = ocean.evolve(ocean.initial_spectrum(parameters), kx, kz, 1.25)
    height = ocean.height_field(spectrum)
    disp_x, disp_z = ocean.displacement_fields(spectrum, kx, kz)
    normal, jacobian = ocean.surface_normals_and_jacobian(
        height, disp_x, disp_z, parameters.cell_size
    )
    lengths = np.linalg.norm(normal, axis=-1)
    check(
        "all normals are unit length",
        np.allclose(lengths, 1.0, atol=1.0e-9),
        f"range [{lengths.min():.9f}, {lengths.max():.9f}]",
    )
    check("all normals face up", bool(np.all(normal[:, :, 1] > 0.0)),
          f"min ny {normal[:,:,1].min():.6f}")
    check("the wave field actually tilts the normals", bool(np.any(normal[:, :, 0] != 0.0)))

    foam = ocean.foam_mask(jacobian)
    check("foam is within [0, 1]", bool(foam.min() >= 0.0 and foam.max() <= 1.0))
    check("the field has some foam where crests compress", float(foam.max()) > 0.0,
          f"max {foam.max():.4f}")

    # The finite differences must be periodic, or every patch shows a seam of
    # false normals down two of its edges.  A sinusoid is used rather than a ramp
    # because a ramp is inherently discontinuous across a periodic boundary, so
    # it can only ever test the interior.
    wavelength = resolution * cell
    xs = np.arange(resolution) * cell
    wave = np.sin(2.0 * np.pi * xs / wavelength)
    wave = np.tile(wave, (resolution, 1))
    normal_wave, _ = ocean.surface_normals_and_jacobian(wave, wave * 0.0, wave * 0.0, cell)
    dh_dx = (2.0 * np.pi / wavelength) * np.cos(2.0 * np.pi * xs / wavelength)
    # The displacement is zero, so the x tangent is (1, dh/dx, 0) and the normal
    # reduces to (-dh/dx, 1, 0) normalised.
    expected_nz = -dh_dx / np.sqrt(1.0 + dh_dx**2)
    expected_nz = np.tile(expected_nz, (resolution, 1))
    # A central difference of a sinusoid carries an O(h^2) truncation error, so
    # the check is not that the error is zero but that the SEAM is no worse than
    # the interior.  Comparing against a tight absolute tolerance would only be
    # measuring the truncation error, and would hide a real seam defect.
    error = np.abs(normal_wave[:, :, 2] - expected_nz)
    interior = error[2:-2, 2:-2].max()
    seam = max(
        error[0, :].max(), error[-1, :].max(), error[:, 0].max(), error[:, -1].max()
    )
    check(
        "the seam error is no worse than the interior truncation error",
        seam <= 1.5 * interior,
        f"seam {seam:.3e} vs interior {interior:.3e}",
    )
    check(
        "the wrap-around derivative is second-order accurate",
        interior < 1.0e-2,
        f"interior error {interior:.3e}",
    )


# -----------------------------------------------------------------------------
# 6. Packing and subsampling
# -----------------------------------------------------------------------------


def test_packing() -> None:
    section("6. Packed payload and host-side subsample")

    parameters = ocean.OceanParameters(resolution=128, patch_size=1000.0)
    kx, kz = ocean.wavenumbers(parameters.resolution, parameters.patch_size)
    spectrum = ocean.evolve(ocean.initial_spectrum(parameters), kx, kz, 3.0)
    packed, height, subsampled = ocean.packed_surface(spectrum, kx, kz, parameters)

    check("packed payload is (N, N, 4)", packed.shape == (128, 128, 4), f"{packed.shape}")
    check("packed payload is finite", bool(np.all(np.isfinite(packed))))
    check(
        "channel 0 carries the height field",
        np.allclose(packed[:, :, 0], height),
    )
    check(
        "normal channels are the x and z components",
        bool(np.all(packed[:, :, 1] <= 1.0) and np.all(packed[:, :, 1] >= -1.0)
             and np.all(packed[:, :, 2] <= 1.0) and np.all(packed[:, :, 2] >= -1.0)),
    )
    check(
        "reconstructed normal.y is non-negative",
        bool(np.all(1.0 - packed[:, :, 1] ** 2 - packed[:, :, 2] ** 2 >= -1.0e-9)),
    )
    check("foam channel is within [0, 1]",
          bool(np.all(packed[:, :, 3] >= 0.0) and np.all(packed[:, :, 3] <= 1.0)))

    check("subsample is 64 x 64", subsampled.shape == (64, 64), f"{subsampled.shape}")
    check(
        "subsample preserves the field mean",
        np.isclose(float(subsampled.mean()), float(height.mean()), rtol=1.0e-12),
        f"{subsampled.mean():.8g} vs {height.mean():.8g}",
    )
    # A box average must be strictly smoother than the source; if the reshape
    # axis order were wrong this would still pass, so also pin one block.
    block = height[:2, :2]
    check(
        "the first block average is computed over the right cells",
        np.isclose(subsampled[0, 0], float(block.mean()), rtol=1.0e-12),
        f"{subsampled[0, 0]:.8g} vs {block.mean():.8g}",
    )

    check(
        "subsample factor must divide the resolution",
        _raises(lambda: ocean.subsample_height(height, 3)),
    )


def _raises(fn) -> bool:
    try:
        fn()
    except Exception:  # noqa: BLE001 - any refusal is what we are asserting
        return True
    return False


def test_calibration() -> None:
    section("7. Sea state calibration")

    resolution = 256

    def measure(patch_size: float, wind_speed: float, time: float = 4.0,
                seed: int = 42, height: float | None = None) -> float:
        parameters = ocean.OceanParameters(
            resolution=resolution, patch_size=patch_size,
            wind_speed=wind_speed, seed=seed, significant_height=height,
        )
        kx, kz = ocean.wavenumbers(resolution, patch_size)
        spectrum = ocean.evolve(ocean.initial_spectrum(parameters), kx, kz, time)
        return 4.0 * float(ocean.height_field(spectrum).std())

    # The calibration solves for the amplitude, so the realised significant
    # height must track the empirical relation it targets.  The tolerance is
    # wider than the target's own accuracy because a single patch is a single
    # realisation of a random field: the estimate below is dominated by the
    # finite number of wave groups the patch can hold, not by the maths.
    for wind in (10.0, 15.0, 20.0):
        got = measure(1000.0, wind)
        want = ocean.significant_wave_height(wind)
        check(
            f"Hs matches the empirical relation at {wind:.0f} m/s",
            np.isclose(got, want, rtol=0.15),
            f"got {got:.4f} m, want {want:.4f} m (ratio {got/want:.4f})",
        )

    # The exact property behind the calibration, independent of realisation:
    # the solved amplitude must reproduce the target variance analytically.
    exact_wind = 15.0
    exact_params = ocean.OceanParameters(
        resolution=resolution, patch_size=1000.0, wind_speed=exact_wind
    )
    exact_kx, exact_kz = ocean.wavenumbers(resolution, exact_params.patch_size)
    shape = ocean.phillips_spectrum(
        exact_kx, exact_kz, wind_speed=exact_wind, amplitude=1.0
    )
    solved = ocean.calibrated_amplitude(exact_params, exact_kx, exact_kz)
    analytic_std = np.sqrt(2.0 * solved * float(shape.sum())) / resolution**2
    check(
        "the solved amplitude reproduces the target variance analytically",
        np.isclose(
            analytic_std, exact_params.target_significant_height / 4.0, rtol=1.0e-12
        ),
        f"std {analytic_std:.6f} vs target "
        f"{exact_params.target_significant_height / 4.0:.6f}",
    )

    # And it must NOT depend on the patch size, which is what a hard-coded
    # constant gets wrong: a short patch truncates the longest waves.
    heights = [measure(patch, 15.0) for patch in (500.0, 1000.0, 2000.0)]
    check(
        "Hs is independent of the patch size",
        max(heights) / min(heights) < 1.15,
        f"spread {min(heights):.4f}..{max(heights):.4f} m",
    )

    # And it must not depend on the grid resolution either.
    by_resolution = []
    for resolution_option in (128, 256):
        parameters = ocean.OceanParameters(
            resolution=resolution_option, patch_size=1000.0, wind_speed=15.0
        )
        kx, kz = ocean.wavenumbers(resolution_option, 1000.0)
        spectrum = ocean.evolve(ocean.initial_spectrum(parameters), kx, kz, 4.0)
        by_resolution.append(4.0 * float(ocean.height_field(spectrum).std()))
    check(
        "Hs is independent of the grid resolution",
        np.isclose(by_resolution[0], by_resolution[1], rtol=0.05),
        f"N=128 {by_resolution[0]:.4f} m, N=256 {by_resolution[1]:.4f} m",
    )

    # An explicit height must win over the wind-derived default.
    explicit = measure(1000.0, 15.0, height=10.0)
    check(
        "an explicit significant height overrides the empirical default",
        np.isclose(explicit, 10.0, rtol=0.05),
        f"got {explicit:.4f} m, want 10.0 m",
    )

    # The estimate is a single spatial realisation, so it carries real spread
    # from one seed to the next.  Measure it, so the tolerance used above is
    # known to be wider than the noise rather than accidentally inside it.
    samples = [measure(1000.0, 15.0, seed=s) for s in range(8)]
    spread = max(samples) / min(samples)
    check(
        "the seed-to-seed spread is finite and modest",
        1.0 < spread < 1.30,
        f"spread {min(samples):.4f}..{max(samples):.4f} m (ratio {spread:.3f})",
    )
    check(
        "the calibration tolerance envelopes the seed noise",
        spread <= 1.15 / 0.85,
        f"spread {spread:.3f} inside the +/-15% envelope "
        f"{1.15 / 0.85:.3f}",
    )

    # Foam must appear on the steepest crests at a normal sea state, and grow
    # with the sea state rather than being always-on or always-off.
    def coverage(wind: float, threshold: float, bias: float) -> tuple[float, float]:
        parameters = ocean.OceanParameters(
            resolution=resolution, patch_size=1000.0, wind_speed=wind
        )
        kx, kz = ocean.wavenumbers(resolution, parameters.patch_size)
        spectrum = ocean.evolve(ocean.initial_spectrum(parameters), kx, kz, 4.0)
        height = ocean.height_field(spectrum)
        disp_x, disp_z = ocean.displacement_fields(spectrum, kx, kz)
        _, jacobian = ocean.surface_normals_and_jacobian(
            height, disp_x, disp_z, parameters.cell_size
        )
        foam = ocean.foam_mask(jacobian, threshold=threshold, bias=bias)
        return float((foam > 0.01).mean()), float(foam.max())

    calm_cover, _ = coverage(5.0, 0.85, 0.35)
    fresh_cover, fresh_max = coverage(15.0, 0.85, 0.35)
    storm_cover, _ = coverage(25.0, 0.85, 0.35)
    check(
        "foam coverage grows with the sea state",
        calm_cover < fresh_cover < storm_cover,
        f"5 m/s {calm_cover:.5f} < 15 m/s {fresh_cover:.5f} < 25 m/s {storm_cover:.5f}",
    )
    check(
        "foam covers the crests but not the whole surface at 15 m/s",
        0.005 < fresh_cover < 0.25,
        f"coverage {fresh_cover*100:.2f}%",
    )
    check(
        "the foamiest crest reaches a visible intensity",
        0.1 < fresh_max <= 1.0,
        f"max {fresh_max:.3f}",
    )


# -----------------------------------------------------------------------------
# 8. Parameter validation
# -----------------------------------------------------------------------------


def test_parameters() -> None:
    section("8. Parameter validation")

    check("a supported resolution is accepted",
          ocean.OceanParameters(resolution=256).resolution == 256)
    check("the smallest power of two is accepted",
          ocean.OceanParameters(resolution=ocean.MIN_RESOLUTION).resolution
          == ocean.MIN_RESOLUTION)
    check("a non power of two is refused",
          _raises(lambda: ocean.OceanParameters(resolution=100)))
    check("a too-small resolution is refused",
          _raises(lambda: ocean.OceanParameters(resolution=8)))
    check("a zero patch size is refused",
          _raises(lambda: ocean.OceanParameters(patch_size=0.0)))
    check("a zero wind speed is refused",
          _raises(lambda: ocean.OceanParameters(wind_speed=0.0)))
    check("a negative significant height is refused",
          _raises(lambda: ocean.OceanParameters(significant_height=-1.0)))
    check("a zero significant height is refused",
          _raises(lambda: ocean.OceanParameters(significant_height=0.0)))
    check("the empirical height is used when none is given",
          np.isclose(ocean.OceanParameters(wind_speed=15.0).target_significant_height,
                     ocean.significant_wave_height(15.0)))

    base = ocean.OceanParameters(resolution=128)
    changed = base.with_changes(patch_size=2000.0)
    check("with_changes returns a new validated value",
          changed.patch_size == 2000.0 and base.patch_size == ocean.DEFAULT_PATCH_SIZE)
    check("cell size follows patch and resolution",
          np.isclose(changed.cell_size, 2000.0 / 128))
    check("wind direction zero points along +x",
          np.allclose(base.wind_direction_vector, (1.0, 0.0)))
    check("wind direction pi/2 points along +z",
          np.allclose(base.with_changes(wind_direction=np.pi / 2).wind_direction_vector,
                      (0.0, 1.0), atol=1.0e-12))


# -----------------------------------------------------------------------------
# 9. GPU transforms
# -----------------------------------------------------------------------------


def _complex_to_device(array: "np.ndarray", device: str):
    """Upload an (N, N) complex array as a Warp vec2f grid."""
    interleaved = np.ascontiguousarray(array.astype(np.complex64).view(np.float32))
    interleaved = interleaved.reshape(array.shape[0], array.shape[1], 2)
    return kernels.wp.array2d(interleaved, dtype=kernels.wp.vec2f, device=device)


def _device_to_complex(array) -> "np.ndarray":
    raw = array.numpy()
    return raw[:, :, 0] + 1j * raw[:, :, 1]


def test_gpu_transforms(device: str) -> None:
    section("9. GPU transforms")

    resolution = 128
    parameters = ocean.OceanParameters(resolution=resolution, patch_size=1000.0)
    solver = kernels.OceanSolver(parameters, device=device)

    generator = np.random.default_rng(11)
    data = (
        generator.standard_normal((resolution, resolution))
        + 1j * generator.standard_normal((resolution, resolution))
    ).astype(np.complex128)

    # Forward: unnormalised, same sign convention as numpy's fft2.
    field = _complex_to_device(data, device)
    solver.forward_transform_2d(field)
    got = _device_to_complex(field)
    expected = np.fft.fft2(data)
    scale = float(np.abs(expected).max())
    check(
        "GPU forward 2D transform matches numpy.fft.fft2",
        np.allclose(got, expected, rtol=1.0e-3, atol=scale * 1.0e-3),
        f"max relative error "
        f"{np.abs(got - expected).max() / scale:.3e}",
    )

    # Round trip: two unnormalised passes give back N^2 times the input.
    field = _complex_to_device(data, device)
    solver.forward_transform_2d(field)
    before = _device_to_complex(field)
    solver._inverse_transform_2d(field)
    after = _device_to_complex(field)
    round_trip = after / (resolution * resolution)
    scale = float(np.abs(data).max())
    check(
        "forward then inverse returns N^2 times the input",
        np.allclose(round_trip, data, rtol=1.0e-3, atol=scale * 1.0e-3),
        f"max relative error "
        f"{np.abs(round_trip - data).max() / scale:.3e}",
    )
    check(
        "the forward transform is not accidentally normalised",
        float(np.abs(before).max()) > 10.0 * float(np.abs(data).max()),
        f"peak {float(np.abs(before).max()):.1f} vs input "
        f"{float(np.abs(data).max()):.2f}",
    )

    # A different resolution through the same solver class, to prove the runtime
    # transform length actually works rather than only the one it was built for.
    for other in (64, 256):
        other_parameters = ocean.OceanParameters(resolution=other, patch_size=1000.0)
        other_solver = kernels.OceanSolver(other_parameters, device=device)
        small = generator.standard_normal((other, other)) + 0j
        field = _complex_to_device(small, device)
        other_solver.forward_transform_2d(field)
        got = _device_to_complex(field)
        want = np.fft.fft2(small)
        reference_scale = float(np.abs(want).max())
        check(
            f"resolution {other} works through the same module",
            np.allclose(got, want, rtol=1.0e-3, atol=reference_scale * 1.0e-3),
            f"max relative error "
            f"{np.abs(got - want).max() / reference_scale:.3e}",
        )
        other_solver.release()

    solver.release()


# -----------------------------------------------------------------------------
# 10. GPU pipeline against the reference
# -----------------------------------------------------------------------------


def test_gpu_pipeline(device: str) -> None:
    section("10. GPU pipeline against the numpy reference")

    resolution = 128
    patch_size = 1000.0
    foam_threshold = 0.85
    foam_bias = 0.35
    parameters = ocean.OceanParameters(
        resolution=resolution, patch_size=patch_size, wind_speed=15.0
    )
    solver = kernels.OceanSolver(
        parameters, foam_threshold=foam_threshold, foam_bias=foam_bias, device=device
    )

    kx, kz = ocean.wavenumbers(resolution, patch_size)
    for time in (0.0, 2.5, 17.0):
        solver.run(time)
        gpu = solver.packed_numpy()
        gpu_host = solver.host_height_numpy()

        spectrum = ocean.evolve(ocean.initial_spectrum(parameters), kx, kz, time)
        reference, reference_height, reference_host = ocean.packed_surface(
            spectrum, kx, kz, parameters,
            foam_threshold=foam_threshold, foam_bias=foam_bias,
        )

        height_scale = float(np.abs(reference_height).max())
        check(
            f"height field matches the reference at t={time}",
            np.allclose(gpu[:, :, 0], reference[:, :, 0], rtol=1.0e-2,
                        atol=height_scale * 1.0e-2),
            f"max error {np.abs(gpu[:, :, 0] - reference[:, :, 0]).max():.3e} "
            f"on a field of {height_scale:.3f} m",
        )

        # The normals are a finite difference, so they amplify any error in the
        # field by 1/cell; the tolerance is stated on the normal itself.
        for channel, name in ((1, "normal.x"), (2, "normal.z")):
            check(
                f"{name} matches the reference at t={time}",
                np.allclose(gpu[:, :, channel], reference[:, :, channel],
                            rtol=1.0e-1, atol=3.0e-2),
                f"max error "
                f"{np.abs(gpu[:, :, channel] - reference[:, :, channel]).max():.3e}",
            )

        check(
            f"foam matches the reference at t={time}",
            np.allclose(gpu[:, :, 3], reference[:, :, 3], rtol=1.0e-1, atol=3.0e-2),
            f"max error {np.abs(gpu[:, :, 3] - reference[:, :, 3]).max():.3e}",
        )

        reference_host_scale = float(np.abs(reference_host).max())
        check(
            f"the host height grid matches the reference at t={time}",
            np.allclose(gpu_host, reference_host, rtol=1.0e-2,
                        atol=reference_host_scale * 1.0e-2),
            f"max error {np.abs(gpu_host - reference_host).max():.3e}",
        )

        check(
            f"the GPU payload is finite at t={time}",
            bool(np.all(np.isfinite(gpu))),
        )
        reconstructed_y = np.sqrt(
            np.clip(1.0 - gpu[:, :, 1] ** 2 - gpu[:, :, 2] ** 2, 0.0, 1.0)
        )
        check(
            f"GPU normals reconstruct to unit length at t={time}",
            bool(np.all(reconstructed_y >= 0.0))
            and bool(np.all(np.isfinite(reconstructed_y))),
            f"min ny {reconstructed_y.min():.4f}",
        )
        check(
            f"the GPU height field is not flat at t={time}",
            float(np.abs(gpu[:, :, 0]).max()) > 0.0,
            f"peak {float(np.abs(gpu[:, :, 0]).max()):.4f} m",
        )

    # Repeating a frame must be deterministic, or a paused timeline would drift.
    solver.run(5.0)
    first = solver.packed_numpy()
    solver.run(5.0)
    second = solver.packed_numpy()
    check("re-running the same time is deterministic", np.array_equal(first, second))

    # The graph is built lazily on the first step.  If capture silently failed we
    # would still be correct but 60 launches a frame slower, so assert it here
    # rather than discover it from a frame budget later.
    if str(device).startswith("cuda"):
        check(
            "the frame is replayed from a CUDA graph",
            solver.graph_captured,
            "capture fell back to per-kernel launches",
        )
    else:
        check(
            "no graph is expected on a CPU device",
            not solver.graph_captured,
        )

    # Accuracy of the packed displacement transform, stated rather than assumed.
    # Both displacements ride in ONE complex transform, so their error is worth
    # measuring against the reference the same way the height field is.
    solver.run(6.0)
    gpu_dx, gpu_dz = solver.displacements_numpy()
    packed_at_six = solver.packed_numpy()
    reference_spectrum = ocean.evolve(ocean.initial_spectrum(parameters), kx, kz, 6.0)
    reference_dx, reference_dz = ocean.displacement_fields(
        reference_spectrum, kx, kz
    )
    for gpu_field, reference_field, name in (
        (gpu_dx, reference_dx, "disp_x"),
        (gpu_dz, reference_dz, "disp_z"),
    ):
        span = float(np.abs(reference_field).max())
        error = float(np.abs(gpu_field - reference_field).max())
        check(
            f"{name} agrees with the reference to float32 precision",
            error < span * 1.0e-4,
            f"max error {error:.3e} on a {span:.3f} m field "
            f"({error / span:.2e} relative)",
        )

    solver_nograph = kernels.OceanSolver(
        parameters, foam_threshold=foam_threshold, foam_bias=foam_bias,
        device=device, use_graph=False,
    )
    check(
        "capture can be switched off",
        not solver_nograph._use_graph,
    )
    if str(device).startswith("cuda"):
        solver_nograph.run(6.0)
        check(
            "the no-graph path produces the same surface as the graph path",
            np.allclose(
                solver_nograph.packed_numpy(), packed_at_six, rtol=1.0e-4, atol=1.0e-4
            ),
        )
    solver_nograph.release()

    solver.release()


# -----------------------------------------------------------------------------
# 10b. Choppiness
# -----------------------------------------------------------------------------


def test_choppiness(device: str) -> None:
    section("10b. Choppiness scales the horizontal displacement")

    resolution = 64
    patch_size = 500.0
    base = ocean.OceanParameters(
        resolution=resolution, patch_size=patch_size, wind_speed=15.0
    )
    chopped = base.with_changes(choppiness=2.0)
    kx, kz = ocean.wavenumbers(resolution, patch_size)
    spectrum = ocean.initial_spectrum(base)

    disp_x1, disp_z1 = ocean.displacement_fields(spectrum, kx, kz, 1.0)
    disp_x2, disp_z2 = ocean.displacement_fields(spectrum, kx, kz, 2.0)
    disp_x0, disp_z0 = ocean.displacement_fields(spectrum, kx, kz, 0.0)

    check(
        "choppiness scales disp_x linearly",
        np.allclose(disp_x2, 2.0 * disp_x1, rtol=1.0e-10, atol=1.0e-12),
        f"max error {np.abs(disp_x2 - 2.0 * disp_x1).max():.3e}",
    )
    check(
        "choppiness scales disp_z linearly",
        np.allclose(disp_z2, 2.0 * disp_z1, rtol=1.0e-10, atol=1.0e-12),
        f"max error {np.abs(disp_z2 - 2.0 * disp_z1).max():.3e}",
    )
    check(
        "choppiness 0 leaves the surface undisplaced",
        float(np.abs(disp_x0).max()) == 0.0 and float(np.abs(disp_z0).max()) == 0.0,
    )
    check(
        "a negative choppiness is rejected",
        _raises(lambda: ocean.OceanParameters(choppiness=-0.5)),
    )

    # With no horizontal displacement the Jacobian is exactly 1 everywhere, so the
    # foam term is identically zero.  That is the physical statement behind the
    # parameter: the displacement is what makes foam possible at all.
    height = ocean.height_field(spectrum)
    _, jacobian0 = ocean.surface_normals_and_jacobian(
        height, disp_x0, disp_z0, base.cell_size
    )
    _, jacobian1 = ocean.surface_normals_and_jacobian(
        height, disp_x1, disp_z1, base.cell_size
    )
    check(
        "an undisplaced surface has a Jacobian of exactly 1",
        bool(np.allclose(jacobian0, 1.0, rtol=0.0, atol=1.0e-12)),
        f"[{jacobian0.min():.6f}, {jacobian0.max():.6f}]",
    )
    check(
        "an undisplaced surface never foams",
        float(ocean.foam_mask(jacobian0, 0.85, 0.35).max()) == 0.0,
    )
    check(
        "displacing the surface does compress it somewhere",
        float(jacobian1.min()) < 1.0,
        f"min Jacobian {jacobian1.min():.4f}",
    )

    # The GPU has to honour it too: a factor applied only on the reference side
    # would hide until someone compared the picture against the physics.
    time = 3.0
    solver = kernels.OceanSolver(
        chopped, foam_threshold=0.85, foam_bias=0.35, device=device
    )
    solver.run(time)
    gpu = solver.packed_numpy()

    evolved = ocean.evolve(ocean.initial_spectrum(chopped), kx, kz, time)
    reference, reference_height, _ = ocean.packed_surface(
        evolved, kx, kz, chopped, foam_threshold=0.85, foam_bias=0.35
    )
    check(
        "the GPU honours choppiness: height matches",
        np.allclose(
            gpu[:, :, 0],
            reference[:, :, 0],
            rtol=1.0e-2,
            atol=float(np.abs(reference_height).max()) * 1.0e-2,
        ),
        f"max error {np.abs(gpu[:, :, 0] - reference[:, :, 0]).max():.3e}",
    )
    for channel, name in ((1, "normal.x"), (2, "normal.z"), (3, "foam")):
        check(
            f"the GPU honours choppiness: {name} matches",
            np.allclose(
                gpu[:, :, channel], reference[:, :, channel], rtol=1.0e-1, atol=3.0e-2
            ),
            f"max error {np.abs(gpu[:, :, channel] - reference[:, :, channel]).max():.3e}",
        )

    plain = kernels.OceanSolver(
        base, foam_threshold=0.85, foam_bias=0.35, device=device
    )
    plain.run(time)
    plain_payload = plain.packed_numpy()
    check(
        "choppiness 2 is a different surface from choppiness 1",
        not np.allclose(gpu, plain_payload, rtol=1.0e-3, atol=1.0e-4),
    )
    # The identity mode must NOT pick up the factor: scaling it would flatten or
    # inflate the sea itself instead of only sharpening the crests.
    check(
        "the height field is identical whatever the choppiness",
        np.allclose(gpu[:, :, 0], plain_payload[:, :, 0], rtol=1.0e-6, atol=1.0e-6),
        f"max error {np.abs(gpu[:, :, 0] - plain_payload[:, :, 0]).max():.3e}",
    )


# -----------------------------------------------------------------------------
# 11. GPU performance
# -----------------------------------------------------------------------------


def test_gpu_performance(device: str) -> None:
    section("11. GPU frame cost")

    import time as _time

    for resolution in (128, 256, 512):
        parameters = ocean.OceanParameters(
            resolution=resolution, patch_size=1000.0, wind_speed=15.0
        )

        timings = {}
        for label, use_graph in (("graph", True), ("plain", False)):
            build_start = _time.perf_counter()
            solver = kernels.OceanSolver(
                parameters, device=device, use_graph=use_graph
            )
            solver.run(0.0)
            solver.synchronize()
            build_ms = (_time.perf_counter() - build_start) * 1000.0

            iterations = 30
            start = _time.perf_counter()
            for step in range(iterations):
                solver.run(0.1 * step)
            solver.synchronize()
            per_frame_ms = (_time.perf_counter() - start) * 1000.0 / iterations
            timings[label] = (per_frame_ms, build_ms, solver.graph_captured)
            solver.release()

        graph_ms, graph_build, captured = timings["graph"]
        plain_ms, plain_build, _ = timings["plain"]
        print(
            f"  N={resolution:4d}  graph {graph_ms:6.2f} ms/frame   "
            f"plain {plain_ms:7.2f} ms/frame   speedup {plain_ms / graph_ms:5.2f}x   "
            f"(build {graph_build:.0f}/{plain_build:.0f} ms, captured={captured})"
        )
        check(
            f"N={resolution} stays inside a 16 ms frame budget",
            graph_ms < 16.0,
            f"graph {graph_ms:.2f} ms/frame, plain {plain_ms:.2f} ms/frame",
        )
        if str(device).startswith("cuda"):
            check(
                f"N={resolution} is faster with the graph than without",
                graph_ms < plain_ms,
                f"{graph_ms:.2f} ms vs {plain_ms:.2f} ms",
            )


# -----------------------------------------------------------------------------
# Runner
# -----------------------------------------------------------------------------


def main() -> int:
    print(f"repo root: {_ROOT}")
    print(f"numpy: {np.__version__}")

    test_wavenumbers()
    test_phillips_spectrum()
    test_evolve_is_hermitian()
    test_initial_spectrum()
    test_normals()
    test_packing()
    test_calibration()
    test_parameters()

    exit_code = 0
    if WARP_AVAILABLE:
        device = kernels.preferred_device()
        print(f"\nGPU device: {device}")
        test_gpu_transforms(device)
        test_gpu_pipeline(device)
        test_choppiness(device)
        test_gpu_performance(device)
    else:
        print("\nWARNING: warp is not importable, GPU sections 9-11 were SKIPPED")
        exit_code = 2

    print(f"\n{CHECKS[0] - len(FAILURES)}/{CHECKS[0]} checks passed")
    if FAILURES:
        print("FAILED: " + "; ".join(FAILURES))
        return 1
    if not WARP_AVAILABLE:
        print("ALL OCEAN SPECTRUM TESTS PASSED (GPU sections skipped)")
        return exit_code
    print("ALL OCEAN TESTS PASSED")
    return 0


if __name__ == "__main__":
    sys.exit(main())
