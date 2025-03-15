/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UIManager.h
 * @brief Wrapper for window creation, input and UI functionality.
 * 
 * ARCHITECTURAL NOTES:
 * Wrapper for window creation, input and UI functionality. Leverages glfw, Dear imgui, imgui-node-editor and other third-party libraries.
 * 
 * TODO:
 * - Change the name of the files to better reflect its broader purpose: hdEditor.h and hdEditor.cpp? HydragonEditor? MainEditor?
 * - Refactor the architecture to use classes, inheritance, encapsulation, more well-structured code, instead of namespace + free-functions.
 * - Also use classes & methods for the sub-editors.
 * - Move here the initialization, update and cleanup code blocks currently in main.cpp responsible for glfw, imgui, steps.
 */
#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_node_editor.h"
#include "imgui_node_editor_internal.h"  // Required for imgui-node-editor
#include <string>
#include <GLFW/glfw3.h>                 // Include GLFW header for GLFWwindow. Required also for imgui-node-editor
#include "NodeGraphState.h"             // Add this include

/**
 * @brief Namespace for ImGui integration functions.
 */
namespace hd {
struct EditorWindowData
{
    // Layout
    float globalWindowBgAlpha = 0.35f;
    // Font pointers are initialized as nullptr and set during UIManager::Initialize()
    // after ImGui context and ResourceManager are available
    ImFont* iconFont = nullptr;
    ImFont* defaultFont = nullptr;
    ImVec2 iconDefaultSize = ImVec2(35.0f, 35.0f);

    // Editor sleep and auto-sleep feature
    bool isSleeping = false;

    // Node Graph Editor variables
    NodeGraphState nodeGraphState;
    float nodeGraphEditor_GridOpacity = 0.1f;

    // Scripting, debugging, profiling useful info
    bool showConsoleCategories = true;
    bool showConsoleTimestamps = true;
    bool showInfoMessages = true;
    bool showWarnings = true;
    bool showErrors = true;
    bool showDebugMessages = true;

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
    bool isProcOrchestratorWindowOpen = true;

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
    bool isAITaskWindowOpen = true;

    // UI-only temporary states
    bool tempBool = false;  // Temporary boolean for UI prototyping
    float tempFloat = 0.0f; // Temporary float for UI prototyping
};

// =========== Initialization & Cleanup ============
/** 
 * @brief Initializes UIManager.
 * @param window The GLFW window to initialize ImGui for.
 */
bool Initialize(GLFWwindow* window, EditorWindowData* windowData);
void Cleanup();

/**
 * @brief Initializes ImGui.
 * @param window The GLFW window to initialize ImGui for.
 */
void InitializeImgui(GLFWwindow* window);

/**
 * @brief Initializes the icon font.
 */
void InitializeIconFont(EditorWindowData* windowData);

/**
 * @brief Initializes the imgui-node-editor context, load and setup configuration.
 * imgui-node-editor is a third-party extension, external to imgui's repo.
 * @param windowData Editor window data containing global settings
 */
void InitializeImguiNodeEditor(EditorWindowData* windowData);

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
void RenderHydragonEditor(EditorWindowData* windowData);

} // namespace hd

