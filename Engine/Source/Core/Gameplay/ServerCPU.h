/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ServerCPU.h
 * @brief Header file for the ServerCPU class.
 * 
 * ARCHITECTURAL NOTES:
 * - ServerCPU is a class that represents a server CPU in Hydragon.
 * - It is used to represent any server CPU in the game world, such as servers, etc.
 * - It supports interactive features, such as typing, clicking, and dragging. Also supports two-way messaging with other servers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct ServerCPUInfo : public NodeInfo {
    ServerCPUInfo() {
        nodeType = "Gameplay/ServerCPU";
        
        inputs = {
            "serverCPUType",    // Type of server CPU (server, etc.)
            "serverCPUData",    // Server CPU data
            "environment",      // Environment data
            "characterData",    // Character data
            "serverCPUState"    // Server CPU state
        };
        
        outputs = {
            "serverCPUStatus",  // Server CPU status
            "serverCPUMetrics"  // Server CPU performance metrics
        };
    }
};

class ServerCPU : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ServerCPU(const ServerCPUInfo& info = ServerCPUInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {    
    }    
    void type();
    void click();
    void drag();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ServerCPU() = default;     // Default destructor
};

} // namespace hd
