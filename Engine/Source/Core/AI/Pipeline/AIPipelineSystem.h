/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * AI pipeline and integration system
 */

#pragma once
#include "Core/AI/AITypes.h"
#include "Core/Scene/SceneTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::AI {

struct AIIntegrationPoint {
    SceneHandle sceneHandle;
    BehaviorTreeHandle behaviorTree;
    GenerationContextHandle generationContext;
};

class AIPipelineSystem {
public:
    struct PipelineConfig {
        bool enableIntegration = true;
        bool enableProfiling = true;
        bool enableValidation = true;
        uint32_t maxIntegrationPoints = 1000;
        size_t memoryBudget = 128 * 1024 * 1024;  // 128MB
    };

    static AIPipelineSystem& Get();
    
    void Initialize(const PipelineConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Integration point management
    IntegrationHandle CreateIntegrationPoint(const AIIntegrationPoint& point);
    void DestroyIntegrationPoint(IntegrationHandle handle);
    void UpdateIntegrationPoint(IntegrationHandle handle, const AIIntegrationPoint& point);
    
    const PipelineStats& GetStats() const { return m_Stats; }

private:
    AIPipelineSystem() = default;
    
    PipelineConfig m_Config;
    PipelineStats m_Stats;
    std::vector<AIIntegrationPoint> m_IntegrationPoints;
    std::unique_ptr<IPipelineProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::AI 