# =============================================================================
# ocean_spectrum.py
# Spectral ocean reference implementation (Phillips spectrum / Tessendorf).
#
# This module is the SINGLE SOURCE OF TRUTH for the wave spectrum and for the
# construction of the complex amplitude field.  The GPU pipeline in
# `ocean_fft_kernels.py` does NOT reimplement any of this maths: it receives the
# initial spectrum produced here and only performs the per-frame evolution,
# transforms and packing.  If the spectrum ever needs to change, it changes
# here, so the two paths cannot drift apart.
#
# Conventions
# -----------
# - The grid is (N, N) over the axes (x, z); up is +Y.  Units are metres.
# - `kx` / `kz` follow `numpy.fft.fftfreq` ordering: the second half of each
#   axis holds NEGATIVE wavenumbers.  No shift, no sign folding.  This matters:
#   folding the sign of the negative half mirrors the spectrum instead of
#   centring it, which is a real defect in the prototype this replaces.
# - Wavenumbers are in radians per metre.
#
# No Kit and no Warp imports: this module is importable by a plain interpreter.
# =============================================================================

from __future__ import annotations

from dataclasses import dataclass, replace
from typing import Optional, Tuple

try:
    import numpy as np

    HAS_NUMPY = True
except ImportError:  # pragma: no cover - only true in a bare interpreter
    np = None
    HAS_NUMPY = False


# -----------------------------------------------------------------------------
# Parameters
# -----------------------------------------------------------------------------

#: The reference implementation works for any power-of-two grid.  The GPU
#: pipeline has a stricter rule of its own (see `ocean_fft_kernels`), because
#: `wp.tile_fft` requires at least two elements per thread in a tile.
MIN_RESOLUTION = 16

DEFAULT_RESOLUTION = 256
DEFAULT_PATCH_SIZE = 1000.0
DEFAULT_WIND_SPEED = 15.0
DEFAULT_WIND_DIRECTION = 0.0
DEFAULT_AMPLITUDE = 1.0
DEFAULT_CHOPPINESS = 1.0
DEFAULT_SEED = 42
GRAVITY = 9.81

#: Below this wavenumber magnitude the spectrum is forced to zero.  Without a
#: floor, `1 / k^4` is undefined at the DC cell, and the exponent below cannot
#: be evaluated.  The Phillips low-frequency cutoff makes this cell approach
#: zero anyway, so this only removes the singularity.
MIN_WAVENUMBER = 1.0e-4



def _is_power_of_two(value: int) -> bool:
    return value > 0 and (value & (value - 1)) == 0


@dataclass(frozen=True)
class OceanParameters:
    """Everything that defines one ocean patch's wave field.

    Frozen on purpose: a patch rebuilds its spectrum from these values, so they
    must be an immutable snapshot rather than a live reference into the stage.
    """

    resolution: int = DEFAULT_RESOLUTION
    patch_size: float = DEFAULT_PATCH_SIZE
    wind_speed: float = DEFAULT_WIND_SPEED
    wind_direction: float = DEFAULT_WIND_DIRECTION
    significant_height: Optional[float] = None
    #: Scales the horizontal displacement of the surface.  1.0 is the physical
    #: value the spectrum implies, 0.0 leaves the surface undisplaced (symmetric
    #: crests, and the Jacobian never compresses, so no foam), and >1 sharpens the
    #: crests further than the spectrum alone would.
    choppiness: float = DEFAULT_CHOPPINESS
    seed: int = DEFAULT_SEED
    gravity: float = GRAVITY

    def __post_init__(self) -> None:
        if not _is_power_of_two(self.resolution) or self.resolution < MIN_RESOLUTION:
            raise ValueError(
                f"resolution must be a power of two and at least {MIN_RESOLUTION}, "
                f"got {self.resolution}"
            )
        if self.patch_size <= 0.0:
            raise ValueError(f"patch_size must be positive, got {self.patch_size}")
        if self.wind_speed <= 0.0:
            raise ValueError(f"wind_speed must be positive, got {self.wind_speed}")
        if self.significant_height is not None and self.significant_height <= 0.0:
            raise ValueError(
                f"significant_height must be positive when given, got {self.significant_height}"
            )
        if self.choppiness < 0.0:
            # A negative factor would displace the surface the wrong way, which is
            # not a sea state anyone wants and not a mistake worth rendering.
            raise ValueError(f"choppiness must not be negative, got {self.choppiness}")
        if self.gravity <= 0.0:
            raise ValueError(f"gravity must be positive, got {self.gravity}")

    @property
    def cell_size(self) -> float:
        """Distance between two adjacent grid samples, in metres."""
        return self.patch_size / self.resolution

    @property
    def wind_direction_vector(self) -> Tuple[float, float]:
        """Wind direction as a unit vector on the (x, z) plane."""
        return (
            float(np.cos(self.wind_direction)),
            float(np.sin(self.wind_direction)),
        )

    def with_changes(self, **kwargs) -> "OceanParameters":
        """Return a copy with the given fields replaced (validated again)."""
        return replace(self, **kwargs)

    @property
    def target_significant_height(self) -> float:
        """Significant wave height to build, in metres.

        An explicit `significant_height` wins; otherwise it is derived from the
        wind speed through the empirical relation.  Keeping the wind speed as
        the primary control and the height as an override means the common case
        (`ocean:windSpeed` only) needs no extra authoring.
        """
        if self.significant_height is not None:
            return float(self.significant_height)
        return significant_wave_height(self.wind_speed, self.gravity)


