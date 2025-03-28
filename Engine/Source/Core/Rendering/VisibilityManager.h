/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VisibilityManager.h
 * @brief Header file for the VisibilityManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - VisibilityManager is a singleton class that manages the visibility of objects in the scene.
 * - It is responsible for updating the visibility and managing the visibility system.
 * - It uses the Vulkan API for visibility management.
 */

#pragma once
#include "Node.h"
#include "CameraManager.h"
#include "Camera.h"

namespace hd {

struct VisibilityManagerInfo : public NodeInfo {
    VisibilityManagerInfo() {
        nodeType = "Rendering/VisibilityManager";
        inputs = {
            "cameras"               // Array of cameras
        };
        outputs = {
            "visibleObjects"        // Visible objects
        };
    }
};

class VisibilityManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit VisibilityManager(const VisibilityManagerInfo& info = VisibilityManagerInfo())
        : Node(info) {}   
    initialize() override {}
    load() override {}

    // Set default params
    float viewDistance = 500.0f;

    // === Processing ===
    void processNode() override {
 
    }
    void updateVisibility();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~VisibilityManager() = default;     // Default destructor
};

} // namespace hd
