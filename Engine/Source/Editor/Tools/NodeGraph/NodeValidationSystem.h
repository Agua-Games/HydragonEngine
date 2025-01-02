/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Node graph validation and error checking system
 */

#pragma once
#include "NodeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <functional>

namespace Hydragon::Editor {

struct NodeValidationConfig {
    bool enableLiveValidation = true;
    bool enableTypeChecking = true;
    bool enableCycleDetection = true;
    bool enableConnectionLimits = true;
    uint32_t maxInputConnections = 8;
    uint32_t maxOutputConnections = 16;
    uint32_t validationInterval = 500;  // ms
};

class NodeValidationSystem {
public:
    static NodeValidationSystem& Get();
    
    void Initialize(const NodeValidationConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void ValidateNode(NodeHandle handle);
    void ValidateConnection(ConnectionHandle handle);
    void ValidateGraph();
    
    void AddValidationRule(const std::string& nodeType, const ValidationRule& rule);
    void RemoveValidationRule(const std::string& nodeType, const std::string& ruleName);
    
    ValidationResult GetNodeValidation(NodeHandle handle) const;
    ValidationResult GetConnectionValidation(ConnectionHandle handle) const;
    
    const NodeValidationStats& GetStats() const { return m_Stats; }

private:
    NodeValidationSystem() = default;
    
    NodeValidationConfig m_Config;
    NodeValidationStats m_Stats;
    std::unique_ptr<IValidationProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 