/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "HD_Node.h"
#include <functional>
#include <future>
#include <tuple>
#include <chrono>
#include <any>

namespace hd {

/**
 * @brief Command node for executing functions with inputs and outputs
 */
template<typename... Inputs>
class HD_CommandNode : public HD_Node {
public:
    using CommandFunc = std::function<void(Inputs...)>;
    using AsyncCommandFunc = std::function<std::future<void>(Inputs...)>;

    HD_CommandNode(const HD_NodeInfo& info,
                  CommandFunc cmd,
                  std::tuple<std::string...> inNames,
                  bool isAsync = false)
        : HD_Node(info)
        , command(std::move(cmd))
        , inputNames(std::move(inNames))
        , isAsynchronous(isAsync) {
        // Disable streaming by default for command nodes
        NodeInfo.streamingConfig.enabled = false;
    }

    HD_CommandNode(const HD_NodeInfo& info,
                  AsyncCommandFunc asyncCmd,
                  std::tuple<std::string...> inNames)
        : HD_Node(info)
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
        return HD_Node::LoadAsync();
    }

private:
    CommandFunc command;
    AsyncCommandFunc asyncCommand;
    std::tuple<std::string...> inputNames;
    bool isAsynchronous = false;
    std::vector<std::future<void>> pendingTasks;

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
std::shared_ptr<HD_CommandNode<Inputs...>> MakeCommandNode(
    const std::string& name,
    std::function<void(Inputs...)> cmd,
    std::tuple<std::string...> inputNames) {
    HD_NodeInfo info(name, true, true, "Command", {}, {}, false); // streaming disabled
    return std::make_shared<HD_CommandNode<Inputs...>>(
        info, std::move(cmd), std::move(inputNames));
}

template<typename... Inputs>
std::shared_ptr<HD_CommandNode<Inputs...>> MakeAsyncCommandNode(
    const std::string& name,
    std::function<std::future<void>(Inputs...)> cmd,
    std::tuple<std::string...> inputNames) {
    HD_NodeInfo info(name);
    info.IsAsyncLoadable = true;
    return std::make_shared<HD_CommandNode<Inputs...>>(
        info, std::move(cmd), std::move(inputNames));
}

} // namespace hd
