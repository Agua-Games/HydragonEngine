/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Workshop.h
 * @brief Header file for the Workshop class.
 * 
 * ARCHITECTURAL NOTES:
 * - Workshop is a class that represents a workshop in Hydragon.
 * - It is used to represent any workshop in the game world, such as blacksmiths, alchemist workshops, witch huts, etc.
 * - It supports interactive features, such as crafting, and producing results. Also supports two-way messaging with other workshops, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "Item.h"
#include "CraftingResult.h"

namespace hd {

struct WorkshopInfo : public NodeInfo {
    WorkshopInfo() {
        nodeType = "Gameplay/Workshop";
        
        inputs = {
            "workshopType",  // Type of workshop (blacksmith, alchemist, etc.)
            "workshopData",  // Workshop data
            "environment",   // Environment data
            "characterData", // Character data
            "workshopState"  // Workshop state
        };
        
        outputs = {
            "workshopStatus",  // Workshop status
            "workshopMetrics"  // Workshop performance metrics
        };
    }
};

class Workshop : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Workshop(const WorkshopInfo& info = WorkshopInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processWorkshop();
    void processNode() override {
        processWorkshop(); 
    }
    void craft();
    void produce();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Workshop() = default;     // Default destructor
};

} // namespace hd
