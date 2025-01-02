/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Visual node graph system
 */

#pragma once
#include "NodeTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Nodes {

class NodeSystem {
public:
    struct NodeConfig {
        uint32_t maxGraphs = 100;
        uint32_t maxNodesPerGraph = 1000;
        bool enableHotReload = true;
        bool enableParallelExecution = true;
        bool enableProfiling = true;
        size_t memoryBudget = 32 * 1024 * 1024;  // 32MB
    };

    static NodeSystem& Get();
    
    void Initialize(const NodeConfig& config = {});
    void Shutdown();
    
    void Update();
    
    GraphHandle CreateGraph(const GraphDesc& desc);
    void DestroyGraph(GraphHandle handle);
    
    NodeHandle AddNode(GraphHandle graph, const NodeDesc& desc);
    void RemoveNode(GraphHandle graph, NodeHandle node);
    
    void Connect(GraphHandle graph, NodeHandle output, NodeHandle input, 
                const std::string& pinName);
    void Disconnect(GraphHandle graph, NodeHandle output, NodeHandle input);
    
    void Execute(GraphHandle graph);
    
    const NodeStats& GetStats() const { return m_Stats; }

private:
    NodeSystem() = default;
    
    NodeConfig m_Config;
    NodeStats m_Stats;
    std::vector<Graph> m_Graphs;
    std::unique_ptr<INodeExecutor> m_Executor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Nodes 