def _require_numpy() -> None:
    if not HAS_NUMPY:
        raise RuntimeError(
            "ocean_spectrum requires numpy. In a bare interpreter, add the Kit "
            "prebundle to sys.path first (see scripts/test_ocean_fft.py)."
        )


# -----------------------------------------------------------------------------
# Wavenumbers
# -----------------------------------------------------------------------------


def wavenumbers(resolution: int, patch_size: float) -> Tuple["np.ndarray", "np.ndarray"]:
    """Return the (kx, kz) axes in `fftfreq` ordering, in radians per metre.

    Sample spacing is `patch_size / resolution`, which is exactly the cell size
    of the generated surface grid, so the discrete frequencies line up with the
    geometry rather than being off by one cell.
    """
    _require_numpy()
    spacing = patch_size / resolution
    kx = np.fft.fftfreq(resolution, d=spacing) * 2.0 * np.pi
    kz = np.fft.fftfreq(resolution, d=spacing) * 2.0 * np.pi
    return kx.astype(np.float64), kz.astype(np.float64)


def wavenumber_grid(
    kx: "np.ndarray", kz: "np.ndarray"
) -> Tuple["np.ndarray", "np.ndarray", "np.ndarray"]:
    """Broadcast the axes into full grids and return (KX, KZ, K)."""
    _require_numpy()
    kx_grid = np.broadcast_to(kx[:, None], (kx.size, kz.size)).astype(np.float64)
    kz_grid = np.broadcast_to(kz[None, :], (kx.size, kz.size)).astype(np.float64)
    return kx_grid, kz_grid, np.sqrt(kx_grid * kx_grid + kz_grid * kz_grid)


# -----------------------------------------------------------------------------
# Phillips spectrum
# -----------------------------------------------------------------------------


def phillips_spectrum(
    kx: "np.ndarray",
    kz: "np.ndarray",
    wind_speed: float = DEFAULT_WIND_SPEED,
    wind_direction: float = DEFAULT_WIND_DIRECTION,
    amplitude: float = DEFAULT_AMPLITUDE,
    gravity: float = GRAVITY,
) -> "np.ndarray":
    """Return the Phillips power spectrum P(k) on the `(kx, kz)` grid.

    .. math::
        P(k) = A \\frac{\\exp\\left(-\\frac{1}{(kL)^2}\\right)}{k^4}
               \\left(\\hat{k}\\cdot\\hat{w}\\right)^2,
        \\qquad L = \\frac{V^2}{g}

    Three properties are load-bearing, and each replaces a defect in the
    prototype this work supersedes:

    1. The exponent is ``-1 / (k^2 * L^2)``.  Note that ``-1/(kL) * (kL)``
       collapses to the constant ``-1``, which silently deletes the
       low-frequency cutoff and lets ``1/k^4`` amplify the longest waves
       without bound.
    2. The directionality is the SQUARED dot product, so energy peaks ALONG the
       wind and is exactly zero across it.  A term such as ``cos(phi * pi/2)``
       instead peaks perpendicular to the wind.
    3. The directionality is NOT clamped to ``max(0, .)``.  A real-valued surface
       always has a power spectrum symmetric under ``k -> -k``, because the
       component at ``-k`` is the conjugate of the component at ``k``.  Clamping
       removes that half of the grid and therefore produces an anisotropic field
       whose statistics do not match the requested spectrum - and it breaks the
       ``sigma_k == sigma_{-k}`` symmetry that the time evolution assumes.
       Direction of travel is carried by the PHASE, not by the power.
    """
    _require_numpy()
    kx_grid, kz_grid, k = wavenumber_grid(kx, kz)

    # k^4 grows far slower than the exponential decays as k -> 0, so P -> 0 at
    # the origin.  The floor only removes the 0/0 at the DC cell.
    safe_k = np.where(k > MIN_WAVENUMBER, k, 1.0)

    inverse_k = 1.0 / safe_k
    k_squared = safe_k * safe_k
    k_fourth = k_squared * k_squared

    length_scale = (wind_speed * wind_speed) / gravity
    exponent = -1.0 / (k_squared * length_scale * length_scale)
    falloff = np.exp(exponent)

    wind_x = np.cos(wind_direction)
    wind_z = np.sin(wind_direction)
    # k . w / |k|, i.e. cos of the angle between the wave vector and the wind.
    # Squaring it keeps the result non-negative AND symmetric under k -> -k,
    # which a clamp would not.
    alignment = (kx_grid * wind_x + kz_grid * wind_z) * inverse_k
    directional = alignment * alignment

    spectrum = amplitude * falloff * directional / k_fourth
    spectrum[k <= MIN_WAVENUMBER] = 0.0
    return spectrum


