/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Hydragon Engine's main entry point.
 */
#include <iostream>
#include <GLFW/glfw3.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_vulkan.h"

void RunHeadlessMode(int argc, char* argv[]) {
    std::cout << "Running in headless mode...\n";
    // Add your headless logic here
}

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
// Entry point
int WinMain(int argc, char* argv[]) {
    if (argc > 1 && std::string(argv[1]) == "--headless") {
        RunHeadlessMode(argc, argv);
    } else {
        RunGUIMode();
    }
    return 0;
}