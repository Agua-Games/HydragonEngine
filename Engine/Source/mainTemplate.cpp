#if 0
/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Hydragon Engine's main entry point.
 */
#pragma once
#include <windows.h>
#include <iostream>
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_vulkan.h>

#include "hdImgui.h"
#include "ResourceManager.h"

float appIdleSleepTime = 60.0f;

/**
 * @brief Initializes the application and its main loop.
 * Initializes the windows manager (GLFW), graphics APIs (Vulkan, Directx), UI (ImGui), creates 
 * a window and Vulkan context, and runs the main loop.
 * The main loop is where your UI (ImGui) code and other code which needs to be updated every frame 
 * should go. The ImGui objects are updated and rendered in this function.
 * @return void
 */
void RunApplication() {
    // =========== Initialization ===========
    // Initialize GLFW, Vulkan, ImGui, etc.
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Create a window and Vulkan context
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    // Fill in Vulkan initialization info...
    ImGui_ImplVulkan_Init(&init_info);

    // === Initialize Resources Manager ===
    // Instantiate the ResourceManager
    hd::ResourceManager resourceManager;

    // Call GetEngineRootPath() to get the engine root path
    fs::path rootPath = resourceManager.GetEngineRootPath();

    // =========== Main Loop ===========
    while (!glfwWindowShouldClose(window)) {
        // Poll and handle window and input events
        glfwPollEvents();

        // Start the Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Process user interactions to handle wake-up
        hdImgui::ProcessUserInteractions(window);

        // Auto-sleep logic
        hdImgui::AutoSleepAfterInactivity(appIdleSleepTime);

        // Render your UI here
        if (!hdImgui::IsSleeping()) {
            // Your application's UI rendering code
            ImGui::Begin("Hydragon Editor");
            ImGui::Text("Hello, world!");
            ImGui::End();
        }

        // ImGui rendering
        ImGui::Render();

        // Vulkan rendering
    }

    // =========== Cleanup ===========
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief The main entry point of the application when run in console mode.
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 if the initialization was successful.
 */
/* int main(int argc, char** argv) {
    RunApplication();
    return 0;
} */

/**
 * @brief The main entry point of the application when run in Windows GUI mode.
 * @param hInstance The instance handle of the application.
 * @param hPrevInstance The previous instance handle of the application.
 * @param lpCmdLine The command line arguments.
 * @param nCmdShow The show state of the window.
 * @return 0 if the initialization was successful.
 */
/* int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    RunApplication();
    return 0;
} */
#endif