def significant_wave_height(
    wind_speed: float = DEFAULT_WIND_SPEED, gravity: float = GRAVITY
) -> float:
    """Empirical significant wave height of a fully developed sea, in metres.

    .. math::  H_s \approx 0.21 \frac{V^2}{g}

    This is the DEFAULT sea state for a given wind speed.  It is only an
    empirical relation, so it is used as a target rather than as truth: the
    spectrum amplitude is solved for it exactly (see `calibrated_amplitude`).
    """
    return 0.21 * wind_speed * wind_speed / gravity


def calibrated_amplitude(
    parameters: "OceanParameters", kx: "np.ndarray", kz: "np.ndarray"
) -> float:
    """Solve for the Phillips multiplier A that yields the target wave height.

    The height field is ``h(x) = (1/N^2) * sum_k h~(k) e^{ikx}``, so

    .. math::
        \\sigma_h^2 = \\frac{2}{N^4} \\sum_k P(k), \\qquad
        H_s = 4\\,\\sigma_h, \\qquad
        A = \\left(\\frac{H_s N^2}{4}\\right)^2 \\Big/ \\left(2 \\sum_k Q(k)\\right)

    where ``Q`` is the same spectrum evaluated with ``A = 1``, and the factor of
    2 comes from the Hermitian pair: the component at ``-k`` carries as much
    energy as the one at ``k``.

    Solving for the amplitude directly - rather than hard-coding a constant -
    is what keeps the sea state independent of the patch size and of the grid
    resolution.  A constant only produces the right wave height for the one
    patch size it was measured on: a 500 m patch truncates the longest waves and
    comes out at a very different height, which is easy to mistake for an
    artistic choice rather than an error.
    """
    _require_numpy()
    shape = phillips_spectrum(
        kx,
        kz,
        wind_speed=parameters.wind_speed,
        wind_direction=parameters.wind_direction,
        amplitude=1.0,
        gravity=parameters.gravity,
    )
    total = float(shape.sum())
    if total <= 0.0:
        raise ValueError("the spectrum has no energy; check wind_speed and patch_size")

    target_std = parameters.target_significant_height / 4.0
    return (target_std * parameters.resolution**2) ** 2 / (2.0 * total)


# -----------------------------------------------------------------------------
# Complex amplitude field
# -----------------------------------------------------------------------------


def gaussian_field(parameters: OceanParameters) -> "np.ndarray":
    """Deterministic unit-variance Gaussian field of shape (N, N, 2).

    The random draw happens HERE, on the host, and is uploaded to the device.
    That is deliberate: it makes the whole pipeline reproducible and comparable
    against the GPU without depending on a device-side generator's stream.
    """
    _require_numpy()
    generator = np.random.default_rng(parameters.seed)
    return generator.standard_normal((parameters.resolution, parameters.resolution, 2))


def initial_spectrum(parameters: OceanParameters) -> "np.ndarray":
    """Return h0(k), the complex amplitude field of shape (N, N).

    .. math::
        \\tilde{h}_0(k) = \\frac{1}{\\sqrt{2}} (\\xi_r + i\\xi_i)\\sqrt{P(k)}

    The field is NOT made Hermitian here.  It does not need to be: the two-term
    time evolution in :func:`evolve` is Hermitian for any h0, which is a
    property proved by `test_ocean_fft.py::test_evolve_is_hermitian`.  Fabricating
    the conjugates up front would be redundant work.
    """
    _require_numpy()
    kx, kz = wavenumbers(parameters.resolution, parameters.patch_size)
    power = phillips_spectrum(
        kx,
        kz,
        wind_speed=parameters.wind_speed,
        wind_direction=parameters.wind_direction,
        amplitude=calibrated_amplitude(parameters, kx, kz),
        gravity=parameters.gravity,
    )
    sigma = np.sqrt(power * 0.5)
    gaussian = gaussian_field(parameters)
    return (sigma * (gaussian[:, :, 0] + 1j * gaussian[:, :, 1])).astype(np.complex128)


