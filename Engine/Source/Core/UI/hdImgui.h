/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <GLFW/glfw3.h>  // Include GLFW header for GLFWwindow
#include "imgui.h"  // Include ImGui's header

/**
 * @brief Namespace for ImGui integration functions.
 */
namespace hdImgui {
    #if 0
    // =========== Initialization ===========
    /** 
     * @brief Initializes hdImgui.
     * @param window The GLFW window to initialize ImGui for.
     */
    void Initialize(GLFWwindow* window);

    // =========== Styling =========== 
    #endif   
    void StyleColorsHydragonDark();
    #if 0
    void StyleColorsHydragonLight();
    void StyleColorsHydragonClassic();
    void StyleColorsHydragonModern();

    // Custom font loading
    void LoadFonts(const std::string& defaultFontPath = "", float defaultFontSize = 13.0f);

    // Icon font loading
    void LoadIconFonts(const std::string& iconFontPath = "", float iconFontSize = 13.0f);

    // =========== Input handling ===========
    /**
     * @brief Sleeps the ImGui window, pausing rendering and processing of most events.
     */
    void Sleep(bool enable);
    void AutoSleepAfterInactivity(float idleTimeSeconds = 60.0f);

    // Utility to check if ImGui is sleeping
    bool IsSleeping();

    // =========== Input handling ===========
    void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
    #endif

    // =========== Rendering ===========
    /**
     * @brief Renders the ImGui user interface.
     */
    void RenderHydragonEditor();
} // namespace hdImgui

