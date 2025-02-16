/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Hydragon Engine's main entry point.
 */
#pragma once
#include <windows.h>
#include <iostream>
#include <string>
#include <filesystem>
#include <GLFW/glfw3.h>

#include "Graphics/Vulkan/VulkanBackend.h"
#include "hdImgui.h"
#include "ResourceManager.h"

float appIdleSleepTime = 60.0f;

/**
 * @brief Initializes the application and its main loop.
 * @return void
 */
void RunApplication() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Create window with Vulkan context
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Hydragon Engine", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }

    try {
        // Initialize Vulkan backend
        hd::Graphics::VulkanBackend& vulkan = hd::Graphics::VulkanBackend::GetInstance();
        if (!vulkan.initialize(window)) {
            throw std::runtime_error("Failed to initialize Vulkan backend");
        }

        // Initialize ImGui with our Vulkan backend
        if (!vulkan.initializeImGui()) {
            throw std::runtime_error("Failed to initialize ImGui with Vulkan");
        }

        // Initialize Resource Manager
        hd::ResourceManager& resourceManager = hd::ResourceManager::GetInstance();
        fs::path rootPath = resourceManager.GetEngineRootPath();

        // Main loop
        while (!glfwWindowShouldClose(window)) {
            glfwPollEvents();

            // Begin frame
            if (!vulkan.beginFrame()) {
                continue;
            }

            // Process user interactions and handle sleep/wake
            hdImgui::ProcessUserInteractions(window);
            hdImgui::AutoSleepAfterInactivity(appIdleSleepTime);

            // Render UI if not sleeping
            if (!hdImgui::IsSleeping()) {
                hdImgui::RenderHydragonEditor();
            }

            // End frame and present
            vulkan.endFrame();
        }

        // Cleanup
        vulkan.waitIdle();
        vulkan.cleanup();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    // Cleanup GLFW
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief The main entry point of the application when run in Windows GUI mode.
 */
#if 0
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    RunApplication();
    return 0;
}
#endif