def evolve(
    spectrum: "np.ndarray",
    kx: "np.ndarray",
    kz: "np.ndarray",
    time: float,
    gravity: float = GRAVITY,
) -> "np.ndarray":
    """Evolve the spectrum in time (deep-water dispersion).

    .. math::
        \\tilde{h}(k,t) = \\tilde{h}_0(k)e^{i\\omega t}
                        + \\overline{\\tilde{h}_0(-k)}e^{-i\\omega t},
        \\qquad \\omega = \\sqrt{g|k|}

    Both terms are required.  With only the first, the field is not real-valued,
    so taking the real part of the inverse transform discards about half the
    energy and produces an asymmetric, non-physical surface.
    """
    _require_numpy()
    _, _, k = wavenumber_grid(kx, kz)
    omega = np.sqrt(gravity * k) * time
    phase = np.exp(1j * omega)
    mirrored = spectrum[np.ix_(-np.arange(kx.size) % kx.size,
                               -np.arange(kz.size) % kz.size)]
    return spectrum * phase + np.conjugate(mirrored) * np.conjugate(phase)


def derivative_factor(kx: "np.ndarray", kz: "np.ndarray", axis: int) -> "np.ndarray":
    """Frequency-domain factor for the horizontal displacement (choppiness).

    ``axis=0`` gives the x displacement, ``axis=1`` the z displacement:

    .. math::
        D_x(k) = -i\\frac{k_x}{|k|}\\tilde{h}(k), \\qquad
        D_z(k) = -i\\frac{k_z}{|k|}\\tilde{h}(k)

    Displacing the surface horizontally is what sharpens wave crests, which in
    turn steepens the normals and is what drives the foam term.
    """
    _require_numpy()
    kx_grid, kz_grid, k = wavenumber_grid(kx, kz)
    safe_k = np.where(k > MIN_WAVENUMBER, k, 1.0)
    component = kx_grid if axis == 0 else kz_grid
    return (-1j * component / safe_k).astype(np.complex128)


# -----------------------------------------------------------------------------
# Fields
# -----------------------------------------------------------------------------


def height_field(spectrum: "np.ndarray") -> "np.ndarray":
    """Real height field h(x, z) from the evolved spectrum.

    `numpy.fft.ifft2` normalises by 1/N^2 internally, which the GPU pipeline has
    to do explicitly (Warp's `tile_ifft` is unnormalised).
    """
    _require_numpy()
    return np.real(np.fft.ifft2(spectrum))


def displacement_fields(
    spectrum: "np.ndarray",
    kx: "np.ndarray",
    kz: "np.ndarray",
    choppiness: float = 1.0,
) -> Tuple["np.ndarray", "np.ndarray"]:
    """Return the (horizontal x, horizontal z) displacement fields.

    `choppiness` scales the factor in the frequency domain, which is exact
    because the transform is linear: the result is identical to scaling the two
    displacement fields afterwards.  Scaling the FACTOR is what the GPU kernel
    does too, so the two paths stay comparable term by term.
    """
    _require_numpy()
    if choppiness < 0.0:
        raise ValueError(f"choppiness must not be negative, got {choppiness}")
    factor_x = derivative_factor(kx, kz, axis=0) * choppiness
    factor_z = derivative_factor(kx, kz, axis=1) * choppiness
    disp_x = np.real(np.fft.ifft2(spectrum * factor_x))
    disp_z = np.real(np.fft.ifft2(spectrum * factor_z))
    return disp_x, disp_z


def _central_difference(field: "np.ndarray", axis: int, cell_size: float) -> "np.ndarray":
    """Central difference with wrap-around, because the field is periodic."""
    _require_numpy()
    return (np.roll(field, -1, axis=axis) - np.roll(field, 1, axis=axis)) / (
        2.0 * cell_size
    )


