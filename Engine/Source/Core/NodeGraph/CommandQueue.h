/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * Class for managing command queues
 */
#pragma once
#include <queue>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <vector>

namespace hd {

/**
 * @brief Command queue for executing commands in a thread-safe manner
 */
class CommandQueue {
public:
    struct Command {
        std::function<void()> execute;
        std::function<void()> undo;
        std::string description;
        bool isAsync;
    };

    void enqueueCommand(Command cmd) {
        std::lock_guard<std::mutex> lock(queueMutex);
        commandQueue.push(std::move(cmd));
        queueCV.notify_one();
    }

    void processQueue() {
        std::lock_guard<std::mutex> lock(queueMutex);
        while (!commandQueue.empty()) {
            auto& cmd = commandQueue.front();
            if (cmd.isAsync) {
                asyncCommands.push_back(
                    std::async(std::launch::async, cmd.execute));
            } else {
                cmd.execute();
            }
            history.push_back(cmd);
            commandQueue.pop();
        }
    }

    void waitForAsyncCommands() {
        for (auto& future : asyncCommands) {
            future.wait();
        }
        asyncCommands.clear();
    }

    // Undo last command
    void undo() {
        if (!history.empty()) {
            auto& cmd = history.back();
            cmd.undo();
            history.pop_back();
        }
    }

private:
    std::queue<Command> commandQueue;
    std::vector<Command> history;
    std::vector<std::future<void>> asyncCommands;
    std::mutex queueMutex;
    std::condition_variable queueCV;
};

} // namespace hd