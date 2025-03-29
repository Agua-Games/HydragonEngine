/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SecurityManager.h
 * @brief Header file for the SecurityManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - SecurityManager is a singleton class that manages the security in the game.
 * - It is responsible for managing the security in the game.
 * - It supports interactive features, such as detecting and responding to security threats. Also supports two-way messaging with other security systems, environment, character, etc.
 */
#pragma once
#include "Core/NodeGraph/Node.h"

namespace hd {

struct SecurityManagerInfo : public NodeInfo {
    SecurityManagerInfo() {
        nodeType = "Gameplay/SecurityManager";
        
        inputs = {
            "securityData",      // Security data
            "environment",       // Environment data
            "characterData",     // Character data
            "securityState"      // Security state
        };
        
        outputs = {
            "securityStatus",    // Security status
            "securityMetrics"    // Security performance metrics
        };
    }
};

class SecurityManager : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit SecurityManager(const SecurityManagerInfo& info = SecurityManagerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processSecurityManager();
    void processNode() override {
        processSecurityManager(); 
    }
    void monitorSecurity();
    void scanSecurity();
    void patrolSecurity();
    void detectSecurityThreats();
    void processSecurityThreats();
    void processSecurityResponses();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SecurityManager() = default;     // Default destructor
};

} // namespace hd
