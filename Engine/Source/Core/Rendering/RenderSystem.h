/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core rendering system
 */

#pragma once
#include "RenderTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::Rendering {

class RenderSystem {
public:
    struct RenderConfig {
        bool enableMultithreading = true;
        bool enableRayTracing = false;
        bool enableVRS = true;
        bool enableMeshShaders = true;
        bool enableAsyncCompute = true;
        uint32_t maxDrawCalls = 100000;
        uint32_t maxMaterials = 10000;
        uint32_t maxTextures = 5000;
        size_t gpuMemoryBudget = 4 * 1024 * 1024 * 1024ull;  // 4GB
    };

    static RenderSystem& Get();
    
    void Initialize(const RenderConfig& config = {});
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void ExecuteRenderGraph(const RenderGraph& graph);
    void SubmitCommandBuffer(const CommandBuffer& buffer);
    
    ResourceHandle CreateResource(const ResourceDesc& desc);
    void DestroyResource(ResourceHandle handle);
    
    PipelineHandle CreatePipeline(const PipelineDesc& desc);
    void DestroyPipeline(PipelineHandle handle);
    
    void UpdateBuffer(BufferHandle handle, const void* data, size_t size);
    void UpdateTexture(TextureHandle handle, const void* data, const TextureRegion& region);
    
    void SetViewport(const Viewport& viewport);
    void SetScissor(const Scissor& scissor);
    
    void BeginRenderPass(const RenderPassDesc& desc);
    void EndRenderPass();
    
    const RenderStats& GetStats() const { return m_Stats; }

private:
    RenderSystem() = default;
    
    RenderConfig m_Config;
    RenderStats m_Stats;
    std::unique_ptr<IRenderDevice> m_Device;
    std::unique_ptr<ICommandQueue> m_Queue;
    std::vector<CommandBuffer> m_PendingCommands;
    bool m_Initialized = false;
};

} // namespace Hydragon::Rendering 