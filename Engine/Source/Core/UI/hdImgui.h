/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <GLFW/glfw3.h>  // Include GLFW header for GLFWwindow
#include <imgui.h>  // Include ImGui's header

/**
 * @brief Namespace for ImGui integration functions.
 */
namespace hdImgui {
struct HdEditorWindowData 
{
    // Layout
    float globalWindowBgAlpha = 0.35f;
    // Font pointers are initialized as nullptr and set during hdImgui::Initialize()
    // after ImGui context and ResourceManager are available
    ImFont* iconFont = nullptr;
    ImFont* defaultFont = nullptr;

    // Editor sleep and auto-sleep feature
    bool isSleeping = false;

    // State - Track the open/closed state of each window
    bool isMainMenuBarWindowOpen = true;
    bool isTopToolbarWindowOpen = true;
    bool isLeftToolbarWindowOpen = true;
    bool isRightToolbarWindowOpen = true;
    bool isBottomToolbarWindowOpen = false;
    bool isSceneGraphWindowOpen = true;

    bool isNodeGraphWindowOpen = true;
    bool nodeGraphEditor_autoCompile = true;

    bool isScriptWindowOpen = true;
    bool isScriptsPaletteWindowOpen = true;
    bool isCommandsPaletteWindowOpen = true;
    bool isConsoleWindowOpen = true;
    bool isAgentsWindowOpen = true;

    bool isViewport3DWindowOpen = true;
    bool showViewport3DGrid = true;
    bool snapToViewport3DGrid = true;
    bool showViewport3DAxes = true;
    int viewport3D_AntiAliasing = 2;
    float viewport3D_BrushSize = 1.0f;
    float viewport3D_BrushMinSize = 0.0f;
    float viewport3D_BrushMaxSize = 100.0f;
    float viewport3D_BrushOpacity = 1.0f;
    float viewport3D_BrushMinOpacity = 0.0f;
    float viewport3D_BrushMaxOpacity = 1.0f;  
    float viewport3D_BrushHardness = 1.0f;  
    float viewport3D_BrushMinHardness = 0.0f;  
    float viewport3D_BrushMaxHardness = 1.0f;  
    float viewport3D_BrushColor[3] = { 0.0f, 0.0f, 0.0f};  
    bool isViewport3DToolsWindowOpen = true;

    bool isViewport2DWindowOpen = true;
    bool showViewport2DGrid = true;
    bool snapToViewport2DGrid = true;
    bool showViewport2DAxes = true;
    bool showViewport2DUVs = true;
    int viewport2D_AntiAliasing = 2;
    float viewport2D_BrushSize = 1.0f;
    float viewport2D_BrushMinSize = 0.0f;
    float viewport2D_BrushMaxSize = 100.0f;
    float viewport2D_BrushOpacity = 1.0f;
    float viewport2D_BrushMinOpacity = 0.0f;
    float viewport2D_BrushMaxOpacity = 1.0f;  
    float viewport2D_BrushHardness = 1.0f;  
    float viewport2D_BrushMinHardness = 0.0f;  
    float viewport2D_BrushMaxHardness = 1.0f;  
    float viewport2D_BrushColor[3] = { 0.0f, 0.0f, 0.0f};  
    bool isViewport2DToolsWindowOpen = true;

    bool isPropertiesWindowOpen = true;
    bool isAssetManagerWindowOpen = true;
    bool isLightingWindowOpen = true;
    bool isPhysicsWindowOpen = true;
    bool isBottomStatusBarWindowOpen = true;
    bool isFileExplorerWindowOpen = true;
    bool isPatternOrchestratorWindowOpen = true;

    bool isProfilerWindowOpen = true;
    bool profilerRecordingEnabled = true;

    bool isStreamingWindowOpen = true;
    bool streamingEnabled = true;

    bool isImageWindowOpen = true;
    bool isAudioWindowOpen = true;
    bool isMontageWindowOpen = true;
    bool isTextWindowOpen = true;
    bool isFontWindowOpen = true;
    bool isPluginWindowOpen = true;
    bool isExtensionsWindowOpen = true;
    bool isMacrosWindowOpen = true;
    bool isDramaWindowOpen = true;
    bool isChimeraPipelineWindowOpen = true;
    bool isUIEditorWindowOpen = true;
    bool isNetworkingWindowOpen = true;
    bool isPerformanceScalabilityWindowOpen = true;
    bool isReflectionWindowOpen = true;
    bool isCollaborationWindowOpen = true;
    bool isCommunityWindowOpen = true;
    bool isInputWindowOpen = true;
    bool isPropertiesMatrixWindowOpen = true;
    bool isLocalizationWindowOpen = true;
    bool isSettingsWindowOpen = true;
    bool isMeshWindowOpen = true;
    bool isMonetizationWindowOpen = true;
    bool isProjectInsightsWindowOpen = true;
    bool isVolumeWindowOpen = true;
    bool isPresetsWindowOpen = true;
    bool isDebugWindowOpen = true;
};

// =========== Initialization ===========
/** 
 * @brief Initializes hdImgui.
 * @param window The GLFW window to initialize ImGui for.
 */
bool Initialize(GLFWwindow* window, HdEditorWindowData* windowData);

// Add function declaration for icon font initialization
void InitializeIconFont(HdEditorWindowData* windowData);

#if 0
void InitializeWindows();
#endif
// =========== Styling ===========  
void StyleColorsHydragonDark();
void StyleColorsHydragonLight();
void StyleColorsHydragonClassic();
void StyleColorsHydragonModern();

// Custom font loading
void LoadFonts(const std::string& defaultFontPath = "", float defaultFontSize = 13.0f);

// Icon font loading
void LoadIconFonts(const std::string& iconFontPath = "", float iconFontSize = 13.0f);
#if 0
// =========== Input handling ===========
/**
 * @brief Sleeps the ImGui window, pausing rendering and processing of most events.
 */
void Sleep(bool enable);
void AutoSleepAfterInactivity(float idleTimeSeconds = 60.0f);

// Utility to check if ImGui is sleeping
bool IsSleeping();

// Function to reset interaction time and wake up the application
void ResetInteractionTime();

// =========== Input handling ===========
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
#endif

// =========== Rendering ===========
/**
 * @brief Renders the ImGui user interface.
 */
void RenderHydragonEditor(HdEditorWindowData* windowData);

} // namespace hdImgui

