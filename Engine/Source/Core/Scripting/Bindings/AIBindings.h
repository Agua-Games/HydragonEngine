/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * AI system bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include "Core/AI/AISystem.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct AIBindingsConfig {
    bool enableBehaviorTrees = true;
    bool enablePathfinding = true;
    bool enablePerception = true;
    bool enableDecisionMaking = true;
    uint32_t maxAgents = 1000;
    uint32_t maxBehaviorTrees = 100;
    uint32_t maxPathRequests = 50;
    std::string aiDataPath = "Data/AI";
};

class AIBindings {
public:
    static AIBindings& Get();
    
    void Initialize(const AIBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    // Agent management
    AgentHandle CreateAgent(const AgentDesc& desc);
    void DestroyAgent(AgentHandle handle);
    void SetAgentPosition(AgentHandle handle, const Vector3& position);
    void SetAgentRotation(AgentHandle handle, const Quaternion& rotation);
    
    // Behavior tree management
    BehaviorTreeHandle CreateBehaviorTree(const std::string& name, const BehaviorTreeDesc& desc);
    void DestroyBehaviorTree(BehaviorTreeHandle handle);
    void AssignBehaviorTree(AgentHandle agent, BehaviorTreeHandle tree);
    void SetBehaviorTreeVariable(BehaviorTreeHandle handle, const std::string& name, const Variant& value);
    
    // Pathfinding
    PathRequestHandle RequestPath(const PathRequest& request);
    void CancelPathRequest(PathRequestHandle handle);
    bool IsPathReady(PathRequestHandle handle) const;
    std::vector<Vector3> GetPath(PathRequestHandle handle) const;
    
    // Perception system
    void RegisterStimulus(const Stimulus& stimulus);
    void UpdateStimulusLocation(StimulusHandle handle, const Vector3& location);
    void RemoveStimulus(StimulusHandle handle);
    std::vector<PerceptionEvent> GetPerceptionEvents(AgentHandle handle) const;
    
    // Decision making
    void SetDecisionMaker(AgentHandle handle, const DecisionMakerDesc& desc);
    void UpdateWorldState(AgentHandle handle, const WorldState& state);
    void AddGoal(AgentHandle handle, const Goal& goal);
    void RemoveGoal(AgentHandle handle, GoalHandle goalHandle);
    
    // Debug utilities
    void EnableDebugVisualization(bool enable);
    void SetDebugMode(AIDebugMode mode);
    void DumpAgentState(AgentHandle handle, const std::string& path);
    
    const AIBindingsStats& GetStats() const { return m_Stats; }

private:
    AIBindings() = default;
    
    AIBindingsConfig m_Config;
    AIBindingsStats m_Stats;
    std::unordered_map<AgentHandle, AgentInstance> m_Agents;
    std::unordered_map<BehaviorTreeHandle, BehaviorTreeInstance> m_BehaviorTrees;
    std::unordered_map<PathRequestHandle, PathRequestInstance> m_PathRequests;
    std::unordered_map<StimulusHandle, StimulusInstance> m_Stimuli;
    std::unique_ptr<AI::IAgentProcessor> m_AgentProcessor;
    std::unique_ptr<AI::IBehaviorTreeRunner> m_BehaviorTreeRunner;
    std::unique_ptr<AI::IPathfinder> m_Pathfinder;
    std::unique_ptr<AI::IPerceptionSystem> m_PerceptionSystem;
    std::unique_ptr<AI::IDecisionMaker> m_DecisionMaker;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 