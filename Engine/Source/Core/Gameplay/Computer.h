/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Computer.h
 * @brief Header file for the Computer class.
 * 
 * ARCHITECTURAL NOTES:
 * - Computer is a class that represents a computer in Hydragon.
 * - It is used to represent any computer in the game world, such as desktops, laptops, etc.
 * - It supports interactive features, such as typing, clicking, and dragging. Also supports two-way messaging with other computers, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "Character.h"
#include "DataTable.h"

namespace hd {

struct ComputerInfo : public NodeInfo {
    ComputerInfo() {
        nodeType = "Gameplay/Computer";
        
        inputs = {
            "computerType",     // Type of computer (desktop, laptop, etc.)
            "computerData",     // Computer data
            "environment",      // Environment data
            "characterData",    // Character data
            "computerState"     // Computer state
        };
        
        outputs = {
            "computerStatus",   // Computer status
            "computerMetrics"   // Computer performance metrics
        };
    }
};

class Computer : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Computer(const ComputerInfo& info = ComputerInfo())
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
    ~Computer() = default;     // Default destructor
} // namespace hd
