/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Boat.h
 * @brief Header file for the Boat class.
 * 
 * ARCHITECTURAL NOTES:
 * - Boat is a class that represents a boat in Hydragon.
 * - It is used to represent any boat in the game world, such as sailboats, motorboats, etc.
 * - It supports interactive features, such as sailing, steering, and docking. Also supports two-way messaging with other boats, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct BoatInfo : public NodeInfo {
    BoatInfo() {
        nodeType = "Gameplay/Boat";
        
        inputs = {
            "model",        // Model of the boat
            "position",     // Position of the boat
            "rotation",     // Rotation of the boat
            "scale",        // Scale of the boat
            "material",     // Material of the boat
            "animation",    // Animation of the boat
            "physics",      // Physics of the boat
            "collision",    // Collision of the boat
            "script"        // Script of the boat
        };
        
        outputs = {
            "model",        // Model of the boat
            "position",     // Position of the boat
            "rotation",     // Rotation of the boat
            "scale",        // Scale of the boat
            "material",     // Material of the boat
            "animation",    // Animation of the boat
            "physics",      // Physics of the boat
            "collision",    // Collision of the boat
            "script"        // Script of the boat
        };
    }
};

class Boat : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Boat(const BoatInfo& info = BoatInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override {
 
    }
    void update() override {}
    void processBoat();
    void () override {
        processBoat(); 
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Boat() = default;     // Default destructor
};

} // namespace hd


