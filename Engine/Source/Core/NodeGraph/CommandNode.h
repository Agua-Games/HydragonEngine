/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CommandNode.h
 * @brief CommandNode represents a command node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Command nodes are used to execute functions with inputs and outputs.
 * - They can be synchronous or asynchronous.
 * 
 * TODO:
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */
#pragma once
#include <functional>
#include <future>
#include <tuple>
#include <chrono>
#include <any>

#include "Node.h"
#include "AIInterface.h"

namespace hd {

/**
 * @brief Command node for executing functions with inputs and outputs
 */
template<typename... Inputs>
class CommandNode : public Node<Inputs...> {
public:
    using CommandFunc = std::function<void(Inputs...)>;
    using AsyncCommandFunc = std::function<std::future<void>(Inputs...)>;

    CommandNode(const NodeInfo& info,
                  CommandFunc cmd,
                  std::tuple<std::string...> inNames,
                  bool isAsync = false)
        : Node(info)
        , command(std::move(cmd))
        , inputNames(std::move(inNames))
        , isAsynchronous(isAsync) {
        // Disable streaming by default for command nodes
        NodeInfo.streamingConfig.enabled = false;
    }

    CommandNode(const NodeInfo& info,
                  AsyncCommandFunc asyncCmd,
                  std::tuple<std::string...> inNames)
        : Node(info)
        , asyncCommand(std::move(asyncCmd))
        , inputNames(std::move(inNames))
        , isAsynchronous(true) {}

    void ProcessNodeGraph() override {
        if (isAsynchronous) {
            ExecuteAsyncCommand(std::index_sequence_for<Inputs...>{});
        } else {
            ExecuteCommand(std::index_sequence_for<Inputs...>{});
        }
    }

    std::future<void> LoadAsync() override {
        if (isAsynchronous) {
            return std::async(std::launch::async, [this]() {
                ProcessNodeGraph();
            });
        }
        return Node::LoadAsync();
    }

    // Required overrides
    void OnResume() override {
        if (!pendingTasks.empty()) {
            // Resume pending async tasks
        }
    }
    
    void OnPause() override {
        if (!pendingTasks.empty()) {
            // Pause/suspend pending async tasks
        }
    }
    
    void OnDirty() override {
        pendingTasks.clear();
    }

    /**
     * @brief Determine if this node can cache results.
     * Cache system integration. We must decide if command nodes should cache results by default. Because despite their dynamic nature, being able to cache may be 
     * beneficial in many situations. e.g. a transform node which scales a mesh, and we want to cache the result of the whole node chain before it, to avoiding unnecessary 
     * recomputation. The other design choice would be to have a dedicated "cache checkpoint" node, dedicated to marking cache checkpoints, but this venue would lead to
     * more verbosity.
     */
    bool CanCache() const override { return true; }
    
    uint64_t ComputeCacheKey() const override {
        return 0;
    }

    // AI integration
    AIInterface GetAIInterface() const override {
        AIInterface interface;
        interface.taskDesc.intent = "Execute parameterized commands";
        interface.capabilities.canModifyProperties = false;
        interface.capabilities.canModifyLogic = true;
        interface.semantics.purpose = "Command execution";
        interface.semantics.domain = "Command Processing";
        return interface;
    }

    std::vector<std::string> GetOptimizationSuggestions() const override {
        return {
            "Consider batching similar commands",
            "Evaluate async execution opportunities",
            "Check command dependencies for parallel execution"
        };
    }

private:
    CommandFunc command;
    AsyncCommandFunc asyncCommand;
    std::tuple<std::string...> inputNames;
    bool isAsynchronous = false;
    std::vector<std::future<void>> pendingTasks;

    // Helper to check if all inputs are ready
    bool AreInputsReady() const {
        return CheckInputsReady(std::index_sequence_for<Inputs...>{});
    }

    template<size_t... Is>
    bool CheckInputsReady(std::index_sequence<Is...>) const {
        return (... && HasInput<Inputs>(std::get<Is>(inputNames)));
    }

    template<size_t... Is>
    void ExecuteCommand(std::index_sequence<Is...>) {
        command(GetInput<Inputs>(std::get<Is>(inputNames))...);
    }

    template<size_t... Is>
    void ExecuteAsyncCommand(std::index_sequence<Is...>) {
        auto future = asyncCommand(GetInput<Inputs>(std::get<Is>(inputNames))...);
        pendingTasks.push_back(std::move(future));
    }
};

// Factory functions for cleaner syntax
template<typename... Inputs>
std::shared_ptr<CommandNode<Inputs...>> MakeCommandNode(
    const std::string& name,
    std::function<void(Inputs...)> cmd,
    std::tuple<std::string...> inputNames) {
    NodeInfo info(name, true, true, "Command", {}, {}, false);  // streaming disabled
    return std::make_shared<CommandNode<Inputs...>>(
        info, std::move(cmd), std::move(inputNames));
}

template<typename... Inputs>
std::shared_ptr<CommandNode<Inputs...>> MakeAsyncCommandNode(
    const std::string& name,
    std::function<std::future<void>(Inputs...)> cmd,
    std::tuple<std::string...> inputNames) {
    NodeInfo info(name);
    info.IsAsyncLoadable = true;
    return std::make_shared<CommandNode<Inputs...>>(
        info, std::move(cmd), std::move(inputNames));
}

} // namespace hd
