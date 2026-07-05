# Hydragon Engine

Hydragon - versatile solution for entertainment studios and individuals. Designed to empower small studios and one-man teams, boost their productivity to new levels with high quality results.

*NOTE:
As of January 2025 the codebase is still udergoing first-phase sketching and maintenance, code cleanup - that was part of our initial approach, as weird as it may seem at first.
So be aware that you won't succeed if you try to compile the engine now as it is.

![Hydragon promo image](./Engine/Docs/UserGuide/images/Hydragon_promo_01.png)

HydragonEngine is an open-source game engine licensed under the **Non-Commercial Use License** for non-commercial use. For commercial use, separate licenses are required depending on gross revenues.

## Purpose

HydragonEngine is designed to provide a powerful and flexible game engine for developers. It is built with a focus on modularity, extensibility, and ease of use, allowing developers to create high-quality games and applications with minimal effort.

The current effort is a **Filament-first rewrite of the engine core**, on branch `rewrite/filament-core`. The legacy core is preserved in git history, on branch `alpha-feature-vulkan-backend`, and copied under `../EngineLegacy/` (git-ignored) for local reference.

## Architecture & Philosophy

Carried over from the design docs:

- **Bare-mode core**: headless, no windowing/GUI dependency in the core library.
- **USD as authoritative backend**: scene graph, state and hierarchy live in OpenUSD.
- **Filament as renderer**: direct USD -> Filament translation (Hydra delegate optional, later).
- **Agnostic nodes**: every entity is a scene graph node (USD composition arcs / Filament `TransformManager`).

## Repository Layout & Build Pipeline

The folder structure enforces a strict **separation of concerns** so that adding a new
third-party library (Filament, GLFW, ...) never requires a per-library decision:

| Folder | Contents | Nature | Versioned? |
| --- | --- | --- | --- |
| `Engine/Source/` | **Only our own source code** and versioned build-definition files (`.vcxproj`, `.props`, license texts, header-only glue under `Source/ThirdParty/`). | Light, lean | Yes |
| `Engine/ThirdParty/<lib>/` | Each **external dependency**, self-contained (its own `include/` + `lib/` + `bin/` + source clone if built from source). Fetched once by `setup.ps1`. | Heavy, reproducible | No (git-ignored) |
| `Engine/Bin/<platform>/<config>/` | **Build artifacts** — our binaries **plus** any third-party runtime DLLs copied here by a post-build step. | Disposable, regenerable | No (git-ignored) |
| `Engine/BuildOutput/Intermediate/` | Intermediate object files (`.obj`) per project/config. | Disposable | No (git-ignored) |

Key rules that make this automatic:

- **`Source/` is source-only.** MSBuild's default of dumping `x64/`, `.obj`, `.exe`, `.pdb`
  next to each `.vcxproj` is overridden centrally: `OutDir` -> `Engine/Bin/...` and
  `IntDir` -> `Engine/BuildOutput/Intermediate/...`, set once in `Directory.Build.props`.
- **Dependency include/lib paths are centralized** in `Directory.Build.props` — you should
  not need to hand-edit any `.vcxproj`.
- **Static vs. dynamic third-party.** A static `.lib` (e.g. Filament, 100% static) is consumed
  at build time and embedded into the `.exe`, so it contributes nothing to distribution. A
  runtime `.dll` (e.g. FBX SDK, dynamic GLFW) is copied from `Engine/ThirdParty/<lib>/` into
  `Bin/` by a post-build step — so shipping still means "grab the `Bin/` folder", while the
  source of truth for the dependency stays in `Engine/ThirdParty/`.
- **Project files** (`.vcxproj`) live next to the module they build (e.g.
  `Engine/Source/Runtime/Runtime.vcxproj`); the solution `Hydragon.sln` at the repo root
  references them.

## Toolchain

- Visual Studio 2022 (MSBuild, toolset v143, **C++20**).
- **vcpkg** (manifest mode) for `imgui`, `glfw3`, `glm`, `spdlog`, and later `usd`.
- **Filament** consumed as a prebuilt SDK under `Engine/ThirdParty/Filament/` (not on vcpkg).

