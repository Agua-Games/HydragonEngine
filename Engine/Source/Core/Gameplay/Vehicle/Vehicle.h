/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Vehicle.h
 * @brief Header file for the Vehicle class.
 * 
 * ARCHITECTURAL NOTES:
 * - Vehicle is a class that represents a vehicle in Hydragon.
 * - It is used to represent any vehicle in the game world, such as cars, motorcycles, boats, planes, etc.
 * - It supports interactive features, such as driving, steering, and braking. Also supports two-way messaging with other vehicles, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"
#include "AdaptiveMesh.h"
#include "Transform.h"
#include "PhysicsTypes.h"
#include "PhysicsMaterial.h"
#include "Solid.h"
#include "Pathfinder.h"
#include "Wheel.h"
#include "AnimationStateMachine.h"

namespace hd {

struct VehicleInfo : public NodeInfo {
    VehicleInfo() {
        nodeType = "Gameplay/Vehicle";
        
        inputs = {
            "model",        // Model of the vehicle
            "position",     // Position of the vehicle
            "rotation",     // Rotation of the vehicle
            "scale",        // Scale of the vehicle
            "material",     // Material of the vehicle
            "animation",    // Animation of the vehicle
            "solid",      // Physics of the vehicle
        };
        
        outputs = {
            "model",        // Model of the vehicle
            "position",     // Position of the vehicle
            "rotation",     // Rotation of the vehicle
            "scale",        // Scale of the vehicle
            "material",     // Material of the vehicle
            "animation",    // Animation of the vehicle
            "solid",      // Physics of the vehicle
        };
    }
};

class Vehicle : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Vehicle(const NodeInfo& info = NodeInfo()) : Node(info) {}    // Call the base class constructor
    initialize() override {}
    load() override {}

    // Set default values
    AdaptiveMesh model;
    PositionTransform position;
    RotationTransform rotation;
    ScaleTransform scale;
    PhysicsMaterial material;
    AnimationStateMachine animation;
    Solid solid;
    Solid::mass mass;
    std::vector<Wheel> wheels;

    // === Processing ===
    void processNode() override {
        model = getInputValue<AdaptiveMesh>("model");
        position = getInputValue<PositionTransform>("position");
        rotation = getInputValue<RotationTransform>("rotation");
        scale = getInputValue<ScaleTransform>("scale");
        material = getInputValue<PhysicsMaterial>("material");
        animation = getInputValue<AnimationStateMachine>("animation");
        solid = getInputValue<Solid>("solid");

        // Process vehicle
        auto vehicleState = updateVehicle(model, position, rotation, scale, material, animation, solid);
        
        // Set outputs
        setOutputValue("model", vehicleState.model);
        setOutputValue("position", vehicleState.position);
        setOutputValue("rotation", vehicleState.rotation);
        setOutputValue("scale", vehicleState.scale);
        setOutputValue("material", vehicleState.material);
        setOutputValue("animation", vehicleState.animation);
        setOutputValue("solid", vehicleState.solid);
    }
    void update() override {}
    void processVehicle();
    void () override {
        processVehicle(); 
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Vehicle() = default;     // Default destructor
};

} // namespace hd
