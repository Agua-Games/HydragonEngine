/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Node graph execution and preview system
 */

#pragma once
#include "NodeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct NodeExecutionConfig {
    bool enableLiveExecution = true;
    bool enableDebugVisualization = true;
    bool enableProfiling = true;
    bool enableCaching = true;
    uint32_t maxExecutionDepth = 100;
    uint32_t cacheSize = 1000;
    float executionTimeout = 5000.0f;  // ms
};

class NodeExecutionSystem {
public:
    static NodeExecutionSystem& Get();
    
    void Initialize(const NodeExecutionConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void ExecuteNode(NodeHandle handle);
    void ExecuteGraph(const std::vector<NodeHandle>& outputNodes);
    void StopExecution();
    
    void SetNodeInput(NodeHandle handle, uint32_t pin, const NodeValue& value);
    NodeValue GetNodeOutput(NodeHandle handle, uint32_t pin) const;
    
    void RegisterExecutor(const std::string& nodeType, 
                         std::unique_ptr<INodeExecutor> executor);
    
    bool IsExecuting() const;
    float GetExecutionProgress() const;
    
    const NodeExecutionStats& GetStats() const { return m_Stats; }

private:
    NodeExecutionSystem() = default;
    
    NodeExecutionConfig m_Config;
    NodeExecutionStats m_Stats;
    std::unordered_map<NodeHandle, NodeState> m_NodeStates;
    std::unique_ptr<IExecutionEngine> m_Engine;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 