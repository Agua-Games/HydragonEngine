/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Render system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/Render/RenderSystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct RenderBindingsConfig {
    bool enableCustomPipelines = true;
    bool enablePostProcessing = true;
    bool enableRenderTargets = true;
    bool enableProfiling = true;
    uint32_t maxCustomPipelines = 50;
    uint32_t maxPostEffects = 20;
    uint32_t maxRenderTargets = 30;
    std::string pipelineCachePath = "Cache/Pipelines";
};

class RenderBindings {
public:
    static RenderBindings& Get();
    
    void Initialize(const RenderBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Pipeline management
    PipelineHandle CreatePipeline(const PipelineDesc& desc);
    void DestroyPipeline(PipelineHandle handle);
    void SetPipelineState(PipelineHandle handle, const PipelineState& state);
    
    // Render target management
    RenderTargetHandle CreateRenderTarget(const RenderTargetDesc& desc);
    void DestroyRenderTarget(RenderTargetHandle handle);
    void ResizeRenderTarget(RenderTargetHandle handle, uint32_t width, uint32_t height);
    
    // Post-processing
    void AddPostEffect(const std::string& name, const PostEffectDesc& desc);
    void RemovePostEffect(const std::string& name);
    void SetPostEffectEnabled(const std::string& name, bool enabled);
    
    // Render commands
    void SetViewport(const Viewport& viewport);
    void SetScissor(const Scissor& scissor);
    void BindPipeline(PipelineHandle handle);
    void BindRenderTarget(RenderTargetHandle handle);
    
    // Resource binding
    void BindVertexBuffers(const std::vector<BufferHandle>& buffers);
    void BindIndexBuffer(BufferHandle buffer);
    void BindDescriptorSets(const std::vector<DescriptorSetHandle>& sets);
    
    // Draw commands
    void Draw(uint32_t vertexCount, uint32_t instanceCount = 1);
    void DrawIndexed(uint32_t indexCount, uint32_t instanceCount = 1);
    void DrawIndirect(BufferHandle buffer, uint32_t offset);
    
    // Profiling and debug
    void BeginProfileScope(const std::string& name);
    void EndProfileScope();
    void SetDebugMarker(const std::string& name);
    
    const RenderBindingsStats& GetStats() const { return m_Stats; }

private:
    RenderBindings() = default;
    
    RenderBindingsConfig m_Config;
    RenderBindingsStats m_Stats;
    std::unordered_map<PipelineHandle, PipelineInstance> m_Pipelines;
    std::unordered_map<RenderTargetHandle, RenderTargetInstance> m_RenderTargets;
    std::unordered_map<std::string, PostEffectInstance> m_PostEffects;
    std::vector<ProfileScope> m_ProfileScopes;
    std::unique_ptr<Render::IPipelineCompiler> m_PipelineCompiler;
    std::unique_ptr<Render::IResourceManager> m_ResourceManager;
    std::unique_ptr<Render::IProfiler> m_Profiler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 