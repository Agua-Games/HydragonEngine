/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Graphics pipeline management system
 */

#pragma once
#include "PipelineTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Graphics {

class PipelineSystem {
public:
    struct PipelineConfig {
        uint32_t maxPipelines = 1000;
        uint32_t maxShaders = 5000;
        bool enableShaderHotReload = true;
        bool enablePipelineCache = true;
        bool enableValidation = true;
        size_t pipelineCacheSize = 256 * 1024 * 1024;  // 256MB
    };

    static PipelineSystem& Get();
    
    void Initialize(const PipelineConfig& config = {});
    void Shutdown();
    
    PipelineHandle CreatePipeline(const PipelineDesc& desc);
    void DestroyPipeline(PipelineHandle handle);
    
    ShaderHandle CreateShader(const ShaderDesc& desc);
    void DestroyShader(ShaderHandle handle);
    
    void BindPipeline(PipelineHandle handle);
    void SetPipelineState(PipelineHandle handle, const PipelineState& state);
    
    const PipelineStats& GetStats() const { return m_Stats; }

private:
    PipelineSystem() = default;
    
    PipelineConfig m_Config;
    PipelineStats m_Stats;
    std::unique_ptr<IPipelineManager> m_Manager;
    std::vector<Pipeline> m_Pipelines;
    bool m_Initialized = false;
};

} // namespace Hydragon::Graphics 