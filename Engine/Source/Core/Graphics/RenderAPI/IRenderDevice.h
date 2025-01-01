/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Abstract render device interface
 */

#pragma once

namespace Hydragon {
namespace Graphics {

enum class RenderAPI {
    DirectX12,
    Vulkan,
    None
};

class IRenderDevice {
public:
    virtual bool supportsMeshShaders() const = 0;
    virtual bool supportsTaskShaders() const = 0;
    virtual bool supportsRayTracing() const = 0;
    
    // API-agnostic device capabilities
    struct DeviceCapabilities {
        bool meshShading;
        bool taskShading;
        bool rayTracing;
        uint32_t maxMeshWorkGroupSize;
        uint32_t maxTaskWorkGroupSize;
    };

    virtual DeviceCapabilities getCapabilities() const = 0;
    virtual ~IRenderDevice() = default;
};

}} // namespace Hydragon::Graphics 