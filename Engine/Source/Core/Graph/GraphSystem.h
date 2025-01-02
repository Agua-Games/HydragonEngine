/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core graph processing and traversal system
 */

#pragma once
#include "GraphTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Graph {

class GraphSystem {
public:
    struct GraphConfig {
        uint32_t maxGraphs = 1000;
        uint32_t maxNodesPerGraph = 10000;
        uint32_t maxEdgesPerNode = 100;
        bool enableParallelTraversal = true;
        bool enableCaching = true;
        bool enableOptimization = true;
        size_t memoryBudget = 128 * 1024 * 1024;  // 128MB
    };

    static GraphSystem& Get();
    
    void Initialize(const GraphConfig& config = {});
    void Shutdown();
    
    void Update();
    
    GraphHandle CreateGraph(const GraphDesc& desc);
    void DestroyGraph(GraphHandle handle);
    
    NodeHandle AddNode(GraphHandle graph, const NodeDesc& desc);
    void RemoveNode(GraphHandle graph, NodeHandle node);
    
    EdgeHandle AddEdge(GraphHandle graph, NodeHandle from, NodeHandle to, const EdgeDesc& desc);
    void RemoveEdge(GraphHandle graph, EdgeHandle edge);
    
    void TraverseGraph(GraphHandle graph, const TraversalParams& params);
    void OptimizeGraph(GraphHandle graph, const OptimizationParams& params);
    
    const GraphStats& GetStats() const { return m_Stats; }

private:
    GraphSystem() = default;
    
    GraphConfig m_Config;
    GraphStats m_Stats;
    std::unique_ptr<IGraphProcessor> m_Processor;
    std::vector<Graph> m_Graphs;
    bool m_Initialized = false;
};

} // namespace Hydragon::Graph 