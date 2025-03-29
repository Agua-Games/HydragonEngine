/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CraftingManager.h
 * @brief Header file for the CraftingManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - CraftingManager is a singleton class that manages the crafting in the game.
 * - It is responsible for updating the crafting and managing the crafting system.
 * - It uses the Vulkan API for crafting management.
 */
#pragma once
#include "Core.h"
#include "Node.h"
#include "CraftingSystem.h"

namespace hd {

struct CraftingManagerInfo : public NodeInfo {
    CraftingManagerInfo() {
        NodeType = "Gameplay/CraftingManager";
        inputs = {
            "CraftingSystem" // Crafting system data
        };        
        outputs = {
            "CraftingStatus",  // Crafting status
            "CraftingMetrics"  // Crafting performance metrics
        };
    }
};

class CraftingManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit CraftingManager(const CraftingManagerInfo& info = CraftingManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addCraftingSystem(CraftingSystem* craftingSystem);
    void removeCraftingSystem(CraftingSystem* craftingSystem);
    void queryCraftingSystem(CraftingSystem* craftingSystem);
    void processCraftingSystem();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~CraftingManager() = default;     // Default destructor
};

} // namespace hd

