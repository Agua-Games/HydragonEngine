# Hydragon Engine Dependencies

This document lists all third-party libraries used by Hydragon Engine and their versions.

## Core Dependencies
- **GLFW**: Window and input management.
  - Version: 3.3.8
  - Repository: https://github.com/glfw/glfw
  - Location: `Engine/Source/ThirdParty/glfw`

- **GLM**: Mathematics library.
  - Version: 0.9.9.8
  - Repository: https://github.com/g-truc/glm
  - Location: `Engine/Source/ThirdParty/glm`

- **imgui**: UI library.
  - Version: v1.91.7
  - Repository: https://github.com/ocornut/imgui
  - Location: `Engine/Source/ThirdParty/imgui`

- **Vulkan-Hpp**: C++ bindings for Vulkan.
  - Version: v1.4.305
  - Repository: https://github.com/KhronosGroup/Vulkan-Hpp
  - Location: `Engine/Source/ThirdParty/Vulkan-Hpp`

## Notes
- All dependencies are managed as Git submodules.
- Submodules are pinned to specific versions to ensure stability.