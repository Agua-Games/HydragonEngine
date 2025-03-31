/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Transform.h
 * @brief Transform represents a transform node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Transform nodes are used to apply transformations to objects.
 * - Transformations can be extended and customized. e.g. to add deformations, custom dimensions, etc.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design, implementation.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once
#include <vector>
#include <memory>

#include "Node.h"

namespace hd {

// Base class for all transformation components
class Transformation {
public:
    virtual ~Transformation() = default;

    // Apply the transformation (e.g., update matrices)
    virtual void apply() = 0;

    // Draw this transformation in the Inspector
    virtual void drawInInspector() = 0;

    // Get the name of the transformation
    virtual std::string getName() const = 0;
};

// Default Position Transformation
class PositionTransform : public Transformation {
public:
    ImVec3 position = {0, 0, 0};
    ImVec3 velocity = {0, 0, 0};

    void apply() override {
        // Update position based on velocity
        position.x += velocity.x * deltaTime;
        position.y += velocity.y * deltaTime;
        position.z += velocity.z * deltaTime;
    }

    void drawInInspector() override {
        ImGui::DragFloat3("Position", &position.x);
        ImGui::DragFloat3("Velocity", &velocity.x);
    }

    std::string getName() const override {
        return "Position";
    }
};

// Default Orientation Transformation
class RotationTransform : public Transformation {
public:
    ImVec3 rotation = {0, 0, 0}; // Euler angles for simplicity
    ImVec3 angularVelocity = {0, 0, 0};

    void Apply() override {
        // Update orientation based on angular velocity
        rotation.x += angularVelocity.x * deltaTime;
        rotation.y += angularVelocity.y * deltaTime;
        rotation.z += angularVelocity.z * deltaTime;
    }

    void drawInInspector() override {
        ImGui::DragFloat3("Rotation", &rotation.x);
        ImGui::DragFloat3("Angular Velocity", &angularVelocity.x);
    }

    std::string getName() const override {
        return "Orientation";
    }
};

// Default Scale Transformation
class ScaleTransform : public Transformation {
public:
    ImVec3 scale = {1, 1, 1}; // Default scale is 1x

    void apply() override {
        // No dynamic scaling logic here, but you could add it if needed
    }

    void drawInInspector() override {
        ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.0f, 10.0f);
    }

    std::string getName() const override {
        return "Scale";
    }
};

// Transform: A node that supports extendable transformations
class Transform : public Node {
public:
    // Constructor with default transformations
    Transform(const NodeInfo& info)
        : Node(info) {
        addDefaultTransformations();
    }

    // Add a custom transformation
    void addTransformation(std::shared_ptr<Transformation> transformation) {
        Transformations.push_back(transformation);
    }

    // Remove a transformation by name
    void removeTransformation(const std::string& name) {
        Transformations.erase(std::remove_if(Transformations.begin(), Transformations.end(),
                                                [&name](const std::shared_ptr<Transformation>& t) {
                                                    return t->getName() == name;
                                                }),
                                Transformations.end());
    }

    // Update all transformations
    void update() override {
        for (auto& transformation : Transformations) {
            transformation->Apply();
        }
    }

    // Draw this node in the Inspector
    void drawInInspector() override {
        for (auto& transformation : Transformations) {
            transformation->drawInInspector();
        }
    }

    // Draw this node in the Node Graph Editor
    void drawInNodeGraph() override {
        ImGui::BeginGroup();
        ImGui::Text("Transform Node");
        for (auto& transformation : Transformations) {
            ImGui::BulletText("%s", transformation->GetName().c_str());
        }
        ImGui::EndGroup();
    }

    // Get input ports (e.g., velocity, angular velocity)
    std::vector<std::string> getInputPorts() const override {
        std::vector<std::string> inputs;
        for (auto& transformation : Transformations) {
            inputs.push_back(transformation->getName() + " Input");
        }
        return inputs;
    }

    // Get output ports (e.g., position, rotation, scale)
    std::vector<std::string> getOutputPorts() const override {
        std::vector<std::string> outputs;
        for (auto& transformation : Transformations) {
            outputs.push_back(transformation->getName() + " Output");
        }
        return outputs;
    }

private:
    std::vector<std::shared_ptr<Transformation>> Transformations; // List of transformations

    // Add default transformations (position, orientation, scale)
    void addDefaultTransformations() {
        Transformations.push_back(std::make_shared<PositionTransform>());
        Transformations.push_back(std::make_shared<RotationTransform>());
        Transformations.push_back(std::make_shared<ScaleTransform>());
    }
};

} // namespace hd
