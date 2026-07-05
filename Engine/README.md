# Hydragon Next (engine core)

Filament-first rewrite of the Hydragon engine core, on branch `rewrite/filament-core`.
The legacy core is preserved in git history and on branch `alpha-feature-vulkan-backend`,
and copied under `../EngineLegacy/` (git-ignored) for local reference.

The architecture, philosophy, repository layout, build pipeline, toolchain, setup
instructions and development phases are documented once, in the **repository root**
[`README.md`](../README.md) — the workspace root is one level above this `Engine/` folder.

Quick pointers:

- First-time setup: run `./scripts/setup.ps1` from the repo root, then open `Hydragon.sln`.
- Our source lives under `Engine/Source/`; external dependencies under `Engine/ThirdParty/`;
  build artifacts under `Engine/Bin/` and `Engine/BuildOutput/` (both git-ignored).
- Enable the renderer with `UseFilament=true` (see the root README, "Enabling Filament").
