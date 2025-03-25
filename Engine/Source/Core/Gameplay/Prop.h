/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Prop.h
 * @brief Header file for the Prop class.
 * 
 * ARCHITECTURAL NOTES:
 * - Prop is a class that represents a prop in Hydragon.
 * - It is used to represent any object in the game world that is not a character or an environment.
 * - It supports interactive features, such as picking up, dropping, and using. Also supports two-way messaging with other props, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct PropInfo : public NodeInfo {
    PropInfo() {
        NodeType = "Gameplay/Prop";
        
        inputs = {
            "Model",        // Model of the prop
            "Position",     // Position of the prop
            "Rotation",     // Rotation of the prop
            "Scale",        // Scale of the prop
            "Material",     // Material of the prop
            "Animation",    // Animation of the prop
            "Physics",      // Physics of the prop
            "Collision",    // Collision of the prop
            "Script"        // Script of the prop
        };
        
        outputs = {
            "Model",        // Model of the prop
            "Position",     // Position of the prop
            "Rotation",     // Rotation of the prop
            "Scale",        // Scale of the prop
            "Material",     // Material of the prop
            "Animation",    // Animation of the prop
            "Physics",      // Physics of the prop
            "Collision",    // Collision of the prop
            "Script"        // Script of the prop
        };
    }
};

class Prop : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Prop(const NodeInfo& info = NodeInfo()) : Node(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // === Processing ===
    void update() override {}
    void processProp();
    void processNodeGraph() override {
        processProp(); 
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Prop() = default;     // Default destructor
};

} // namespace hd
