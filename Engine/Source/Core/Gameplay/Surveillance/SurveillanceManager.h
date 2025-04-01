/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SurveillanceManager.h
 * @brief Header file for the SurveillanceManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - SurveillanceManager is a singleton class that manages in-game surveillance and security systems, like security networks, surveillance satellites arrays, etc.
 * - It is responsible for managing surveillance and security in the game.
 * - It supports interactive features, such as detecting and responding to security threats. Also supports two-way messaging with other surveillance and security systems,
 * environment, character, etc.
 */
#pragma once
#include "Core/NodeGraph/Node.h"
#include "DataTable.h"

namespace hd {

struct SurveillanceManagerInfo : public NodeInfo {
    SurveillanceManagerInfo() {
        nodeType = "Gameplay/SurveillanceManager";
        
        inputs = {
            "surveillanceData",      // Surveillance data
            "environment",       // Environment data
            "characterData",     // Character data
            "surveillanceState"      // Surveillance state
        };
        
        outputs = {
            "surveillanceStatus",    // Surveillance status
            "surveillanceMetrics"    // Surveillance performance metrics
        };
    }
};

class SurveillanceManager : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit SurveillanceManager(const SurveillanceManagerInfo& info = SurveillanceManagerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // === Processing ===
    void processSurveillanceManager();
    void processNode() override {
        processSurveillanceManager(); 
    }
    void monitor();
    void scanEnvironment();
    void scanCharacters();
    void scanNetwork();
    void analyzeData();
    void synchronizeSensors();
    void patrol();
    void detectThreats();
    void processThreats();
    void processResponses();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~SurveillanceManager() = default;     // Default destructor
};

} // namespace hd
