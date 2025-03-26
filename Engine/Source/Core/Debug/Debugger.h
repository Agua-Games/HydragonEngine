/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Debugger.h
 * @brief Header file for the Debugger class.
 * 
 * ARCHITECTURAL NOTES:
 * - Debugger is a class that represents a debugger in Hydragon.
 * - It is used to debug the game world in real-time.
 * - It is a part of the Debugger module in Hydragon.
 */
#pragma once
#include "Core.h"
#include "Node.h"

namespace hd {

struct DebuggerInfo : public NodeInfo {
    DebuggerInfo() {
        nodeType = "Debugger/Debugger";
        
        inputs = {
            "debugMode",     // Debug mode, like object selection, etc.
            "objectData",    // Object data to debug
            "debugAction"    // Debug action, like move, rotate, etc.
        };
        
        outputs = {
            "debuggedObject",  // Debugged object data
            "debugMetrics"     // Performance metrics
        };
    }
};

class Debugger : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit Debugger(const DebuggerInfo& info = DebuggerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default values
    // (...)

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Debugger() = default;     // Default destructor
};

} // namespace hd
