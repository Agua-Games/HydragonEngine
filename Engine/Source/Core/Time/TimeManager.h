/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file TimeManager.h
 * @brief Header file for the TimeManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - TimeManager is a singleton class that manages the time in the game.
 * - It is responsible for updating the time and providing the time to other systems.
 */
#pragma once
#include <chrono>
#include <thread>
#include <atomic>
#include "Node.h"

namespace hd {

struct TimeManagerInfo : public NodeInfo {
    TimeManagerInfo() {
        NodeType = "Core/TimeManager";
        inputs = {
            "TimeScale" // Time scale
        };
        outputs = {
            "CurrentTime" // Current time
        };
    }
};

class TimeManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit TimeManager(const TimeManagerInfo& info = TimeManagerInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~TimeManager() = default;     // Default destructor
};

} // namespace hd
