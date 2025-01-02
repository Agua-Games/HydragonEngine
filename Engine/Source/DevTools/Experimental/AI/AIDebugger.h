/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Experimental AI debugging and visualization tools
 */

#pragma once
#include "AITypes.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::DevTools::Experimental {

struct AIDebuggerConfig {
    bool enableBehaviorTreeViz = true;
    bool enableDecisionGraphs = true;
    bool enableStateInspection = true;
    bool enablePerformanceAnalysis = true;
    uint32_t maxTrackedEntities = 1000;
    float updateInterval = 0.1f;
};

class AIDebugger {
public:
    static AIDebugger& Get();
    
    void Initialize(const AIDebuggerConfig& config = {});
    void Shutdown();
    
    void Update(float deltaTime);
    
    void TrackEntity(EntityHandle entity);
    void UntrackEntity(EntityHandle entity);
    
    void VisualizeDecisionTree(EntityHandle entity);
    void VisualizeBehaviorTree(EntityHandle entity);
    void VisualizePathfinding(EntityHandle entity);
    
    void CaptureAISnapshot(const std::string& name);
    void CompareAISnapshots(const std::string& snapshot1, const std::string& snapshot2);
    
    const AIDebuggerStats& GetStats() const { return m_Stats; }

private:
    AIDebugger() = default;
    
    AIDebuggerConfig m_Config;
    AIDebuggerStats m_Stats;
    std::vector<TrackedEntity> m_TrackedEntities;
    std::unique_ptr<IAIAnalyzer> m_Analyzer;
    bool m_Initialized = false;
};

} // namespace Hydragon::DevTools::Experimental 