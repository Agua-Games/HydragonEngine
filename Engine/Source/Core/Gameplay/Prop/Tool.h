/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Tool.h
 * @brief Header file for the Tool class.
 * 
 * ARCHITECTURAL NOTES:
 * - Tool is a class that represents a tool in Hydragon.
 * - It is used to represent any tool in the game world, such as weapons, props, etc.
 * - It supports interactive features, such as using the tool, interacting with the environment, etc. Also supports two-way messaging with other tools, environment, character, etc.
 */
#pragma once
#include "Node.h"
#include "Wave.h"
#include "PhysicsFields.h"
#include "AudioFile.h"

namespace hd {

struct ToolInfo : public NodeInfo {
    ToolInfo() {
        nodeType = "Gameplay/Tool";
        
        inputs = {
            "toolType",        // Type of tool (weapon, prop, etc.)
            "toolData",        // Tool data
            "environment",     // Environment data
            "characterData",   // Character data
            "toolState"        // Tool state
        };
        
        outputs = {
            "toolStatus",      // Tool status
            "toolMetrics"      // Tool performance metrics
        };
    }
};

class Tool : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Tool(const ToolInfo& info = ToolInfo())
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
    ~Tool() = default;     // Default destructor
};

} // namespace hd
    