/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Graph algorithm implementations
 */

#pragma once
#include "../GraphTypes.h"
#include <memory>
#include <vector>

namespace Hydragon::Graph {

class GraphAlgorithms {
public:
    struct AlgorithmConfig {
        bool enableParallel = true;
        bool enableCaching = true;
        uint32_t maxIterations = 1000;
        float convergenceThreshold = 0.001f;
    };

    static GraphAlgorithms& Get();
    
    void Initialize(const AlgorithmConfig& config = {});
    void Shutdown();
    
    // Path finding
    std::vector<NodeHandle> FindShortestPath(GraphHandle graph, NodeHandle start, NodeHandle end);
    std::vector<NodeHandle> FindMinimumSpanningTree(GraphHandle graph);
    
    // Analysis
    bool HasCycle(GraphHandle graph);
    uint32_t GetConnectedComponents(GraphHandle graph);
    float ComputeGraphDensity(GraphHandle graph);
    
    // Optimization
    void OptimizeLayout(GraphHandle graph, const LayoutParams& params);
    void ReduceEdges(GraphHandle graph, float threshold);
    
    const AlgorithmStats& GetStats() const { return m_Stats; }

private:
    GraphAlgorithms() = default;
    
    AlgorithmConfig m_Config;
    AlgorithmStats m_Stats;
    std::unique_ptr<IGraphAlgorithm> m_Algorithm;
    bool m_Initialized = false;
};

} // namespace Hydragon::Graph 