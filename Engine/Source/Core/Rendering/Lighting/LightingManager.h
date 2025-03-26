/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LightingManager.h
 * @brief Header file for the LightingManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - LightingManager is a singleton class that manages the lighting in the scene.
 * - It is responsible for updating the lighting and managing the lighting system.
 * - It uses the Vulkan API for light management.
 */
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Node.h"
#include "Light.h"

namespace hd {

struct LightingManagerInfo : public NodeInfo {
    LightingManagerInfo() {
        NodeType = "Rendering/LightingManager";
        inputs = {
            "Lights" // Array of lights
        };
        outputs = {
            "ActiveLights" // Active lights
        };
    }
};

class LightingManager : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit LightingManager(const LightingManagerInfo& info = LightingManagerInfo())
        : Node(info), LightingManagerInfo(info) {}
    void initialize() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();
    void addLight(const std::shared_ptr<Light>& light);
    void removeLight(const std::shared_ptr<Light>& light);

    // === Cleanup ===
    ~LightingManager() override = default; // Default destructor
private:
    std::vector<std::shared_ptr<Light>> lights;
};

} // namespace hd
