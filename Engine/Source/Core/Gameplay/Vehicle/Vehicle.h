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
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

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
            "physics",      // Physics of the vehicle
            "collision",    // Collision of the vehicle
            "script"        // Script of the vehicle
        };
        
        outputs = {
            "model",        // Model of the vehicle
            "position",     // Position of the vehicle
            "rotation",     // Rotation of the vehicle
            "scale",        // Scale of the vehicle
            "material",     // Material of the vehicle
            "animation",    // Animation of the vehicle
            "physics",      // Physics of the vehicle
            "collision",    // Collision of the vehicle
            "script"        // Script of the vehicle
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
    Model model;
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    Material material;
    Animation animation;
    Physics physics;
    Collision collision;
    Script script;

    // === Processing ===
    void processNode() override {
        model = getInputValue<Model>("model");
        position = getInputValue<glm::vec3>("position");
        rotation = getInputValue<glm::vec3>("rotation");
        scale = getInputValue<glm::vec3>("scale");
        material = getInputValue<Material>("material");
        animation = getInputValue<Animation>("animation");
        physics = getInputValue<Physics>("physics");
        collision = getInputValue<Collision>("collision");
        script = getInputValue<Script>("script");

        // Process vehicle
        auto vehicleState = updateVehicle(model, position, rotation, scale, material, animation, physics, collision, script);
        
        // Set outputs
        setOutputValue("model", vehicleState.model);
        setOutputValue("position", vehicleState.position);
        setOutputValue("rotation", vehicleState.rotation);
        setOutputValue("scale", vehicleState.scale);
        setOutputValue("material", vehicleState.material);
        setOutputValue("animation", vehicleState.animation);
        setOutputValue("physics", vehicleState.physics);
        setOutputValue("collision", vehicleState.collision);
        setOutputValue("script", vehicleState.script);
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
