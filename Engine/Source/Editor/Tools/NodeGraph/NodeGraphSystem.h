/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Visual node graph editing system
 */

#pragma once
#include "NodeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct NodeGraphConfig {
    bool enableLivePreview = true;
    bool enableNodeSnapping = true;
    bool enableAutoLayout = true;
    bool enableComments = true;
    uint32_t maxNodes = 1000;
    uint32_t maxConnections = 2000;
    float gridSize = 16.0f;
    Vector2 nodeSpacing = {200.0f, 150.0f};
};

class NodeGraphSystem {
public:
    static NodeGraphSystem& Get();
    
    void Initialize(const NodeGraphConfig& config = {});
    void Shutdown();
    
    void Update();
    
    NodeHandle CreateNode(const NodeDesc& desc);
    void DeleteNode(NodeHandle handle);
    void DuplicateNode(NodeHandle handle);
    
    ConnectionHandle ConnectNodes(NodeHandle output, uint32_t outputPin,
                                NodeHandle input, uint32_t inputPin);
    void DisconnectNodes(ConnectionHandle handle);
    
    void SetNodePosition(NodeHandle handle, const Vector2& position);
    void SetNodeComment(NodeHandle handle, const std::string& comment);
    
    void GroupNodes(const std::vector<NodeHandle>& nodes, const std::string& name);
    void UngroupNodes(GroupHandle handle);
    
    void RegisterNodeType(const NodeTypeInfo& info);
    void UnregisterNodeType(const std::string& type);
    
    const NodeGraphStats& GetStats() const { return m_Stats; }

private:
    NodeGraphSystem() = default;
    
    NodeGraphConfig m_Config;
    NodeGraphStats m_Stats;
    std::unordered_map<NodeHandle, Node> m_Nodes;
    std::unique_ptr<INodeProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 