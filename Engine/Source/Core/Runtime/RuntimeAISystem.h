/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime AI and behavior management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeAIConfig {
    bool enableBehaviorTrees = true;
    bool enablePathfinding = true;
    bool enablePerception = true;
    bool enablePrediction = true;
    uint32_t maxAgents = 1000;
    uint32_t maxBehaviorTrees = 100;
    uint32_t perceptionUpdatesPerFrame = 50;
    float perceptionRange = 100.0f;
    std::string behaviorTreePath = "Config/AI/Behaviors";
};

class RuntimeAISystem {
public:
    static RuntimeAISystem& Get();
    
    void Initialize(const RuntimeAIConfig& config = {});
    void Shutdown();
    
    void Update();
    
    AIAgentHandle CreateAgent(const AIAgentDesc& desc);
    void DestroyAgent(AIAgentHandle handle);
    
    void AssignBehavior(AIAgentHandle handle, const std::string& behaviorId);
    void SetAgentTarget(AIAgentHandle handle, const Vector3& position);
    void SetAgentState(AIAgentHandle handle, const std::string& state);
    
    bool RequestPath(AIAgentHandle handle, const Vector3& destination);
    void CancelPathRequest(AIAgentHandle handle);
    void UpdatePathfinding(AIAgentHandle handle);
    
    void AddPerceptionStimulus(const Vector3& position, const StimulusDesc& desc);
    void UpdatePerception(AIAgentHandle handle, const PerceptionEvent& event);
    void SetPerceptionRange(AIAgentHandle handle, float range);
    
    void RegisterBehaviorNode(const std::string& type, std::unique_ptr<IAIBehaviorNode> node);
    void UnregisterBehaviorNode(const std::string& type);
    
    void RegisterPerceptionSensor(const std::string& type, std::unique_ptr<IPerceptionSensor> sensor);
    void UnregisterPerceptionSensor(const std::string& type);
    
    AIAgentState GetAgentState(AIAgentHandle handle) const;
    Vector3 GetAgentDestination(AIAgentHandle handle) const;
    float GetPathProgress(AIAgentHandle handle) const;
    std::vector<PerceptionHit> GetPerceptionResults(AIAgentHandle handle) const;
    
    const RuntimeAIStats& GetStats() const { return m_Stats; }

private:
    RuntimeAISystem() = default;
    
    RuntimeAIConfig m_Config;
    RuntimeAIStats m_Stats;
    std::unordered_map<AIAgentHandle, AIAgentInstance> m_Agents;
    std::unordered_map<std::string, std::unique_ptr<IAIBehaviorNode>> m_BehaviorNodes;
    std::unordered_map<std::string, std::unique_ptr<IPerceptionSensor>> m_Sensors;
    std::unique_ptr<IPathfinder> m_Pathfinder;
    std::unique_ptr<IBehaviorProcessor> m_BehaviorProcessor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 