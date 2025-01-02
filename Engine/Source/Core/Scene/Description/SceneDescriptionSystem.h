/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene description and serialization system
 */

#pragma once
#include "SceneDescriptionTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <string>
#include <vector>

namespace Hydragon::Scene {

class SceneDescriptionSystem {
public:
    struct DescriptionConfig {
        bool enableLazyEvaluation = true;
        bool enableCaching = true;
        bool enableCompression = true;
        bool enableValidation = true;
        uint32_t maxNodes = 1000000;
        uint32_t maxConnections = 2000000;
        size_t memoryBudget = 512 * 1024 * 1024;  // 512MB
    };

    static SceneDescriptionSystem& Get();
    
    void Initialize(const DescriptionConfig& config = {});
    void Shutdown();
    
    NodeHandle CreateNode(const NodeDesc& desc);
    void DestroyNode(NodeHandle handle);
    
    void Connect(NodeHandle from, NodeHandle to, const ConnectionDesc& desc);
    void Disconnect(NodeHandle from, NodeHandle to);
    
    void SetParameter(NodeHandle node, const char* name, const Parameter& value);
    Parameter GetParameter(NodeHandle node, const char* name) const;
    
    void Serialize(const std::string& path);
    void Deserialize(const std::string& path);
    
    void Validate();
    void Optimize();
    
    const DescriptionStats& GetStats() const { return m_Stats; }

private:
    SceneDescriptionSystem() = default;
    
    DescriptionConfig m_Config;
    DescriptionStats m_Stats;
    std::unique_ptr<IDescriptionProcessor> m_Processor;
    std::vector<Node> m_Nodes;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scene 