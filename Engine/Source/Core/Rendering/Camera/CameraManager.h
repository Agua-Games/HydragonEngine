/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CameraManager.h
 * @brief Header file for the CameraManager class.
 * 
 * ARCHITECTURAL NOTES:
 * - CameraManager is a singleton class that manages the cameras in the scene.
 * - It is responsible for updating the cameras and managing the camera system.
 * 
 */
#pragma once
#include <vector>
#include <memory>
#include <string>
#include "Camera.h"

namespace hd {

struct CameraManagerInfo : public NodeInfo {
    CameraManagerInfo() {
        NodeType = "Rendering/CameraManager";
        inputs = {
            "Cameras" // Array of cameras
        };
        outputs = {
            "ActiveCamera" // Active camera
        };
    }
};

class CameraManager : public Node { 
public:
    // === Allocation, Initialization, Loading ===
    explicit CameraManager(const CameraManagerInfo& info = CameraManagerInfo())
        : Node(info), CameraManagerInfo(info) {}   
    void initialize();

    // === Processing ===
    void processNodeGraph() {
        update();
    }
    void update();
    void addCamera(const std::shared_ptr<Camera>& camera);
    void removeCamera(const std::shared_ptr<Camera>& camera);

    // === Cleanup ===
    ~CameraManager() = default;
private:
    std::vector<std::shared_ptr<Camera>> cameras;
};

}