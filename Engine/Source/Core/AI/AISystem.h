/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core AI system
 */

#pragma once
#include "AITypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::AI {

class AISystem {
public:
    struct AIConfig {
        uint32_t maxAgents = 1000;
        uint32_t maxBehaviorTrees = 100;
        bool enablePathfinding = true;
        bool enablePerception = true;
        bool enablePrediction = true;
        size_t memoryBudget = 64 * 1024 * 1024;  // 64MB
    };

    static AISystem& Get();
    
    void Initialize(const AIConfig& config = {});
    void Shutdown();
    
    void Update(float deltaTime);
    
    AgentHandle CreateAgent(const AgentDesc& desc);
    void DestroyAgent(AgentHandle handle);
    
    void SetBehavior(AgentHandle agent, const std::string& behaviorPath);
    void SetTarget(AgentHandle agent, const Vector3& position);
    
    bool RequestPath(AgentHandle agent, const Vector3& destination);
    void UpdatePerception(AgentHandle agent, const PerceptionEvent& event);
    
    const AIStats& GetStats() const { return m_Stats; }

private:
    AISystem() = default;
    
    AIConfig m_Config;
    AIStats m_Stats;
    std::vector<Agent> m_Agents;
    std::unique_ptr<IPathfinder> m_Pathfinder;
    bool m_Initialized = false;
};

} // namespace Hydragon::AI 