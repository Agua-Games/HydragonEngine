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

namespace Hydragon::Graphics {

class RenderSystem {
public:
    struct RenderConfig {
        uint32_t maxDrawCalls = 100000;
        uint32_t maxMaterials = 10000;
        uint32_t maxTextures = 5000;
        bool enableInstancing = true;
        bool enableMultithreading = true;
        bool enableGPUCulling = true;
        size_t gpuMemoryBudget = 4 * 1024 * 1024 * 1024ull;  // 4GB
    };

    static RenderSystem& Get();
    
    void Initialize(const RenderConfig& config = {});
    void Shutdown();
    
    void BeginFrame();
    void EndFrame();
    
    void Submit(const RenderCommand& command);
    void SetViewport(const Viewport& viewport);
    void SetRenderTarget(RenderTargetHandle target);
    
    MaterialHandle CreateMaterial(const MaterialDesc& desc);
    void DestroyMaterial(MaterialHandle handle);
    
    TextureHandle CreateTexture(const TextureDesc& desc);
    void DestroyTexture(TextureHandle handle);
    
    void UpdateBuffer(BufferHandle handle, const void* data, size_t size);
    void SetShaderParameter(MaterialHandle material, const char* name, const void* data);
    
    const RenderStats& GetStats() const { return m_Stats; }

private:
    RenderSystem() = default;
    
    RenderConfig m_Config;
    RenderStats m_Stats;
    std::unique_ptr<IRenderDevice> m_Device;
    std::vector<RenderCommand> m_CommandBuffer;
    bool m_Initialized = false;
};

} // namespace Hydragon::Graphics 