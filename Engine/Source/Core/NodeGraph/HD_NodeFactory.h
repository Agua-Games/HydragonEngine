/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * The Node Factory is responsible for creating and managing nodes in the engine's node graph.
 */

namespace hd {

class HD_NodeFactory {
public:
    // Register built-in nodes
    void RegisterBuiltInNodes() {
        // This is a placeholder for actual registration logic, which we will try automate, using parsing, loops.
        RegisterNode<HD_TransformNode>("Transform");
        RegisterNode<HD_PhysicsNode>("Physics");
        // ...
    }

    // Load custom nodes from YAML/JSON
    void LoadCustomNodes(const std::string& configPath) {
        // Parse YAML/JSON and create node templates
    }

    // Create node instance
    std::shared_ptr<HD_Node> CreateNode(const std::string& nodeType) {
        if (auto it = m_BuiltInNodes.find(nodeType); it != m_BuiltInNodes.end()) {
            return it->second();  // Use native implementation
        }
        return CreateCustomNode(nodeType);  // Create from template
    }
};

} // namespace hd