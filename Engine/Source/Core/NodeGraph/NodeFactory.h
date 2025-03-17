/** 
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * The Node Factory is responsible for creating and managing nodes in the engine's node graph.
 */
#pragma once

namespace hd {

struct NodeTemplateValidator {
    bool validateTemplate(const YAML::Node& node) {
        // Validate required fields
        // Check types compatibility
        // Validate port configurations
        // Ensure implementation completeness
    }
};

class NodeFactory {
public:
    // Register built-in nodes
    void registerBuiltInNodes() {           // TODO: Move to implementation file
        // This is a placeholder for actual registration logic, which we will try automate, using parsing, loops.
        registerNode<TransformNode>("Transform");
        registerNode<PhysicsNode>("Physics");
        // ...
    }

    // Load custom nodes from YAML/JSON
    void loadCustomNodes(const std::string& configPath) {
        // Parse YAML/JSON and create node templates
    }

    // Create node instance
    std::shared_ptr<Node> createNode(const std::string& nodeType) {              // TODO: Move to implementation file
        if (auto it = m_BuiltInNodes.find(nodeType); it != m_BuiltInNodes.end()) {
            return it->second();  // Use native implementation
        }
        return createCustomNode(nodeType);  // Create from template
    }
};

} // namespace hd