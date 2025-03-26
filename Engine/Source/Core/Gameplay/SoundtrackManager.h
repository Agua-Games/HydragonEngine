/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SoundtrackManager.h
 * @brief Header file for the SoundtrackManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - SoundtrackManager is a singleton class that manages the soundtrack in the game.
 * - It is responsible for updating the soundtrack and managing the soundtrack system.
 * - It uses the Vulkan API for soundtrack management.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"

namespace hd {

struct SoundtrackManagerInfo : public NodeInfo {
    SoundtrackManagerInfo() {
        nodeType = "Gameplay/SoundtrackManager";
        
        inputs = {
            "soundtrackData",  // Soundtrack data
            "soundtrackState"  // Soundtrack state
        };
        
        outputs = {
            "soundtrackStatus",  // Soundtrack status
            "soundtrackMetrics"  // Soundtrack performance metrics
        };
    }
};

class SoundtrackManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit SoundtrackManager(const SoundtrackManagerInfo& info = SoundtrackManagerInfo())
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
    ~SoundtrackManager() = default;     // Default destructor
};

} // namespace hd

