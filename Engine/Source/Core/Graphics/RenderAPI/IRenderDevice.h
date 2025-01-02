/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Render device interface
 */

#pragma once
#include "../RenderTypes.h"
#include "CommandBuffer.h"
#include <memory>

namespace Hydragon::Graphics {

class IRenderDevice {
public:
    virtual ~IRenderDevice() = default;

    virtual bool Initialize() = 0;
    virtual void Shutdown() = 0;

    virtual CommandBuffer* CreateCommandBuffer() = 0;
    virtual void SubmitCommandBuffer(CommandBuffer* cmdBuffer) = 0;
    
    virtual BufferHandle CreateBuffer(const BufferDesc& desc) = 0;
    virtual void DestroyBuffer(BufferHandle handle) = 0;
    
    virtual ShaderHandle CreateShader(const ShaderDesc& desc) = 0;
    virtual void DestroyShader(ShaderHandle handle) = 0;

    virtual void WaitIdle() = 0;
    virtual bool IsInitialized() const = 0;
};

} // namespace Hydragon::Graphics 