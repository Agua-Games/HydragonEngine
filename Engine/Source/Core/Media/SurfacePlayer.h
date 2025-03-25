/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *  
 * @file SurfacePlayer.h
 * @brief Header file for the SurfacePlayer class.
 * 
 * ARCHITECTURAL NOTES:
 * - SurfacePlayer is a class that represents a surface player node in the engine's node graph.
 * - It is used to play and manage surface-based media content.
 * - It can be used to play various types of surface-based media, such as videos, images, and animations.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Node.h"

namespace hd {

struct SurfacePlayerInfo : public NodeInfo {
    SurfacePlayerInfo() {
        NodeType = "Media/SurfacePlayer";
        inputs = {
            "Surface",       // Surface data
            "PlaybackSpeed"  // Playback speed
        };
        outputs = {
            "PlaybackStatus",  // Playback status
            "SurfaceMetrics"   // Surface performance metrics
        };
    }
};

class SurfacePlayer : public Node {
public:
    // === Allocation, Initialization, Loading === 
    explicit SurfacePlayer(const SurfacePlayerInfo& info = SurfacePlayerInfo())
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
    ~SurfacePlayer() = default;     // Default destructor
};

} // namespace hd
