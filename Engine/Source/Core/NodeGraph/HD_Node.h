/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <vector>
#include <future>
#include <mutex>

#if 0
namespace hd {

    // Struct to hold metadata and attributes for HD_Node
    struct HD_NodeInfo : public HD_ObjectInfo {
        std::string NodeType;               // Type of the node (e.g., "Transform", "Physics")
        //std::vector<std::string> validInputs;  // Allowed input types. Need to refactor this to be a list/check for each input port below.
        //std::vector<std::string> validOutputs; // Allowed output types. Need to refactor this to be a list/check for each output port below.
        std::vector<std::string> Inputs;   // List of input ports for the node
        std::vector<std::string> Outputs;  // List of output ports for the node
        bool IsStreamable = true;          // Whether the node supports streaming
        bool IsAsyncLoadable = true;       // Whether the node supports async loading

        // Constructor for convenience
        HD_NodeInfo(const std::string& name = "", bool isSerializable = true, bool isEditableInEditor = true,
                    const std::string& nodeType = "", const std::vector<std::string>& inputs = {},
                    const std::vector<std::string>& outputs = {}, bool isStreamable = true, bool isAsyncLoadable = true)
            : HD_ObjectInfo(name, isSerializable, isEditableInEditor),
              NodeType(nodeType), Inputs(inputs), Outputs(outputs),
              IsStreamable(isStreamable), IsAsyncLoadable(isAsyncLoadable) {}
    };

    // Base class for all nodes in Hydragon
    class HD_Node : public HD_Object {
    public:
        // Constructor with HD_NodeInfo for initialization
        explicit HD_Node(const HD_NodeInfo& info) : HD_Object(info), NodeInfo(info) {}

        // Virtual destructor for proper cleanup of derived classes
        virtual ~HD_Node() = default;

        // Getters for metadata
        const HD_NodeInfo& GetNodeInfo() const { return NodeInfo; }

        // Add a child node to this node (making it a node graph)
        void AddChild(const std::shared_ptr<HD_Node>& child) {
            std::lock_guard<std::mutex> lock(ChildrenMutex);
            Children.push_back(child);
        }

        // Remove a child node by name
        void RemoveChild(const std::string& childName) {
            std::lock_guard<std::mutex> lock(ChildrenMutex);
            Children.erase(std::remove_if(Children.begin(), Children.end(),
                                          [&childName](const std::shared_ptr<HD_Node>& child) {
                                              return child->GetInfo().Name == childName;
                                          }),
                           Children.end());
        }

        // Get all child nodes
        const std::vector<std::shared_ptr<HD_Node>>& GetChildren() const {
            return Children;
        }

        // Async load this node's data
        virtual std::future<void> LoadAsync() {
            return std::async(std::launch::async, [this]() {
                if (NodeInfo.IsAsyncLoadable) {
                    Load();
                }
            });
        }

        // Stream this node's data
        virtual void Stream() {
            if (NodeInfo.IsStreamable) {
                // Implement streaming logic here
            }
        }

        // Update logic for this node (can be overridden by derived classes)
        virtual void Update() {
            std::lock_guard<std::mutex> lock(ChildrenMutex);
            for (const auto& child : Children) {
                child->Update();
            }
        }
        // Get the imputs and outputs for this node, to use in Node Graph Editor, process input data and output it.
        // These methods should be implemented in derived classes.
        virtual std::vector<std::string> GetInputPorts() const = 0;  // Inputs for connections
        virtual std::vector<std::string> GetOutputPorts() const = 0; // Outputs for connections
};

        // Draw this node in the Inspector (can be overridden by derived classes)
        virtual void DrawInInspector() {
            ImGui::Text("Node Name: %s", NodeInfo.Name.c_str());
            ImGui::Text("Node Type: %s", NodeInfo.NodeType.c_str());
        }

        // Draw this node in the Node Graph Editor (can be overridden by derived classes)
        virtual void DrawInNodeGraph() {
            ImGui::BeginGroup();
            ImGui::Text("Node: %s", NodeInfo.Name.c_str());
            ImGui::Text("Inputs:");
            for (const auto& input : NodeInfo.Inputs) {
                ImGui::BulletText("%s", input.c_str());
            }
            ImGui::Text("Outputs:");
            for (const auto& output : NodeInfo.Outputs) {
                ImGui::BulletText("%s", output.c_str());
            }
            ImGui::EndGroup();
        }

        // Serialization (overrides base class implementation)
        void Serialize(std::ostream& stream) const override {
            std::lock_guard<std::mutex> lock(SerializationMutex);
            stream << "Node Name: " << NodeInfo.Name << "\n";
            stream << "Node Type: " << NodeInfo.NodeType << "\n";
            for (const auto& child : Children) {
                child->Serialize(stream);
            }
        }

        // Deserialization (overrides base class implementation)
        void Deserialize(std::istream& stream) override {
            std::lock_guard<std::mutex> lock(SerializationMutex);
            std::string line;
            while (std::getline(stream, line)) {
                if (line.find("Node Name:") != std::string::npos) {
                    NodeInfo.Name = line.substr(line.find(":") + 1);
                } else if (line.find("Node Type:") != std::string::npos) {
                    NodeInfo.NodeType = line.substr(line.find(":") + 1);
                }
            }
            for (auto& child : Children) {
                child->Deserialize(stream);
            }
        }

    protected:
        HD_NodeInfo NodeInfo;                      // Metadata and attributes for the node
        std::vector<std::shared_ptr<HD_Node>> Children; // Child nodes (making this a node graph)

        // Protected methods for derived classes
        virtual void Load() {
            // Default implementation for loading node data
        }

        mutable std::mutex ChildrenMutex;         // Mutex for thread-safe access to children
        mutable std::mutex SerializationMutex;    // Mutex for thread-safe serialization
    };

} // namespace hd
#endif