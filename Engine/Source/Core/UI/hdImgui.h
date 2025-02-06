#if 0
#pragma once

#include <string>
#include "imgui.h"  // Include ImGui's header

/**
 * @brief Namespace for ImGui integration functions.
 */
namespace hdImgui {
    // =========== Initialization ===========
    /** 
     * @brief Initializes hdImgui.
     * @param window The GLFW window to initialize ImGui for.
     */
    void Initialize(GLFWwindow* window);

    // =========== Styling ===========    
    void StyleColorsHydragonDark();
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

    /**
     * @brief Processes user interactions.
     * @param window The GLFW window to process user interactions for.
     */
    void ProcessUserInteractions(GLFWwindow* window);

} // namespace hdImgui
#endif
