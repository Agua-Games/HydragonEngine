# Hydragon Next

Filament-first rewrite of the Hydragon engine core, on branch `rewrite/filament-core`.
The legacy core stays intact under `../Engine/` (branch `alpha-feature-vulkan-backend`).

## Philosophy (carried over from design docs)

- **Bare-mode core**: headless, no windowing/GUI dependency in the core library.
- **USD as authoritative backend**: scene graph, state and hierarchy live in OpenUSD.
- **Filament as renderer**: direct USD -> Filament translation (Hydra delegate optional, later).
- **Agnostic nodes**: every entity is a scene graph (USD composition arcs / Filament `TransformManager`).

## Toolchain

- Visual Studio 2022 (MSBuild, toolset v143, C++20).
- **vcpkg** (manifest mode) for `imgui`, `glfw3`, `glm`, `spdlog`, and later `usd`.
- **Filament** as a prebuilt SDK under `ThirdParty/Filament/` (not on vcpkg).

Dependency include/lib paths are centralized in `Directory.Build.props` — you should
not need to hand-edit any `.vcxproj`.

## First-time setup

```powershell
# From a Developer PowerShell at HydragonNext/
./scripts/setup.ps1
```

This bootstraps vcpkg (`vcpkg integrate install`) and downloads the Filament SDK.
Then open `HydragonNext.sln` in Visual Studio and build.

## Phases

| Phase | Goal | Switch |
| --- | --- | --- |
| 0 | Toolchain smoke test (`App` prints and runs) | default |
| 1 | Filament hello-cube (window + PBR cube) | `<UseFilament>true</UseFilament>` |
| 2 | ImGui overlay editor (decoupled from core) | vcpkg `imgui` |
| 3 | OpenUSD backend (traverse prims -> Filament entities) | add `usd` to `vcpkg.json` |
| 4 | WavePhysics (XPBD + fields) | — |

### Enabling Filament (Phase 1)

After `setup.ps1` populated `ThirdParty/Filament/`, set the project property
`UseFilament=true` (e.g. in `Directory.Build.props` or via `/p:UseFilament=true`),
then include Filament headers in `App/main.cpp`.

## Note

Google Filament builds its own SLM/materials with CMake upstream, but here it is
consumed **prebuilt**, so your day-to-day loop stays 100% in Visual Studio.
If you ever prefer CMake + `CMakePresets.json` (VS opens those natively too, and
it is how Filament/USD build from source), that path remains available.
