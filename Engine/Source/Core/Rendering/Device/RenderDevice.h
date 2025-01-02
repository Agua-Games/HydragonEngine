/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Abstract rendering device interface
 */

#pragma once
#include "../RenderTypes.h"
#include <memory>
#include <string>

namespace Hydragon::Rendering {

class IRenderDevice {
public:
    virtual ~IRenderDevice() = default;
    
    virtual void Initialize(const DeviceDesc& desc) = 0;
    virtual void Shutdown() = 0;
    
    virtual ResourceHandle CreateResource(const ResourceDesc& desc) = 0;
    virtual void DestroyResource(ResourceHandle handle) = 0;
    
    virtual PipelineHandle CreatePipeline(const PipelineDesc& desc) = 0;
    virtual void DestroyPipeline(PipelineHandle handle) = 0;
    
    virtual void UpdateBuffer(BufferHandle handle, const void* data, size_t size) = 0;
    virtual void UpdateTexture(TextureHandle handle, const void* data, const TextureRegion& region) = 0;
    
    virtual CommandBuffer CreateCommandBuffer() = 0;
    virtual void SubmitCommandBuffer(const CommandBuffer& buffer) = 0;
    
    virtual void WaitIdle() = 0;
    virtual const DeviceStats& GetStats() const = 0;
};

class VulkanDevice : public IRenderDevice {
public:
    void Initialize(const DeviceDesc& desc) override;
    void Shutdown() override;
    
    ResourceHandle CreateResource(const ResourceDesc& desc) override;
    void DestroyResource(ResourceHandle handle) override;
    
    PipelineHandle CreatePipeline(const PipelineDesc& desc) override;
    void DestroyPipeline(PipelineHandle handle) override;
    
    void UpdateBuffer(BufferHandle handle, const void* data, size_t size) override;
    void UpdateTexture(TextureHandle handle, const void* data, const TextureRegion& region) override;
    
    CommandBuffer CreateCommandBuffer() override;
    void SubmitCommandBuffer(const CommandBuffer& buffer) override;
    
    void WaitIdle() override;
    const DeviceStats& GetStats() const override;

private:
    VkDevice m_Device;
    VkPhysicalDevice m_PhysicalDevice;
    std::unique_ptr<VulkanMemoryAllocator> m_Allocator;
    DeviceStats m_Stats;
};

} // namespace Hydragon::Rendering 