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
        nodeType = "Gameplay/Prop";
        
        inputs = {
            "model",        // Model of the prop
            "position",     // Position of the prop
            "rotation",     // Rotation of the prop
            "scale",        // Scale of the prop
            "material",     // Material of the prop
            "animation",    // Animation of the prop
            "physics",      // Physics of the prop
            "collision",    // Collision of the prop
            "script"        // Script of the prop
        };
        
        outputs = {
            "model",        // Model of the prop
            "position",     // Position of the prop
            "rotation",     // Rotation of the prop
            "scale",        // Scale of the prop
            "material",     // Material of the prop
            "animation",    // Animation of the prop
            "physics",      // Physics of the prop
            "collision",    // Collision of the prop
            "script"        // Script of the prop
        };
    }
};

class Prop : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Prop(const NodeInfo& info = NodeInfo()) : Node(info) {}    // Call the base class constructor
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

        // Process prop
        auto propState = updateProp(model, position, rotation, scale, material, animation, physics, collision, script);

        // Set outputs
        setOutputValue("model", propState.model);
        setOutputValue("position", propState.position);
        setOutputValue("rotation", propState.rotation);
        setOutputValue("scale", propState.scale);
        setOutputValue("material", propState.material);
        setOutputValue("animation", propState.animation);
        setOutputValue("physics", propState.physics);
        setOutputValue("collision", propState.collision);
        setOutputValue("script", propState.script);

    }
    void interact();
    void use();
    void drop();
    void pickUp();
    void update() override {}
    void processProp();
    void () override {
        processProp(); 
    }

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Prop() = default;     // Default destructor
};

} // namespace hd
