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
  - Notes: At first included Vulkan-Hpp, but then switched to using the Vulkan SDK, including vulkan.h (from outside the codebase). Then switched to Volk, afterwards. So, probably remove it later if sticking with Volk.

- **Volk**: Vulkan meta-loader.
  - Version: 1.4.304
  - Repository: https://github.com/zeux/volk
  - Location: `Engine/Source/ThirdParty/volk`
  - Notes: Used for Vulkan loading and function resolution

## Notes
- All dependencies are managed as Git submodules.
- Submodules are pinned to specific versions to ensure stability.
