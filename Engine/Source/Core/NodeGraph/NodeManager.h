/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
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
#include "Node.h"

namespace hd {

class NodeManager {
public:
    static NodeManager& Get() {
        static NodeManager instance;
        return instance;
    }

    bool Connect(const std::shared_ptr<Node>& source, 
                   const std::string& sourcePort = "",
                   const std::shared_ptr<Node>& target,
                   const std::string& targetPort = "") {
        if (IsValidConnection(source, target, sourcePort, targetPort)) {
            // Add the connection
            connections.push_back({source, target, sourcePort, targetPort});

            // TODO: Notify nodes of the connection, make sure the connection persists
            source->OnConnected(target, sourcePort, targetPort);
            target->OnConnected(source, targetPort, sourcePort);

            return true;
        }
        return false;
    }

    void ProcessGraph() {
        
    }

    // New node management functionality
    template<typename T>
    std::shared_ptr<T> CreateNode(const std::string& name = "") {
        auto node = std::make_shared<T>();
        if (!name.empty()) {
            node->SetName(name);
        }
        RegisterNode(node);
        return node;
    }

    void RegisterNode(const std::shared_ptr<Node>& node) {
        nodes.push_back(node);
    }

    template<typename T>
    T* FindNode(const std::string& name) const {
        for (const auto& node : nodes) {
            if (node->GetName() == name) {
                if (auto typed = std::dynamic_pointer_cast<T>(node)) {
                    return typed.get();
                }
            }
        }
        return nullptr;
    }

    private:
        struct Connection {
            std::shared_ptr<Node> source;
            std::shared_ptr<Node> target;
            std::string sourcePort;
            std::string targetPort;
        };

    std::vector<Connection> connections;
    std::vector<std::shared_ptr<Node>> nodes;  // New member for node management

        bool IsValidConnection(const std::shared_ptr<Node>& source, 
                             const std::shared_ptr<Node>& target,
                             const std::string& sourcePort,
                             const std::string& targetPort) {
            // Type compatibility check
            if (!sourcePort.empty() && !targetPort.empty()) {
                if (!source->IsPortValid(sourcePort) || !target->IsPortValid(targetPort)) {
                    return false;
                }
            }

            // Cycle detection
            if (WouldCreateCycle(source, target)) {
                return false;
            }

            return true;
        }

        bool WouldCreateCycle(const std::shared_ptr<Node>& source,
                             const std::shared_ptr<Node>& target) {
            // Simple DFS to detect cycles
            std::unordered_set<Node*> visited;
            return CheckCycle(target.get(), source.get(), visited);
        }

        bool CheckCycle(Node* current, Node* target, 
                       std::unordered_set<Node*>& visited) {
            if (current == target) return true;
            if (visited.contains(current)) return false;
            
            visited.insert(current);
            for (const auto& conn : connections) {
                if (conn.source.get() == current) {
                    if (CheckCycle(conn.target.get(), target, visited)) {
                        return true;
                    }
                }
            }
            return false;
        }
    };
    
} // namespace hd
