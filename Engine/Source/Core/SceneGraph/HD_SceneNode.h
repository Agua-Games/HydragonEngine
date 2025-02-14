/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#if 0
#include "HD_Node.h"

namespace hd {

    // Struct to hold metadata and attributes for HD_Scene
    struct HD_SceneInfo : public HD_NodeInfo {
        std::string LayerPath;               // Path to the USD layer file
        std::vector<std::string> References; // List of referenced USD files
        bool IsLayered = true;               // Whether the scene uses USD layers

        // Constructor for convenience
        HD_SceneInfo(const std::string& name = "", bool isSerializable = true, bool isEditableInEditor = true,
                     const std::string& nodeType = "", const std::vector<std::string>& inputs = {},
                     const std::vector<std::string>& outputs = {}, bool isStreamable = true, bool isAsyncLoadable = true,
                     const std::string& layerPath = "", const std::vector<std::string>& references = {}, bool isLayered = true)
            : HD_NodeInfo(name, isSerializable, isEditableInEditor, nodeType, inputs, outputs, isStreamable, isAsyncLoadable),
              LayerPath(layerPath), References(references), IsLayered(isLayered) {}
    };

    // Base class for all scenes in Hydragon
    class HD_SceneNode : public HD_Node {
    public:
        // Constructor with HD_SceneInfo for initialization
        explicit HD_Scene(const HD_SceneInfo& info) : HD_Node(info), SceneInfo(info) {}

        // Virtual destructor for proper cleanup of derived classes
        virtual ~HD_Scene() = default;

        // Getters for metadata
        const HD_SceneInfo& GetSceneInfo() const { return SceneInfo; }

        // Load the scene asynchronously
        std::future<void> LoadAsync() override {
            return std::async(std::launch::async, [this]() {
                if (SceneInfo.IsAsyncLoadable) {
                    LoadScene();
                }
            });
        }

        // Stream the scene
        void Stream() override {
            if (SceneInfo.IsStreamable) {
                StreamScene();
            }
        }

        // Update the scene (propagate transformations, etc.)
        void Update() override {
            PropagateTransformations();
            HD_Node::Update(); // Call base class update to process children
        }

        // Draw this scene in the Inspector
        void DrawInInspector() override {
            ImGui::Text("Scene Name: %s", SceneInfo.Name.c_str());
            ImGui::Text("Layer Path: %s", SceneInfo.LayerPath.c_str());
            ImGui::Text("References:");
            for (const auto& reference : SceneInfo.References) {
                ImGui::BulletText("%s", reference.c_str());
            }
        }

        // Draw this scene in the Node Graph Editor
        void DrawInNodeGraph() override {
            ImGui::BeginGroup();
            ImGui::Text("Scene: %s", SceneInfo.Name.c_str());
            ImGui::Text("Layer Path: %s", SceneInfo.LayerPath.c_str());
            ImGui::Text("References:");
            for (const auto& reference : SceneInfo.References) {
                ImGui::BulletText("%s", reference.c_str());
            }
            ImGui::EndGroup();
        }

    protected:
        HD_SceneInfo SceneInfo; // Metadata and attributes for the scene

        // Protected methods for derived classes
        virtual void LoadScene() {
            // Load the scene from the USD layer file
            if (!SceneInfo.LayerPath.empty()) {
                // Example: Load USD file and parse layers/references
            }
        }

        virtual void StreamScene() {
            // Implement streaming logic for the scene
        }

        virtual void PropagateTransformations() {
            // Propagate transformations from parent to child nodes
            // Example: Apply parent transforms to children
        }
    };

} // namespace hd
#endif

#if 0
// Adapt and insert these member methods later. For adding and removing nodes, reading and writing to files (preferrably scene graph based formats).
void AddNode(std::shared_ptr<HD_Node> node);
void RemoveNode(const std::string& nodeName);
void ReferenceSceneGraph(const std::shared_ptr<HD_SceneGraph>& otherGraph);
void LoadFromFile(const std::string& filePath);
void SaveToFile(const std::string& filePath);
#endif