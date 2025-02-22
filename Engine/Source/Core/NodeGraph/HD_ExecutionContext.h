/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include <chrono>
#include <string>
#include <memory>
#include <future>
#include <atomic>
#include <stack>

namespace hd {

enum class ExecutionStatus {
    Success,
    Failed,
    Pending,
    Canceled,
    Timeout
};

enum class ExecutionMode {
    Synchronous,
    Asynchronous,
    Deferred
};

struct ExecutionResult {
    ExecutionStatus status = ExecutionStatus::Pending;
    std::string errorMessage;
    std::chrono::microseconds executionTime{0};
    uint64_t frameNumber{0};
    std::string nodeId;  // ID of the executed node
};

class HD_ExecutionContext {
public:
    HD_ExecutionContext() = default;
    
    // Timeout control
    void SetTimeout(std::chrono::milliseconds timeout) {
        this->timeout = timeout;
    }

    const std::chrono::milliseconds& GetTimeout() const { 
        return timeout; 
    }

    bool HasTimedOut() const {
        if (startTime.time_since_epoch().count() == 0) return false;
        auto now = std::chrono::steady_clock::now();
        return (now - startTime) > timeout;
    }

    // Cancellation control
    bool ShouldCancel() const { 
        return cancelRequested.load(std::memory_order_relaxed); 
    }
    
    void RequestCancel() { 
        cancelRequested.store(true, std::memory_order_release); 
    }

    void ResetCancel() {
        cancelRequested.store(false, std::memory_order_release);
    }

    // Frame management
    void SetFrameNumber(uint64_t frame) { 
        frameNumber = frame; 
    }
    
    uint64_t GetFrameNumber() const { 
        return frameNumber; 
    }

    // Execution state
    void BeginExecution() {
        startTime = std::chrono::steady_clock::now();
        executionDepth++;
    }

    void EndExecution() {
        executionDepth--;
        if (executionDepth == 0) {
            startTime = std::chrono::steady_clock::time_point();
        }
    }

    bool IsExecuting() const {
        return executionDepth > 0;
    }

    // Execution mode control
    void SetExecutionMode(ExecutionMode mode) {
        executionMode = mode;
    }

    ExecutionMode GetExecutionMode() const {
        return executionMode;
    }

    // Error handling
    void SetLastError(const std::string& error) {
        lastError = error;
    }

    const std::string& GetLastError() const {
        return lastError;
    }

    // Context stack management
    void PushNodeId(const std::string& nodeId) {
        nodeStack.push(nodeId);
    }

    void PopNodeId() {
        if (!nodeStack.empty()) {
            nodeStack.pop();
        }
    }

    std::string GetCurrentNodeId() const {
        return nodeStack.empty() ? "" : nodeStack.top();
    }

private:
    std::chrono::milliseconds timeout{1000};  // Default 1 second timeout
    std::chrono::steady_clock::time_point startTime;
    std::atomic<bool> cancelRequested{false};
    uint64_t frameNumber{0};
    int executionDepth{0};
    ExecutionMode executionMode{ExecutionMode::Synchronous};
    std::string lastError;
    std::stack<std::string> nodeStack;  // Tracks execution hierarchy
};

} // namespace hd
