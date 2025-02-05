/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Hydragon Engine's main entry point.
 */
#include <windows.h>
#include <iostream>
#include <glfw/glfw3.h>
#include "ThirdParty/imgui/imgui.h"
#include "ThirdParty/imgui/backends/imgui_impl_glfw.h"
#include "ThirdParty/imgui/backends/imgui_impl_vulkan.h"

/**
 * @brief Runs the engine in headless mode.
 * @param argc The number of command line arguments.
 * @param argv The command line arguments.
 * @return Void.
 */
void RunHeadlessMode(int argc, char* argv[]) {
    std::cout << "Running in headless mode...\n";
    // Add your headless logic here
}

/**
 * @brief Runs the engine in GUI mode.
 * @return Void.
 */
void RunGUIMode() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW\n";
        return;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Hydragon", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window\n";
        glfwTerminate();
        return;
    }
    glfwMakeContextCurrent(window);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForVulkan(window, true);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplGlfw_NewFrame();

        // Render ImGui content here

        // Render the frame
        ImGui::Render();
    }

    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}

/**
 * @brief The main entry point for the engine.
 * @param hInstance The instance handle for the application.
 * @param hPrevInstance The previous instance handle for the application.
 * @param lpCmdLine The command line for the application.
 * @param nCmdShow The show command for the application.
 * @return The exit code for the application.
 */
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow) {
    // Initialize GLFW
    if(!glfwInit()) {
        MessageBoxA(NULL, "Failed to initialize GLFW", "Error", MB_OK | MB_ICONERROR);
        return -1;
    }

    // Create a window
    GLFWwindow* window = glfwCreateWindow(1280, 720, "Hydragon", nullptr, nullptr);
    if (!window) {
        MessageBoxA(NULL, "Failed to create GLFW window", "Error", MB_OK | MB_ICONERROR);
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGui_ImplGlfw_InitForVulkan(window, true);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();

        // Start ImGui frame
        ImGui_ImplGlfw_NewFrame();

        // Render ImGui content here

        // Render the frame
        ImGui::Render();
    }

    // Cleanup
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}