#if 0
/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "HD_Node.h"
#include <vector>
#include <memory>

namespace hd {

    // Base class for all transformation components
    class Transformation {
    public:
        virtual ~Transformation() = default;

        // Apply the transformation (e.g., update matrices)
        virtual void Apply() = 0;

        // Draw this transformation in the Inspector
        virtual void DrawInInspector() = 0;

        // Get the name of the transformation
        virtual std::string GetName() const = 0;
    };

    // Default Position Transformation
    class PositionTransformation : public Transformation {
    public:
        ImVec3 position = {0, 0, 0};
        ImVec3 velocity = {0, 0, 0};

        void Apply() override {
            // Update position based on velocity
            position.x += velocity.x * deltaTime;
            position.y += velocity.y * deltaTime;
            position.z += velocity.z * deltaTime;
        }

        void DrawInInspector() override {
            ImGui::DragFloat3("Position", &position.x);
            ImGui::DragFloat3("Velocity", &velocity.x);
        }

        std::string GetName() const override {
            return "Position";
        }
    };

    // Default Orientation Transformation
    class OrientationTransformation : public Transformation {
    public:
        ImVec3 rotation = {0, 0, 0}; // Euler angles for simplicity
        ImVec3 angularVelocity = {0, 0, 0};

        void Apply() override {
            // Update orientation based on angular velocity
            rotation.x += angularVelocity.x * deltaTime;
            rotation.y += angularVelocity.y * deltaTime;
            rotation.z += angularVelocity.z * deltaTime;
        }

        void DrawInInspector() override {
            ImGui::DragFloat3("Rotation", &rotation.x);
            ImGui::DragFloat3("Angular Velocity", &angularVelocity.x);
        }

        std::string GetName() const override {
            return "Orientation";
        }
    };

    // Default Scale Transformation
    class ScaleTransformation : public Transformation {
    public:
        ImVec3 scale = {1, 1, 1}; // Default scale is 1x

        void Apply() override {
            // No dynamic scaling logic here, but you could add it if needed
        }

        void DrawInInspector() override {
            ImGui::DragFloat3("Scale", &scale.x, 0.01f, 0.0f, 10.0f);
        }

        std::string GetName() const override {
            return "Scale";
        }
    };

    // TransformNode: A node that supports extendable transformations
    class TransformNode : public HD_Node {
    public:
        // Constructor with default transformations
        TransformNode(const HD_NodeInfo& info)
            : HD_Node(info) {
            AddDefaultTransformations();
        }

        // Add a custom transformation
        void AddTransformation(std::shared_ptr<Transformation> transformation) {
            Transformations.push_back(transformation);
        }

        // Remove a transformation by name
        void RemoveTransformation(const std::string& name) {
            Transformations.erase(std::remove_if(Transformations.begin(), Transformations.end(),
                                                 [&name](const std::shared_ptr<Transformation>& t) {
                                                     return t->GetName() == name;
                                                 }),
                                  Transformations.end());
        }

        // Update all transformations
        void Update() override {
            for (auto& transformation : Transformations) {
                transformation->Apply();
            }
        }

        // Draw this node in the Inspector
        void DrawInInspector() override {
            for (auto& transformation : Transformations) {
                transformation->DrawInInspector();
            }
        }

        // Draw this node in the Node Graph Editor
        void DrawInNodeGraph() override {
            ImGui::BeginGroup();
            ImGui::Text("Transform Node");
            for (auto& transformation : Transformations) {
                ImGui::BulletText("%s", transformation->GetName().c_str());
            }
            ImGui::EndGroup();
        }

        // Get input ports (e.g., velocity, angular velocity)
        std::vector<std::string> GetInputPorts() const override {
            std::vector<std::string> inputs;
            for (auto& transformation : Transformations) {
                inputs.push_back(transformation->GetName() + " Input");
            }
            return inputs;
        }

        // Get output ports (e.g., position, rotation, scale)
        std::vector<std::string> GetOutputPorts() const override {
            std::vector<std::string> outputs;
            for (auto& transformation : Transformations) {
                outputs.push_back(transformation->GetName() + " Output");
            }
            return outputs;
        }

    private:
        std::vector<std::shared_ptr<Transformation>> Transformations; // List of transformations

        // Add default transformations (position, orientation, scale)
        void AddDefaultTransformations() {
            Transformations.push_back(std::make_shared<PositionTransformation>());
            Transformations.push_back(std::make_shared<OrientationTransformation>());
            Transformations.push_back(std::make_shared<ScaleTransformation>());
        }
    };

} // namespace hd
    #endif