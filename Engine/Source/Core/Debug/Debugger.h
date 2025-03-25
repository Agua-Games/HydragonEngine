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
        NodeType = "Debugger/Debugger";
        
        inputs = {
            "DebugMode",     // Debug mode, like object selection, etc.
            "ObjectData",    // Object data to debug
            "DebugAction"    // Debug action, like move, rotate, etc.
        };
        
        outputs = {
            "DebuggedObject",  // Debugged object data
            "DebugMetrics"     // Performance metrics
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

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Debugger() = default;     // Default destructor
};

} // namespace hd
