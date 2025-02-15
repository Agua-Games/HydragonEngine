/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
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

    // Marks a subgraph for compilation
    void MarkCompilationBoundary(const std::vector<HD_Node*>& subgraphNodes, 
                                const std::string& cacheIdentifier) {
        std::unique_lock lock(graphMutex);
        CompilationBoundary boundary;
        boundary.nodes = subgraphNodes;
        boundary.identifier = cacheIdentifier;
        compilationBoundaries.push_back(boundary);
    }

    // Compiles marked subgraphs and updates execution strategy
    void CompileMarkedSubgraphs() {
        std::unique_lock lock(graphMutex);
        for (const auto& boundary : compilationBoundaries) {
            if (auto compiled = TryLoadFromCache(boundary.identifier)) {
                compiledSubgraphs[boundary.identifier] = std::move(*compiled);
            } else {
                auto newCompiled = CompileSubgraph(boundary);
                CacheCompiledSubgraph(boundary.identifier, newCompiled);
                compiledSubgraphs[boundary.identifier] = std::move(newCompiled);
            }
        }
        UpdateExecutionStrategy();
    }

    void ProcessGraphParallel() {
        auto executionPlan = GetExecutionPlan();
        
        for (const auto& task : executionPlan) {
            std::visit(overloaded{
                [](const SingleNode& node) {
                    node.ptr->Process();
                },
                [](const CompiledSubgraphTask& subgraph) {
                    subgraph.Execute();
                }
            }, task);
        }
    }

private:
    struct CompilationBoundary {
        std::vector<HD_Node*> nodes;
        std::string identifier;
    };

    struct SingleNode {
        HD_Node* ptr;
    };

    struct CompiledSubgraphTask {
        CompiledSubgraph* subgraph;
        std::vector<std::any> inputs;
        std::vector<std::any> outputs;
        void Execute() { subgraph->executeFunc(inputs, outputs); }
    };

    using ExecutionTask = std::variant<SingleNode, CompiledSubgraphTask>;

    std::shared_mutex graphMutex;
    std::unordered_map<HD_Node*, std::unordered_set<HD_Node*>> dependencies;
    std::unordered_map<HD_Node*, std::unordered_set<HD_Node*>> reverseDependencies;
    std::vector<CompilationBoundary> compilationBoundaries;
    std::unordered_map<std::string, CompiledSubgraph> compiledSubgraphs;

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

    CompiledSubgraph CompileSubgraph(const CompilationBoundary& boundary) {
        CompiledSubgraph result;
        
        // Analyze subgraph inputs/outputs
        AnalyzeSubgraphBoundaries(boundary, result);
        
        // Generate optimized execution code
        result.executeFunc = GenerateOptimizedExecutionCode(boundary);
        
        // Pack required data into compiled representation
        PackCompiledData(boundary, result);
        
        return result;
    }

    void UpdateExecutionStrategy() {
        // Rebuild execution plan considering compiled subgraphs
        auto order = GetExecutionOrder();
        executionPlan.clear();
        
        for (auto* node : order) {
            if (auto subgraph = FindContainingSubgraph(node)) {
                if (IsSubgraphEntryPoint(node, *subgraph)) {
                    executionPlan.push_back(CreateSubgraphTask(*subgraph));
                }
            } else {
                executionPlan.push_back(SingleNode{node});
            }
        }
    }

    std::vector<ExecutionTask> GetExecutionPlan() const {
        std::shared_lock lock(graphMutex);
        return executionPlan;
    }

    // Cache management
    std::optional<CompiledSubgraph> TryLoadFromCache(const std::string& identifier);
    void CacheCompiledSubgraph(const std::string& identifier, const CompiledSubgraph& compiled);
    
    // Compilation helpers
    void AnalyzeSubgraphBoundaries(const CompilationBoundary& boundary, CompiledSubgraph& result);
    std::function<void(const std::vector<std::any>&, std::vector<std::any>&)> 
    GenerateOptimizedExecutionCode(const CompilationBoundary& boundary);
    void PackCompiledData(const CompilationBoundary& boundary, CompiledSubgraph& result);

    std::vector<ExecutionTask> executionPlan;
};

} // namespace hd
