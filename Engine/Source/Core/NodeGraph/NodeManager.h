/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NodeManager.h
 * @brief NodeManager is responsible for managing the various node graphs we may have,
 * including creating, connecting, processing, keeping track of nodes, cleanup.
 * 
 * ARCHITECTURAL NOTES:
 * - NodeManager handles the creation, connection, processing and management of nodes.
 * 
 * TODO:
 * - The NodeManager, a singleton, should be responsible to keep track of and update all node graphs,
 * all nodes in the system.
 */
#pragma once
#include <memory>
#include <vector>
#include <unordered_set>
#include "Node.h"

namespace hd {

class NodeManager {
public:
    struct Connection {
        std::shared_ptr<Node> source;
        std::shared_ptr<Node> target;
        std::string sourcePort;
        std::string targetPort;
    };

    /**
     * @brief Get the singleton instance of NodeManager.
     */
    static NodeManager& get() {
        static NodeManager instance;
        return instance;
    }

    /**
     * @brief Create a new node.
     */
    std::shared_ptr<Node> createNode(const std::string& name = "") {
        auto node = std::make_shared<Node>();
        if (!name.empty()) {
            node->setName(name);
        }
        registerNode(node);
        return node;
    }

    /**
     * @brief Connect two nodes.
     */
    bool connect(const std::shared_ptr<Node>& source, 
                   const std::string& sourcePort = "",
                   const std::shared_ptr<Node>& target,
                   const std::string& targetPort = "") {
        if (isValidConnection(source, target, sourcePort, targetPort)) {
            // Add the connection
            connections.push_back({source, target, sourcePort, targetPort});

            // TODO: Notify nodes of the connection, make sure the connection persists
            source->onConnected(target, sourcePort, targetPort);
            target->onConnected(source, targetPort, sourcePort);

            return true;
        }
        return false;
    }

    void processGraph() {
        // TODO: Implement graph processing
    }

    void registerNode(const std::shared_ptr<Node>& node) {
        nodes.push_back(node);
    }

    template<typename T>
    T* findNode(const std::string& name) const {
        for (const auto& node : nodes) {
            if (node->getName() == name) {
                if (auto typed = std::dynamic_pointer_cast<T>(node)) {
                    return typed.get();
                }
            }
        }
        return nullptr;
    }

    std::pair<std::shared_ptr<Node>, std::string> parsePortPath(const std::string& targetPath) const;

    private:

    std::vector<Connection> connections;
    std::vector<std::shared_ptr<Node>> nodes;  // New member for node management

        bool isValidConnection(const std::shared_ptr<Node>& source, 
                             const std::shared_ptr<Node>& target,
                             const std::string& sourcePort,
                             const std::string& targetPort) {
            // Type compatibility check
            if (!sourcePort.empty() && !targetPort.empty()) {
                if (!source->isPortValid(sourcePort) || !target->isPortValid(targetPort)) {
                    return false;
                }
            }

            // Cycle detection
            if (wouldCreateCycle(source, target)) {
                return false;
            }

            return true;
        }

        bool wouldCreateCycle(const std::shared_ptr<Node>& source,
                             const std::shared_ptr<Node>& target) {
            // Simple DFS to detect cycles
            std::unordered_set<Node*> visited;
            return checkCycle(target.get(), source.get(), visited);
        }

        bool checkCycle(Node* current, Node* target, 
                       std::unordered_set<Node*>& visited) {
            if (current == target) return true;
            if (visited.contains(current)) return false;
            
            visited.insert(current);
            for (const auto& conn : connections) {
                if (conn.source.get() == current) {
                    if (checkCycle(conn.target.get(), target, visited)) {
                        return true;
                    }
                }
            }
            return false;
        }
    };
    
} // namespace hd
