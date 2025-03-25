/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LiveEditing.h
 * @brief Header file for the LiveEditing class.
 * 
 * ARCHITECTURAL NOTES:
 * - LiveEditing is a class that represents a live editing session in Hydragon.
 * - It is used to edit the game world in real-time.
 * - It is a part of the LiveEditing module in Hydragon.
 */
#pragma once
#include "Core.h"
#include "Node.h"

namespace hd {

struct LiveEditingInfo : public NodeInfo {
    LiveEditingInfo() {
        NodeType = "LiveEditing/LiveEditing";
        
        inputs = {
            "EditMode",      // Edit mode, like object selection, etc.
            "ObjectData",    // Object data to edit
            "EditAction"     // Edit action, like move, rotate, etc.
        };
        
        outputs = {
            "EditedObject",  // Edited object data
            "EditMetrics"    // Performance metrics
        };
    }
};

class LiveEditing : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit LiveEditing(const LiveEditingInfo& info = LiveEditingInfo())
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
    ~LiveEditing() = default;     // Default destructor
};

} // namespace hd
