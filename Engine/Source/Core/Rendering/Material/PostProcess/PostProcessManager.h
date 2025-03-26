/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PostProcessManager.h
 * @brief Header file for the PostProcessManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - PostProcessManager is a singleton class that manages the post-processing effects in the scene.
 * - It is responsible for updating the post-processing effects and managing the post-processing system.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Wave.h"
#include "PhysicsTypes.h"
#include "PostProcess.h"

namespace hd {

struct PostProcessManagerInfo : public NodeInfo {
    PostProcessManagerInfo() {
        NodeType = "Rendering/PostProcessManager";
        inputs = {
            "PostProcesses" // Array of post-processing effects
        };
        outputs = {
            "ActivePostProcesses" // Active post-processing effects
        };
    }
};

class PostProcessManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit PostProcessManager(const PostProcessManagerInfo& info = PostProcessManagerInfo())
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
    ~PostProcessManager() = default;     // Default destructor
};

} // namespace hd
