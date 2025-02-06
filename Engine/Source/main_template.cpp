/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Hydragon Engine's main entry point.
 */
#include <windows.h>
#include <iostream>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.h>

/**
 * @brief Main entry point for the application.
 * Initializes GLFW, Vulkan, ImGui, creates a window and Vulkan context, and runs the main loop.
 * The main loop is where your ImGui code  and other code which needs to be updated every frame 
 * should go. 
 * The ImGui objects are updated and rendered in this function.
 * To run the application as a console application (headless mode), with command line arguments, 
 * it should be called directly from this function.
 * @return void
 */
void RunApplication() {
    // Initialize GLFW, Vulkan, ImGui, etc. =================
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    // Create a window and Vulkan context ================================================
    GLFWwindow* window = glfwCreateWindow(1280, 720, "ImGui Example", nullptr, nullptr);
    if (!window) {
        glfwTerminate();
        return;
    }

    // Initialize ImGui =========================
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForVulkan(window, true);
    ImGui_ImplVulkan_InitInfo init_info = {};
    // Fill in Vulkan initialization info...
    ImGui_ImplVulkan_Init(&init_info);

    // Main loop ===============================
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // Hydragon's ImGui code here

        // ImGui rendering
        ImGui::Render();

        // Vulkan rendering code here
    }

    // Cleanup =================
    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief Entry point for the Windows application, when run on Windows.
 * @param hInstance The instance handle of the application.
 * @param hPrevInstance The previous instance handle of the application.
 * @param lpCmdLine The command line arguments.
 * @param nCmdShow The show state of the window.
 * @return 0 if the initialization was successful.
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    RunApplication();
    return 0;
}