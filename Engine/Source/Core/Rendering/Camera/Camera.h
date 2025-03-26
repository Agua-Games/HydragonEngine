/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Camera.h
 * @brief Header file for the Camera class.
 * 
 * ARCHITECTURAL NOTES:
 * - Camera is a class that represents a camera in the scene.
 * 
*/
#pragma once
#include <vector>
#include <string>
//#include <glm/glm.hpp>
#include "Node.h"

namespace hd {

struct CameraInfo : public NodeInfo {
    CameraInfo() {
        NodeType = "Rendering/Camera";
        
        inputs = {
            "Position",      // Position of the camera
            "Target",        // Target of the camera
            "Up",            // Up vector of the camera
            "FOV",           // Field of view of the camera
            "AspectRatio",   // Aspect ratio of the camera
            "NearPlane",     // Near plane of the camera
            "FarPlane"       // Far plane of the camera
        };
        
        outputs = {
            "ViewMatrix",    // View matrix of the camera
            "ProjectionMatrix"// Projection matrix of the camera
        };
    }
};

class Camera : public Node {
public:
    // === Allocation, Initialization, Loading ===
    Camera() = default;

    glm::vec3 position;
    glm::vec3 target;
    glm::vec3 up;
    float fov;
    float aspectRatio;
    float nearPlane;
    float farPlane;

    void initialize();

    // === Processing ===
    void setViewMatrix(const glm::mat4& viewMatrix);
    void setProjectionMatrix(const glm::mat4& projectionMatrix);
    void setViewport(const glm::ivec4& viewport);
    vois setClearColor(const glm::vec4& clearColor);
    void () {
        updateViewMatrix();
        updateProjectionMatrix();
    }
    void update();
    void updateViewMatrix();
    void updateProjectionMatrix();

    // === Cleanup ===
    ~Camera() = default;
private:
    glm::mat4 viewMatrix;
    glm::mat4 projectionMatrix;
};

}