/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Gate.h
 * @brief Header file for the Gate class.
 * 
 * ARCHITECTURAL NOTES:
 * - Gate is a class that represents a gate in Hydragon.
 * - It is used to represent any gate in the game world, such as doors, windows, portals, skilights, city gates, castle gates, etc.
 * - It supports interactive features, such as opening, closing, and locking. Also supports two-way messaging with other gates, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct GateInfo : public NodeInfo {
    GateInfo() {
        nodeType = "Gameplay/Gate";
        
        inputs = {
            "gateType",        // Type of gate (door, city gate, castle gate, etc.)
            "gateData",        // Gate data
            "environment",     // Environment data
            "characterData",   // Character data
            "gateState"        // Gate state
        };
        
        outputs = {
            "gateStatus",      // Gate status
            "gateMetrics"      // Gate performance metrics
        };
    }
};

class Gate : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Gate(const GateInfo& info = GateInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Gate() = default;     // Default destructor
};

} // namespace hd
