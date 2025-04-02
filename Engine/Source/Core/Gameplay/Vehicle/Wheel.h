/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Wheel.h
 * @brief Header file for the Wheel class.
 * 
 * ARCHITECTURAL NOTES:
 * - Wheel is a class that represents a wheel in Hydragon.
 * - It is used to represent any wheel in the game world, such as car wheels, motorcycle wheels, etc.
 * - It supports collision detection and response, through the use of solid physics, solid shapes (with fallback to traditional rigid body physics), solid materials,
 * suspension springs, and other physics-related features.
 * - It supports interactive features, such as spinning, braking, and steering. Also supports two-way messaging with other wheels, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include "Node.h"
#include "DataTable.h"
#include "Transform.h"
#include "PhysicsTypes.h"
#include "PhysicsMaterial.h"
#include "Solid.h"
#include "Pathfinder.h"

namespace hd {

struct WheelInfo : public NodeInfo {
    WheelInfo() {
        nodeType = "Gameplay/Vehicle/Wheel";
        
        inputs = {
            "radius",       // Radius of the wheel
            "solid"        // Solid of the wheel
        };
        
        outputs = {
            "solid"        // Solid of the wheel
        };
    }
};

class Wheel : public Solid {
public:
    // === Allocation, Initialization, Loading ===
    explicit Wheel(const WheelInfo& info = WheelInfo())
        : Solid(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    PhysicsMaterial material;
    float radius = 0.0f;
    float width = 0.0f;
    float traction = 0.0f;
    float suspensionRange = 0.0f;
    float suspensionStiffness = 0.0f;
    float suspensionDamping = 0.0f;
    float suspensionCompression = 0.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Wheel() = default;     // Default destructor
};

} // namespace hd
