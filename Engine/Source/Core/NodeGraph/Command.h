/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Command.h
 * @brief Command is used to execute a function with inputs and outputs.
 * 
 * ARCHITECTURAL NOTES:
 * - It can be synchronous or asynchronous, and can be executed in parallel threads.
 * - Command takes loose inspiration from Vulkan's single-time command buffer concept.
 * - For a node which executes an array of commands, use Commands.
 * 
 * TODO:
 * - Check with assistant if caching functions are compatible with caching in Node, not conflicting with it - should simply override and extend it.
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
 * @brief Command is used to execute a function with inputs and outputs.
 */
template<typename... Inputs>
class Command : public Node<Inputs...> {
public:
    // === Initialization ===
    using CommandFunc = std::function<void(Inputs...)>;
    using AsyncCommandFunc = std::function<std::future<void>(Inputs...)>;

    Command(const NodeInfo& info,
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

    Command(const NodeInfo& info,
                  AsyncCommandFunc asyncCmd,
                  std::tuple<std::string...> inNames)
        : Node(info)
        , asyncCommand(std::move(asyncCmd))
        , inputNames(std::move(inNames))
        , isAsynchronous(true) {}

    // === Caching & Optimization ===
    /**
     * @brief Determine if this node can cache results.
     * Cache system integration. We must decide if command nodes should cache results by default. Because despite their dynamic nature, being able to cache may be 
     * beneficial in many situations. e.g. a transform node which scales a mesh, and we want to cache the result of the whole node chain before it, to avoiding unnecessary 
     * recomputation. The other design choice would be to have a dedicated "cache checkpoint" node, dedicated to marking cache checkpoints, but this venue would lead to
     * more verbosity.
     */
    bool canCache() const override { return true; }
    
    uint64_t computeCacheKey() const override {
        return 0;
    }

    // === Streaming ===
    // TODO: Study wether to nest processNodeGraph() and other processing functions here in load time. If so, conform the architecture in the other nodes to
    // be unified, follow the same pattern
    std::future<void> loadAsync() override {
        if (isAsynchronous) {
            return std::async(std::launch::async, [this]() {
                processNodeGraph();
            });
        }
        return Node::loadAsync();
    }

    // === Processing ===
    void processNodeGraph() override {
        if (isAsynchronous) {
            executeAsyncCommand(std::index_sequence_for<Inputs...>{});
        } else {
            executeCommand(std::index_sequence_for<Inputs...>{});
        }
    }

    // === Execution ===
    // Required overrides
    void onResume() override {
        if (!pendingTasks.empty()) {
            // Resume pending async tasks
        }
    }
    
    void onPause() override {
        if (!pendingTasks.empty()) {
            // Pause/suspend pending async tasks
        }
    }
    
    void onDirty() override {
        pendingTasks.clear();
    }

    // === AI Agent Integration ===
    AIInterface getAIInterface() const override {
        AIInterface interface;
        interface.taskDesc.intent = "Execute parameterized commands";
        interface.capabilities.canModifyProperties = false;
        interface.capabilities.canModifyLogic = true;
        interface.semantics.purpose = "Command execution";
        interface.semantics.domain = "Command Processing";
        return interface;
    }

    std::vector<std::string> getOptimizationSuggestions() const override {
        return {
            "Consider batching similar commands",
            "Evaluate async execution opportunities",
            "Check command dependencies for parallel execution"
        };
    }

private:
    // === Initialization ===
    // Factory functions for cleaner syntax
    template<typename... Inputs>
    std::shared_ptr<Command<Inputs...>> makeCommandNode(
        const std::string& name,
        std::function<void(Inputs...)> cmd,
        std::tuple<std::string...> inputNames) {
        NodeInfo info(name, true, true, "Command", {}, {}, false);  // streaming disabled
        return std::make_shared<Command<Inputs...>>(
            info, std::move(cmd), std::move(inputNames));
    }

template<typename... Inputs>
std::shared_ptr<Command<Inputs...>> makeAsyncCommandNode(
    const std::string& name,
    std::function<std::future<void>(Inputs...)> cmd,
    std::tuple<std::string...> inputNames) {
    NodeInfo info(name);
    info.IsAsyncLoadable = true;
    return std::make_shared<Command<Inputs...>>(
        info, std::move(cmd), std::move(inputNames));
}

    // === Execution ===
    // Command and input
    CommandFunc command;
    AsyncCommandFunc asyncCommand;
    std::tuple<std::string...> inputNames;      // ?
    bool isAsynchronous = false;
    std::vector<std::future<void>> pendingTasks;

    // Helper to check if all inputs are ready
    bool areInputsReady() const {
        return checkInputsReady(std::index_sequence_for<Inputs...>{});
    }

    template<size_t... Is>
    bool checkInputsReady(std::index_sequence<Is...>) const {
        return (... && hasInput<Inputs>(std::get<Is>(inputNames)));
    }

    template<size_t... Is>
    void executeCommand(std::index_sequence<Is...>) {
        command(getInput<Inputs>(std::get<Is>(inputNames))...);
    }

    template<size_t... Is>
    void executeAsyncCommand(std::index_sequence<Is...>) {
        auto future = asyncCommand(getInput<Inputs>(std::get<Is>(inputNames))...);
        pendingTasks.push_back(std::move(future));
    }
};

} // namespace hd