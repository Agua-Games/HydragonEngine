/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Pet.h
 * @brief Header file for the Pet class.
 * 
 * ARCHITECTURAL NOTES:
 * - Pet is a class that represents a pet in Hydragon.
 * - It is used to represent any pet in the game world, such as dogs, cats, birds, etc.
 * - It supports interactive features, such as barking, meowing, and flying. Also supports two-way messaging with other pets, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct PetInfo : public CharacterInfo {
    PetInfo() {
        nodeType = "Gameplay/Pet";
        
        inputs = {
            "petType",        // Type of pet (dog, cat, bird, etc.)
            "petData",        // Pet data
            "environment",    // Environment data
            "characterData",  // Character data
            "petState"        // Pet state
        };
        
        outputs = {
            "petStatus",      // Pet status
            "petMetrics"      // Pet performance metrics
        };
    }
};

class Pet : public Character {
public:
    // === Allocation, Initialization, Loading ===
    explicit Pet(const PetInfo& info = PetInfo())
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
    ~Pet() = default;     // Default destructor
};

} // namespace hd
