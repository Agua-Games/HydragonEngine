/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Node graph layout and organization system
 */

#pragma once
#include "NodeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct NodeLayoutConfig {
    bool enableAutoArrange = true;
    bool enableMinimization = true;
    bool enableAlignment = true;
    bool enableSpacing = true;
    float minNodeDistance = 50.0f;
    float verticalSpacing = 100.0f;
    float horizontalSpacing = 150.0f;
};

class NodeLayoutSystem {
public:
    static NodeLayoutSystem& Get();
    
    void Initialize(const NodeLayoutConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void ArrangeNodes(const std::vector<NodeHandle>& nodes);
    void AlignNodes(const std::vector<NodeHandle>& nodes, AlignmentMode mode);
    void DistributeNodes(const std::vector<NodeHandle>& nodes, DistributionMode mode);
    
    void MinimizeGroup(GroupHandle handle);
    void ExpandGroup(GroupHandle handle);
    
    void SetLayoutStrategy(std::unique_ptr<ILayoutStrategy> strategy);
    void ApplyLayout(const std::string& layoutName);
    
    const NodeLayoutStats& GetStats() const { return m_Stats; }

private:
    NodeLayoutSystem() = default;
    
    NodeLayoutConfig m_Config;
    NodeLayoutStats m_Stats;
    std::unique_ptr<ILayoutManager> m_Manager;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 