> Google Filament builds its own materials/shaders with CMake upstream, but here it is
> consumed **prebuilt**, so the day-to-day loop stays 100% in Visual Studio. If you ever
> prefer CMake + `CMakePresets.json` (VS opens those natively too, and it is how Filament/USD
> build from source), that path remains available.

## Building

### First-time setup

```powershell
# From a Developer PowerShell at the repo root
./scripts/setup.ps1
```

This bootstraps vcpkg (`vcpkg integrate install`) and downloads the prebuilt Filament SDK
into `Engine/ThirdParty/Filament/`. Then open `Hydragon.sln` in Visual Studio 2022 and build.

Options:

```powershell
./scripts/setup.ps1 -SkipFilament            # only vcpkg
./scripts/setup.ps1 -FilamentVersion v1.72.1 # pin a Filament version
```

### Enabling Filament

After `setup.ps1` has populated `Engine/ThirdParty/Filament/`, set the project property
`UseFilament=true` (in `Directory.Build.props` or via `/p:UseFilament=true`), which adds the
Filament include path, links its static libs and defines `HYDRAGON_USE_FILAMENT`.

## Development Phases

| Phase | Goal | Switch |
| --- | --- | --- |
| 0 | Toolchain smoke test (`Runtime` prints and runs) | default |
| 1 | Filament hello-cube (window + PBR cube) | `<UseFilament>true</UseFilament>` |
| 2 | ImGui overlay editor (decoupled from core) | vcpkg `imgui` |
| 3 | OpenUSD backend (traverse prims -> Filament entities) | add `usd` to `vcpkg.json` |
| 4 | WavePhysics (XPBD + fields) | — |

## Documentation

For detailed documentation on HydragonEngine, please refer to the [User Guide](./Engine/Docs/UserGuide.md).

## Contributing

We welcome contributions to HydragonEngine! Please refer to the [Contributing Guide](./CONTRIBUTING.md) for more information on how to contribute.

## Licensing

- **Non-Commercial Use**: This software is free to use, modify, and distribute under the terms of the [Non-Commercial Use License](./NON_COMMERCIAL_LICENSE.md).
- **Commercial Indie License**: Required for commercial use with gross revenues up to $15,000 USD. See the [Commercial Indie License](./COMMERCIAL_INDIE_LICENSE.md) for details.
- **Commercial Enterprise License**: Required for commercial use with gross revenues over $15,000 USD. See the [Commercial Enterprise License](./COMMERCIAL_ENTERPRISE_LICENSE.md) for details.

For licensing inquiries or further information, please contact **Agua Games** at **athos.sampaio@aguagames.com**.

## Third-Party Libraries

This software uses the following third-party libraries:

- **glfw**: [zlib License](./Engine/Source/ThirdParty/Licenses/glfw_LICENSE.md).
- **fmt**:  [fmt License](./Engine/Source/ThirdParty/Licenses/fmt_LICENSE.txt).
- **imgui**: [MIT License](./Engine/Source/ThirdParty/Licenses/imgui_LICENSE.txt).
- **IconFontCppHeaders**:  [IconFontCppHeaders License](./Engine/Source/ThirdParty/Licenses/IconFontCppHeaders_LICENSE.txt).
- **Font-Awesome**: [GPL License](./Engine/Source/ThirdParty/Licenses/Font-Awesome_LICENSE.txt).
- **material-design-icons**: [Apache 2.0 License](./Engine/Source/ThirdParty/Licenses/material-design-icons_LICENSE.txt).
- **imgui-node-editor**: [MIT License](./Engine/Source/ThirdParty/Licenses/imgui-node-editor_LICENSE.txt).
- **vulkan-headers**: [Apache 2.0 License](./Engine/Source/ThirdParty/Licenses/vulkan-headers_LICENSE.txt).
- **volk**: [volk License](./Engine/Source/ThirdParty/Licenses/volk_LICENSE.txt).
- **VulkanMemoryAllocator**: [VulkanMemoryAllocator License](./Engine/Source/ThirdParty/Licenses/VulkanMemoryAllocator_LICENSE.txt).

For more information about the licensing terms of these libraries, please refer to their respective license files in the `third_party` directory.

## Terms and Conditions

Please review the [Terms and Conditions](./Software-Terms-Conditions.md) for acceptable and prohibited uses of this software.