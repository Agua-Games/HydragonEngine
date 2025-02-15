/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <atomic>
#include <shared_mutex>

namespace hd {

class HD_DependencyGraph {
public:
    void AddDependency(HD_Node* dependent, HD_Node* dependency) {
        std::unique_lock lock(graphMutex);
        dependencies[dependent].insert(dependency);
        reverseDependencies[dependency].insert(dependent);
    }

    std::vector<HD_Node*> GetExecutionOrder() {
        std::shared_lock lock(graphMutex);
        std::vector<HD_Node*> order;
        std::unordered_set<HD_Node*> visited;
        std::unordered_set<HD_Node*> processing;

        for (const auto& [node, _] : dependencies) {
            if (!visited.contains(node)) {
                TopologicalSort(node, visited, processing, order);
            }
        }
        return order;
    }

    void ProcessGraphParallel() {
        auto order = GetExecutionOrder();
        
        // Group nodes that can be executed in parallel
        std::vector<std::vector<HD_Node*>> layers;
        GroupNodesIntoLayers(order, layers);

        // Process layers in sequence, nodes within layer in parallel
        for (const auto& layer : layers) {
            #pragma omp parallel for
            for (size_t i = 0; i < layer.size(); ++i) {
                layer[i]->Process();
            }
        }
    }

private:
    std::shared_mutex graphMutex;
    std::unordered_map<HD_Node*, std::unordered_set<HD_Node*>> dependencies;
    std::unordered_map<HD_Node*, std::unordered_set<HD_Node*>> reverseDependencies;

    void TopologicalSort(HD_Node* node, 
                        std::unordered_set<HD_Node*>& visited,
                        std::unordered_set<HD_Node*>& processing,
                        std::vector<HD_Node*>& order) {
        processing.insert(node);

        for (const auto& dep : dependencies[node]) {
            if (processing.contains(dep)) {
                throw std::runtime_error("Cyclic dependency detected");
            }
            if (!visited.contains(dep)) {
                TopologicalSort(dep, visited, processing, order);
            }
        }

        processing.erase(node);
        visited.insert(node);
        order.push_back(node);
    }

    void GroupNodesIntoLayers(const std::vector<HD_Node*>& order,
                             std::vector<std::vector<HD_Node*>>& layers) {
        std::unordered_map<HD_Node*, size_t> nodeLayer;
        
        // Assign layer numbers to nodes
        for (auto* node : order) {
            size_t maxDepLayer = 0;
            for (auto* dep : dependencies[node]) {
                maxDepLayer = std::max(maxDepLayer, nodeLayer[dep] + 1);
            }
            nodeLayer[node] = maxDepLayer;
            
            if (maxDepLayer >= layers.size()) {
                layers.resize(maxDepLayer + 1);
            }
            layers[maxDepLayer].push_back(node);
        }
    }
};

} // namespace hd