def surface_normals_and_jacobian(
    height: "np.ndarray", disp_x: "np.ndarray", disp_z: "np.ndarray", cell_size: float
) -> Tuple["np.ndarray", "np.ndarray"]:
    """Return (unit normals, Jacobian) of the displaced surface.

    The surface point is ``P = (x + Dx, h, z + Dz)``, so

    .. math::
        \\frac{\\partial P}{\\partial x} = (1 + \\partial_x D_x,\\ \\partial_x h,\\
                                            \\partial_x D_z), \\qquad
        n = \\frac{\\partial_z P \\times \\partial_x P}
                 {\\lVert \\partial_z P \\times \\partial_x P \\rVert}

    The cross product is taken in that order so an undisplaced surface yields
    ``+Y``.  Its Y component before normalisation equals the Jacobian of the
    horizontal displacement, which is the quantity that says how much the
    surface is being compressed - i.e. where foam belongs.

    Axis convention, which the kernel mirrors exactly: array index 0 runs along
    world X and carries ``kx``, array index 1 runs along world Z and carries
    ``kz``.  Getting this association wrong swaps the x and z components of every
    normal, which is nearly invisible on a square patch with symmetric
    statistics and therefore worth stating rather than inferring.
    """
    _require_numpy()
    ddx_dx = _central_difference(disp_x, 0, cell_size)
    dh_dx = _central_difference(height, 0, cell_size)
    ddz_dx = _central_difference(disp_z, 0, cell_size)

    ddx_dz = _central_difference(disp_x, 1, cell_size)
    dh_dz = _central_difference(height, 1, cell_size)
    ddz_dz = _central_difference(disp_z, 1, cell_size)

    tangent_x = np.stack((1.0 + ddx_dx, dh_dx, ddz_dx), axis=-1)
    tangent_z = np.stack((ddx_dz, dh_dz, 1.0 + ddz_dz), axis=-1)

    normal = np.cross(tangent_z, tangent_x)
    jacobian = normal[:, :, 1]
    length = np.linalg.norm(normal, axis=-1, keepdims=True)
    normal = normal / np.where(length > 1.0e-12, length, 1.0)

    # A negative Y means the surface folded over itself; flip so that every
    # normal faces the sky, which is what a shading normal must do.
    flip = np.where(normal[:, :, 1:2] < 0.0, -1.0, 1.0)
    return normal * flip, jacobian


def foam_mask(jacobian: "np.ndarray", threshold: float = 0.6, bias: float = 0.4) -> "np.ndarray":
    """Foam coverage in [0, 1] from the Jacobian of the horizontal displacement."""
    _require_numpy()
    if bias <= 0.0:
        raise ValueError(f"bias must be positive, got {bias}")
    return np.clip((threshold - jacobian) / bias, 0.0, 1.0)


def packed_surface(
    spectrum: "np.ndarray",
    kx: "np.ndarray",
    kz: "np.ndarray",
    parameters: OceanParameters,
    foam_threshold: float = 0.6,
    foam_bias: float = 0.4,
) -> Tuple["np.ndarray", "np.ndarray", "np.ndarray"]:
    """Full reference pipeline: (packed RGBA, height, subsampled height).

    The packed layout is `(height, normal.x, normal.z, foam)`.  `normal.y` is
    omitted because it is non-negative by construction and can be recovered as
    ``sqrt(1 - x^2 - z^2)``, and because the payload dtype of the dynamic
    texture path is a 4-component float vector, which this fills exactly.
    """
    _require_numpy()
    height = height_field(spectrum)
    disp_x, disp_z = displacement_fields(spectrum, kx, kz, parameters.choppiness)
    normal, jacobian = surface_normals_and_jacobian(
        height, disp_x, disp_z, parameters.cell_size
    )
    foam = foam_mask(jacobian, threshold=foam_threshold, bias=foam_bias)

    packed = np.empty((parameters.resolution, parameters.resolution, 4), dtype=np.float64)
    packed[:, :, 0] = height
    packed[:, :, 1] = normal[:, :, 0]
    packed[:, :, 2] = normal[:, :, 2]
    packed[:, :, 3] = foam

    subsampled = subsample_height(height, parameters.resolution // 64)
    return packed, height, subsampled


def subsample_height(height: "np.ndarray", factor: int) -> "np.ndarray":
    """Box-average `height` down by `factor` for the host-side wave query.

    The gameplay systems need the water height at a world position every step.
    Reading back the full N x N field would be wasteful; a small averaged grid
    is both cheaper to copy and a better answer, because buoyancy wants the mean
    height over the submerged footprint rather than a single sample.
    """
    _require_numpy()
    if factor < 1:
        raise ValueError(f"factor must be at least 1, got {factor}")
    resolution = height.shape[0]
    if resolution % factor != 0:
        raise ValueError(
            f"factor {factor} must divide the resolution {resolution}"
        )
    side = resolution // factor
    return height.reshape(side, factor, side, factor).mean(axis=(1, 3))
