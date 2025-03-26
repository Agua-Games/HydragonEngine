/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#if 0
#pragma once
#include <atomic>
#include <shared_mutex>
#include <variant>

namespace hd {

struct CompiledSubgraph {
    uint64_t cacheKey;                    // Hash of inputs and node configurations
    std::vector<uint8_t> compiledData;    // Optimized binary representation
    std::vector<std::string> inputPorts;  // Required inputs for the compiled subgraph
    std::vector<std::string> outputPorts; // Produced outputs from the compiled subgraph
    
    // Runtime execution function (compiled to optimized code)
    std::function<void(const std::vector<std::any>&, std::vector<std::any>&)> executeFunc;
};

class DependencyGraph {
public:
    void addDependency(Node* dependent, Node* dependency) {
        std::unique_lock lock(graphMutex);
        dependencies[dependent].insert(dependency);
        reverseDependencies[dependency].insert(dependent);
    }

    std::vector<Node*> getExecutionOrder() {
        std::shared_lock lock(graphMutex);
        std::vector<Node*> order;
        std::unordered_set<Node*> visited;
        std::unordered_set<Node*> processing;

        for (const auto& [node, _] : dependencies) {
            if (!visited.contains(node)) {
                topologicalSort(node, visited, processing, order);
            }
        }
        return order;
    }

    // Marks a subgraph for compilation
    void markCompilationBoundary(const std::vector<Node*>& subgraphNodes, 
                                const std::string& cacheIdentifier) {
        std::unique_lock lock(graphMutex);
        
        // Validate subgraph boundary
        for (auto* node : subgraphNodes) {
            // Check if all required inputs are available within the subgraph
            for (const auto& input : node->getInputPorts()) {
                if (!node->isPortValid(input)) {
                    throw std::runtime_error("Invalid port in subgraph boundary: " + 
                                           node->getName() + ":" + input);
                }
            }
        }

        CompilationBoundary boundary;
        boundary.nodes = subgraphNodes;
        boundary.identifier = cacheIdentifier;
        compilationBoundaries.push_back(boundary);
    }

    // Compiles marked subgraphs and updates execution strategy
    void compileMarkedSubgraphs() {
        std::unique_lock lock(graphMutex);
        for (const auto& boundary : compilationBoundaries) {
            if (auto compiled = tryLoadFromCache(boundary.identifier)) {
                compiledSubgraphs[boundary.identifier] = std::move(*compiled);
            } else {
                auto newCompiled = compileSubgraph(boundary);
                cacheCompiledSubgraph(boundary.identifier, newCompiled);
                compiledSubgraphs[boundary.identifier] = std::move(newCompiled);
            }
        }
        updateExecutionStrategy();
    }

    void processGraphParallel() {
        auto executionPlan = getExecutionPlan();
        
        for (const auto& task : executionPlan) {
            std::visit(overloaded{
                [](const SingleNode& node) {
                    node.ptr->processNode();
                },
                [](const CompiledSubgraphTask& subgraph) {
                    subgraph.execute();
                }
            }, task);
        }
    }

private:
    struct CompilationBoundary {
        std::vector<Node*> nodes;
        std::string identifier;
    };

    struct SingleNode {
        Node* ptr;
    };

    struct CompiledSubgraphTask {
        CompiledSubgraph* subgraph;
        std::vector<std::any> inputs;
        std::vector<std::any> outputs;
        void execute() { subgraph->executeFunc(inputs, outputs); }
    };

    using ExecutionTask = std::variant<SingleNode, CompiledSubgraphTask>;

    std::shared_mutex graphMutex;
    std::unordered_map<Node*, std::unordered_set<Node*>> dependencies;
    std::unordered_map<Node*, std::unordered_set<Node*>> reverseDependencies;
    std::vector<CompilationBoundary> compilationBoundaries;
    std::unordered_map<std::string, CompiledSubgraph> compiledSubgraphs;

    void topologicalSort(Node* node, 
                        std::unordered_set<Node*>& visited,
                        std::unordered_set<Node*>& processing,
                        std::vector<Node*>& order) {
        processing.insert(node);

        for (const auto& dep : dependencies[node]) {
            if (processing.contains(dep)) {
                throw std::runtime_error("Cyclic dependency detected");
            }
            if (!visited.contains(dep)) {
                topologicalSort(dep, visited, processing, order);
            }
        }

        processing.erase(node);
        visited.insert(node);
        order.push_back(node);
    }

    void groupNodesIntoLayers(const std::vector<Node*>& order,
                             std::vector<std::vector<Node*>>& layers) {
        std::unordered_map<Node*, size_t> nodeLayer;
        
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

    CompiledSubgraph compileSubgraph(const CompilationBoundary& boundary) {
        CompiledSubgraph result;
        
        // Analyze subgraph inputs/outputs
        analyzeSubgraphBoundaries(boundary, result);
        
        // Generate optimized execution code
        result.executeFunc = generateOptimizedExecutionCode(boundary);
        
        // Pack required data into compiled representation
        packCompiledData(boundary, result);
        
        return result;
    }

    void updateExecutionStrategy() {
        // Rebuild execution plan considering compiled subgraphs
        auto order = getExecutionOrder();
        executionPlan.clear();
        
        for (auto* node : order) {
            if (auto subgraph = findContainingSubgraph(node)) {
                if (isSubgraphEntryPoint(node, *subgraph)) {
                    executionPlan.push_back(createSubgraphTask(*subgraph));
                }
            } else {
                executionPlan.push_back(SingleNode{node});
            }
        }
    }

    std::vector<ExecutionTask> getExecutionPlan() const {
        std::shared_lock lock(graphMutex);
        return executionPlan;
    }

    // Cache management
    std::optional<CompiledSubgraph> tryLoadFromCache(const std::string& identifier);
    void cacheCompiledSubgraph(const std::string& identifier, const CompiledSubgraph& compiled);
    
    // Compilation helpers
    void analyzeSubgraphBoundaries(const CompilationBoundary& boundary, CompiledSubgraph& result);
    std::function<void(const std::vector<std::any>&, std::vector<std::any>&)> 
    generateOptimizedExecutionCode(const CompilationBoundary& boundary);
    void packCompiledData(const CompilationBoundary& boundary, CompiledSubgraph& result);

    std::vector<ExecutionTask> executionPlan;
};

} // namespace hd

#endif