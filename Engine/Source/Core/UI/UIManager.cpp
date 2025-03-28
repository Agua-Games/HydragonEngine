/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file UIManager.cpp
 * @brief Wrapper for window creation, input and UI functionality.
 * 
 * ARCHITECTURAL NOTES:
 * - Wrapper for window creation, input and UI functionality. Leverages glfw, Dear imgui, imgui-node-editor and other third-party libraries.
 * - Unfortunately, imgui uses capital letter for function names. This is why you may see mixed capitalization in this file and other files which
 * use imgui.
 * 
 */
#pragma once
#include <chrono>
#include <iostream>
#include <stdio.h>          // To use printf, fprintf
#include <stdlib.h>         // To use abort
#include <GLFW/glfw3.h>
#include <imgui_internal.h>
#include <imgui_impl_glfw.h>
#include <imgui.h>
#include "imgui_node_editor.h"
namespace nodeEd = ax::NodeEditor;

#include "UIManager.h"
#include "ResourceManager.h"
#include "MainMenuBar.h"
#include "TopToolbar.h"
#include "LeftToolbar.h"
#include "RightToolbar.h"
#include "BottomToolbar.h"
#include "SceneGraphEditor.h"
#include "NodeGraphEditor.h"
#include "ScriptEditor.h"
#include "ScriptsPalette.h"
#include "CommandsPalette.h"
#include "ConsoleEditor.h"
#include "AgentsEditor.h"
#include "Viewport3D.h"
#include "Viewport3DTools.h"
#include "Viewport2D.h"
#include "Viewport2DTools.h"
#include "PropertyEditor.h"
#include "AssetEditor.h"
#include "LightingEditor.h"
#include "PhysicsEditor.h"
#include "BottomStatusBar.h"
#include "FileExplorer.h"
#include "ProceduralManagerEditor.h"
#include "Profiler.h"
#include "StreamingEditor.h"
#include "ImageTools.h"
#include "AudioEditor.h"
#include "MontageEditor.h"
#include "FontEditor.h"
#include "PluginEditor.h"
#include "ExtensionsEditor.h"
#include "MacrosEditor.h"
#include "DramaEditor.h"
#include "ChimeraPipelineEditor.h"
#include "UIEditor.h"
#include "NetworkingEditor.h"
#include "PerformanceScalabilityEditor.h"
#include "ReflectionEditor.h"
#include "CollaborationEditor.h"
#include "CommunityEditor.h"
#include "InputEditor.h"
#include "PropertiesMatrixEditor.h"
#include "LocalizationEditor.h"
#include "SettingsEditor.h"
#include "MeshEditor.h"
#include "MonetizationEditor.h"
#include "ProjectInsightsEditor.h"
#include "VolumeEditor.h"
#include "PresetEditor.h"
#include "DebugEditor.h"
#include "AITaskEditor.h"

namespace hd {

// Static instance of window data
EditorWindowData windowData;
// Declare imgui-node-editor context
nodeEd::EditorContext* nodeEditorContext = nullptr;
nodeEd::Config config;
// Static variables for sleep/idle functionality
static std::chrono::steady_clock::time_point s_lastInteractionTime;
// rendering vars

// =========== Initialization ===========
void initializeWindows(EditorWindowData* windowData){
    #if 0
    // TODO: Refactor all these initializations to a class-based approach.
    Editor::initialize();
    MainMenu::initialize();
    TopToolbar::initialize();
    LeftToolbar::initialize();
    RighToolbar::initialize();
    BottomToolbar::initialize();
    ScriptsPalette::initialize();
    CommandsPalette::initialize();
    ConsoleEditor::initialize();
    ScriptEditor::initialize();
    Viewport3D::initialize();
    Viewport3DTools::initialize();
    Viewport2D::initialize();
    Viewport2DTools::initialize();
    ImageTools::initialize();
    SceneGraphEditor::initialize();
    #endif
    hd::initializeNodeGraphEditor(windowData);     // To be changed to class-based approach. Methods.
    #if 0
    MacroEditor::initialize();
    PropertyEditor::initialize();
    AssetEditor::initialize();
    ChimeraPipelineEditor::initialize();
    AgentsEditor::initialize();
    LightingEditor::initialize();
    PhysicsEditor::initialize();
    BottomStatusBar::initialize();
    FileExplorer::initialize();
    ProceduralManager::initialize();
    Profiler::initialize();
    StreamingEditor::initialize();
    ImageTools::initialize();
    AudioEditor::initialize();
    MontageEditor::initialize();
    FontEditor::initialize();
    PluginEditor::initialize();
    ExtensionsEditor::initialize();
    MacrosEditor::initialize();
    DramaEditor::initialize();
    UIEditor::initialize();
    NetworkingEditor::initialize();
    PerformanceScalabilityEditor::initialize();
    ReflectionEditor::initialize();
    CollaborationEditor::initialize();
    CommunityEditor::initialize();
    MonetizationEditor::initialize();
    ProjectInsightsEditor::initialize();
    InputEditor::initialize();
    PropertiesMatrixEditor::initialize();
    LocalizationEditor::initialize();
    SetttingsEditor::initialize();
    MeshEditor::initialize();
    VolumeEditor::initialize();
    PresetsEditor::initialize();
    DebugEditor::initialize();
    AITaskEditor::initialize();
    #endif
}

bool initialize(GLFWwindow* window, EditorWindowData* windowData) {
    if (!window) return false;

    //initializeImgui(window);                         // imgui is the main UI library
    //initializeImguiNodeEditor(windowData);           // imgui-node-editor is an imgui extension
    initializeIconFont(windowData);                  // Initialize icon font

    // Set default style
    styleColorsHydragonDark();

    // === Idle sleep ===
    // Initialize last interaction time - used for idle sleep
    s_lastInteractionTime = std::chrono::steady_clock::now();
    
    // === Windows, sub-editors ===
    // Initialize each ImGui window (Editor windows, sub-editors, etc.)
    initializeWindows(windowData);

    return true;
}

void cleanup() {
    // Cleanup imgui-node-editor context (before imgui). This is the reverse of initialization.
    if (nodeEditorContext) {
        nodeEd::DestroyEditor(nodeEditorContext);
        nodeEditorContext = nullptr;
    }

    // Destroy ImGui context
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void initializeImgui(GLFWwindow* window) {
    if (!window) return;

    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;         // Enable Docking
    io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;       // Enable Multi-Viewport / Platform Windows


    // Initialize ImGui GLFW and Vulkan implementation
    ImGui_ImplGlfw_InitForVulkan(window, true);
}

void initializeIconFont(EditorWindowData* windowData) {
    if (!windowData) return;

    // Get the singleton instance of ResourceManager
    auto& resourceManager = hd::ResourceManager::getInstance();
    // Set the icon font pointer in the window data
    windowData->iconFont = resourceManager.getIconFont();
}

void initializeImguiNodeEditor(EditorWindowData* windowData) {
    if (!windowData) return;

    config.SettingsFile = "NodeEditorSettings.json"; // Optional: save layout to file

    // Make sure navigation is enabled
    config.NavigateButtonIndex = ImGuiMouseButton_Middle;  // Middle mouse button for panning
    config.DragButtonIndex = ImGuiMouseButton_Left;        // Left mouse button for dragging nodes
    nodeEditorContext = nodeEd::CreateEditor(&config);
}

// =========== Styling ===========
void styleColorsHydragonDark(){
    // Start with ImGui's default dark style
    ImGui::StyleColorsDark();

    // Get references to the style structures
    ImGuiStyle& style = ImGui::GetStyle();

    // Need to figure out how to use ensureNodeEditorContext() here (the variables and functions there
    // in NodeGraphEditor.cpp are static)
    //nodeEd::Style& nodesStyle = nodeEd::GetStyle();

    // Customize spacing and rounding
    style.WindowPadding = ImVec2(15.0f, 15.0f);  // Padding within windows
    style.FramePadding = ImVec2(4.0f, 4.0f);   // Padding within frames (e.g., buttons)
    style.ItemSpacing = ImVec2(8.0f, 4.0f);    // Spacing between widgets
    style.ItemInnerSpacing = ImVec2(4.0f, 4.0f); // Spacing within widgets (e.g., text and icons)
    style.ScrollbarSize = 14.0f;
    style.GrabMinSize = 8.0f;                   // Size of the slider grab

    style.WindowRounding = 11.0f;               // Rounding of window corners
    style.ChildRounding = 11.0f;               // Rounding of child  corners
    style.FrameRounding = 7.0f;                // Rounding of frame corners (e.g., buttons)
    style.PopupRounding = 0.0;
    style.ScrollbarRounding = 10.0f;  
    style.GrabRounding = 7.0f;                 // Rounding of slider grabs
    style.TabRounding = 7.0f;

    // Customize colors
    style.Colors[ImGuiCol_Text]                   = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_WindowBg]               = ImVec4(0.21f, 0.22f, 0.22f, 1.00f);
    style.Colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_Border]                 = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
    style.Colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_FrameBg]                = ImVec4(0.42f, 0.43f, 0.44f, 0.54f);
    style.Colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.86f, 0.92f, 1.00f, 0.40f);
    style.Colors[ImGuiCol_FrameBgActive]          = ImVec4(0.80f, 0.89f, 1.00f, 0.43f);
    style.Colors[ImGuiCol_TitleBg]                = ImVec4(0.24f, 0.24f, 0.24f, 0.32f);
    style.Colors[ImGuiCol_TitleBgActive]          = ImVec4(0.23f, 0.24f, 0.25f, 0.80f);
    style.Colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.40f, 0.41f, 0.43f, 0.54f);
    style.Colors[ImGuiCol_MenuBarBg]              = ImVec4(0.27f, 0.29f, 0.31f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.12f, 0.12f, 0.12f, 0.53f);
    style.Colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.23f, 0.24f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
    style.Colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
    style.Colors[ImGuiCol_CheckMark]              = ImVec4(0.88f, 0.88f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_SliderGrab]             = ImVec4(0.74f, 0.74f, 0.74f, 1.00f);
    style.Colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.49f, 0.61f, 0.75f, 1.00f);
    style.Colors[ImGuiCol_Button]                 = ImVec4(0.44f, 0.46f, 0.49f, 0.40f);
    style.Colors[ImGuiCol_ButtonHovered]          = ImVec4(0.37f, 0.42f, 0.48f, 1.00f);
    style.Colors[ImGuiCol_ButtonActive]           = ImVec4(0.69f, 0.79f, 0.88f, 1.00f);
    style.Colors[ImGuiCol_Header]                 = ImVec4(0.36f, 0.40f, 0.43f, 0.33f);
    style.Colors[ImGuiCol_HeaderHovered]          = ImVec4(0.48f, 0.52f, 0.56f, 0.80f);
    style.Colors[ImGuiCol_HeaderActive]           = ImVec4(0.47f, 0.51f, 0.56f, 0.80f);
    style.Colors[ImGuiCol_Separator]              = ImVec4(0.51f, 0.51f, 0.54f, 0.50f);
    style.Colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.48f, 0.54f, 0.61f, 0.78f);
    style.Colors[ImGuiCol_SeparatorActive]        = ImVec4(0.50f, 0.58f, 0.66f, 1.00f);
    style.Colors[ImGuiCol_ResizeGrip]             = ImVec4(0.26f, 0.59f, 0.98f, 0.20f);
    style.Colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.26f, 0.59f, 0.98f, 0.67f);
    style.Colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.26f, 0.59f, 0.98f, 0.95f);
    style.Colors[ImGuiCol_TabHovered]             = ImVec4(0.47f, 0.51f, 0.55f, 1.00f);
    style.Colors[ImGuiCol_Tab]                    = ImVec4(0.35f, 0.37f, 0.37f, 1.00f);
    style.Colors[ImGuiCol_TabSelected]            = ImVec4(0.41f, 0.45f, 0.50f, 1.00f);
    style.Colors[ImGuiCol_TabSelectedOverline]    = ImVec4(0.53f, 0.56f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_TabDimmed]              = ImVec4(0.30f, 0.31f, 0.33f, 1.00f);
    style.Colors[ImGuiCol_TabDimmedSelected]      = ImVec4(0.37f, 0.39f, 0.42f, 1.00f);
    style.Colors[ImGuiCol_TabDimmedSelectedOverline]  = ImVec4(0.50f, 0.50f, 0.50f, 0.00f);
    style.Colors[ImGuiCol_DockingPreview]         = ImVec4(0.69f, 0.69f, 0.69f, 0.23f);
    style.Colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
    style.Colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
    style.Colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
    style.Colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
    style.Colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
    style.Colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
    style.Colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    style.Colors[ImGuiCol_TextLink]               = ImVec4(0.29f, 0.51f, 0.78f, 1.00f);
    style.Colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
    style.Colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
    style.Colors[ImGuiCol_NavCursor]              = ImVec4(0.64f, 0.75f, 0.89f, 1.00f);
    style.Colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
    style.Colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
    style.Colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);

    // TO-DO: Find a way to move NodeGraphEditor's style here.
}

void StyleColorsHydragonLight() {
    ImGui::StyleColorsLight();  // Start with ImGui's default light style

    // Get a reference to the style structure
    ImGuiStyle& style = ImGui::GetStyle();

    // Customize colors
    style.Colors[ImGuiCol_TitleBg] =                ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] =          ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
    // Add more customizations here...
}

void StyleColorsHydragonClassic() {
    ImGui::StyleColorsClassic();  // Start with ImGui's default classic style

    // Get a reference to the style structure
    ImGuiStyle& style = ImGui::GetStyle();

    // Customize colors
    style.Colors[ImGuiCol_TitleBg] =                ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
    style.Colors[ImGuiCol_TitleBgActive] =          ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
    // Add more customizations here...
}

void StyleColorsHydragonModern() {
    // Start with a base style (e.g., dark)
    styleColorsHydragonDark();

    // Get a reference to the style structure
    ImGuiStyle& style = ImGui::GetStyle();

    // Adjust rounding values for a more modern look
    style.FrameRounding = 4.0f;
    style.GrabRounding = 4.0f;
    style.WindowRounding = 8.0f;
    // Add more customizations here...
}

void LoadFonts(const std::string& defaultFontPath, float defaultFontSize) {
    ImGuiIO& io = ImGui::GetIO();

    // Load default font
    if (!defaultFontPath.empty()) {
        io.Fonts->AddFontFromFileTTF(defaultFontPath.c_str(), defaultFontSize);
    } else {
        io.Fonts->AddFontDefault();  // Use ImGui's default font
    }

    // Build font atlas
    io.Fonts->Build();
}
#if 0
void loadIconFonts(const std::string& iconFontPath, float iconFontSize) {
    ImGuiIO& io = ImGui::GetIO();

    // Load icon font (e.g., FontAwesome)
    if (!iconFontPath.empty()) {
        static const ImWchar iconRanges[] = { 0xf000, 0xf3ff, 0 };  // Example range for FontAwesome
        io.Fonts->AddFontFromFileTTF(iconFontPath.c_str(), iconFontSize, nullptr, iconRanges);
    }

    // Build font atlas
    io.Fonts->Build();
}

// =========== Input handling ===========
void sleep(EditorWindowData.enable) {
    s_isSleeping = enable;
}

void autoSleepAfterInactivity(float idleTimeSeconds) {
    auto now = std::chrono::steady_clock::now();
    auto idleDuration = std::chrono::duration_cast<std::chrono::seconds>(now - s_lastInteractionTime).count();

    if (idleDuration >= idleTimeSeconds) {
        sleep(true);
    }
}

bool isSleeping() {
    return s_isSleeping;
}

// Function to reset interaction time and wake up the application
void resetInteractionTime() {
    s_lastInteractionTime = std::chrono::steady_clock::now();
    if (s_isSleeping) {
        sleep(false);  // Wake up
    }
}

// GLFW key callback
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        resetInteractionTime();
    }
}

// GLFW mouse button callback
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        resetInteractionTime();
    }
}
#endif
// =========== Rendering ===================
void renderHydragonEditor(EditorWindowData* windowData) {
    // Create docking space
    ImGui::DockSpaceOverViewport(ImGui::GetID("MainDockSpace"));

    // === Render each window ===
    // Main Menu Bar
    if (windowData->isMainMenuBarWindowOpen) { hd::showMainMenuBar(&windowData->isMainMenuBarWindowOpen, windowData); }
    // Top Toolbar
    if (windowData->isTopToolbarWindowOpen) { hd::showTopToolbar(&windowData->isTopToolbarWindowOpen, windowData); }
    // Left Toolbar
    if (windowData->isLeftToolbarWindowOpen) { hd::showLeftToolbar(&windowData->isLeftToolbarWindowOpen, windowData); }
    // Right Toolbar
    if (windowData->isRightToolbarWindowOpen) { hd::showRightToolbar(&windowData->isRightToolbarWindowOpen, windowData); }
    // Bottom Toolbar
    if (windowData->isBottomToolbarWindowOpen) { hd::showBottomToolbar(&windowData->isBottomToolbarWindowOpen, windowData); }
    // Scripts Palette
    if (windowData->isScriptsPaletteWindowOpen) { hd::showScriptsPalette(&windowData->isScriptsPaletteWindowOpen, windowData); }
    // Commands Palette
    if (windowData->isCommandsPaletteWindowOpen) { hd::showCommandsPalette(&windowData->isCommandsPaletteWindowOpen, windowData); }
    // Console Editor
    if (windowData->isConsoleWindowOpen) { hd::showConsoleEditor(&windowData->isConsoleWindowOpen, windowData); }
    // Script Editor
    if (windowData->isScriptWindowOpen) { hd::showScriptEditor(&windowData->isScriptWindowOpen, windowData); }
    // Viewport 3D
    if (windowData->isViewport3DWindowOpen) { hd::showViewport3D(&windowData->isViewport3DWindowOpen, windowData); }
    // Viewport 3D Tools
    if (windowData->isViewport3DToolsWindowOpen) { hd::showViewport3DTools(&windowData->isViewport3DToolsWindowOpen, windowData); }
    // Viewport 2D
    if (windowData->isViewport2DWindowOpen) { hd::showViewport2D(&windowData->isViewport2DWindowOpen, windowData); }
    // Viewport 2D Tools
    if (windowData->isViewport2DToolsWindowOpen) { hd::showViewport2DTools(&windowData->isViewport2DToolsWindowOpen, windowData); }
    // Scene Graph Editor
    if (windowData->isSceneGraphWindowOpen) { hd::showSceneGraphEditor(&windowData->isSceneGraphWindowOpen, windowData); }
    // Node Graph Editor
    if (windowData->isNodeGraphWindowOpen) { hd::showNodeGraphEditor(&windowData->isNodeGraphWindowOpen, windowData); }
    // Properties Editor
    if (windowData->isPropertiesWindowOpen) { hd::showPropertyEditor(&windowData->isPropertiesWindowOpen, windowData); }
    // Asset Manager
    if (windowData->isAssetEditorWindowOpen) { hd::showAssetEditor(&windowData->isAssetEditorWindowOpen, windowData); }
    // Chimera Pipeline Editor
    if (windowData->isChimeraPipelineWindowOpen) { hd::showChimeraPipelineEditor(&windowData->isChimeraPipelineWindowOpen, windowData); }
    // Agents Editor
    if (windowData->isAgentsWindowOpen) { hd::showAgentsEditor(&windowData->isAgentsWindowOpen, windowData); }
    // Lighting Editor
    if (windowData->isLightingWindowOpen) { hd::showLightingEditor(&windowData->isLightingWindowOpen, windowData); }
    // Physics Editor
    if (windowData->isPhysicsWindowOpen) { hd::showPhysicsEditor(&windowData->isPhysicsWindowOpen, windowData); }
    // Bottom status bar
    if (windowData->isBottomStatusBarWindowOpen) { hd::showBottomStatusBar(&windowData->isBottomStatusBarWindowOpen, windowData); }
    // File Explorer
    if (windowData->isFileExplorerWindowOpen) { hd::showFileExplorer(&windowData->isFileExplorerWindowOpen, windowData); }
    // Procedural (Pattern) Orchestrator
    if (windowData->isProceduralManagerWindowOpen) { hd::showProceduralManager(&windowData->isProceduralManagerWindowOpen, windowData); }
    // Profiler
    if (windowData->isProfilerWindowOpen) { hd::showProfiler(&windowData->isProfilerWindowOpen, windowData); }
    // Streaming Editor
    if (windowData->isStreamingWindowOpen) { hd::showStreamingEditor(&windowData->isStreamingWindowOpen, windowData); }
    // Image Editor
    // Used to display and edit textures, texture settings, edit UVs, packed textures, apply adjustments,
    // configure procedurals, use AI-assisted texture generation etc.
    if (windowData->isImageWindowOpen) { hd::showImageTools(&windowData->isImageWindowOpen, windowData); }
    // Audio Editor
    if (windowData->isAudioWindowOpen) { hd::showAudioEditor(&windowData->isAudioWindowOpen, windowData); }
    // Montage Editor
    // Timeline with tracks, blending, transitions, etc. Used to compose animations, cutscenes, video clips, 
    // audio clips, images and other time varying media.
    // Whereas DCC apps usually rely on a simple timeline by default, Hydragon uses a more advanced montage editor,
    // with two visualization modes: collapsed and expanded (defaults to expanded).
    if (windowData->isMontageWindowOpen) { hd::showMontageEditor(&windowData->isMontageWindowOpen, windowData); }
    // Font Editor
    if (windowData->isFontWindowOpen) { hd::showFontEditor(&windowData->isFontWindowOpen, windowData); }
    // Plugin Editor
    if (windowData->isPluginWindowOpen) { hd::showPluginEditor(&windowData->isPluginWindowOpen, windowData); }
    // Extensions Editor
    if (windowData->isExtensionsWindowOpen) { hd::showExtensionsEditor(&windowData->isExtensionsWindowOpen, windowData); }
    // Macros Editor
    if (windowData->isMacrosWindowOpen) { hd::showMacrosEditor(&windowData->isMacrosWindowOpen, windowData); }
    // Drama Editor
    if (windowData->isDramaWindowOpen) { hd::showDramaEditor(&windowData->isDramaWindowOpen, windowData); }
    // UI Editor
    if (windowData->isUIEditorWindowOpen) { hd::showUIEditor(&windowData->isUIEditorWindowOpen, windowData); }
    // Networking Editor
    if (windowData->isNetworkingWindowOpen) { hd::showNetworkingEditor(&windowData->isNetworkingWindowOpen, windowData); }
    // Performance Scalability Editor
    if (windowData->isPerformanceScalabilityWindowOpen) { hd::showPerformanceScalabilityEditor(&windowData->isPerformanceScalabilityWindowOpen, windowData); }
    // Reflection Editor
    if (windowData->isReflectionWindowOpen) { hd::showReflectionEditor(&windowData->isReflectionWindowOpen, windowData); }
    // Collaboration Editor
    if (windowData->isCollaborationWindowOpen) { hd::showCollaborationEditor(&windowData->isCollaborationWindowOpen, windowData); }
    // Community Editor
    if (windowData->isCommunityWindowOpen) { hd::showCommunityEditor(&windowData->isCommunityWindowOpen, windowData); }
    // Monetization Editor
    if (windowData->isMonetizationWindowOpen) { hd::showMonetizationEditor(&windowData->isMonetizationWindowOpen, windowData); }
    // Project Insights Editor
    if (windowData->isProjectInsightsWindowOpen) { hd::showProjectInsightsEditor(&windowData->isProjectInsightsWindowOpen, windowData); }
    // Input Editor
    if (windowData->isInputWindowOpen) { hd::showInputEditor(&windowData->isInputWindowOpen, windowData); }
    // Properties Matrix Editor
    if (windowData->isPropertiesMatrixWindowOpen) { hd::showPropertiesMatrixEditor(&windowData->isPropertiesMatrixWindowOpen, windowData); }
    // Localization Editor
    if (windowData->isLocalizationWindowOpen) { hd::showLocalizationEditor(&windowData->isLocalizationWindowOpen, windowData); }
    // Settings Editor
    if (windowData->isSettingsWindowOpen) { hd::showSettingsEditor(&windowData->isSettingsWindowOpen, windowData); }
    // Mesh Editor
    if (windowData->isMeshWindowOpen) { hd::showMeshEditor(&windowData->isMeshWindowOpen, windowData); }
    // Volume Editor
    if (windowData->isVolumeWindowOpen) { hd::showVolumeEditor(&windowData->isVolumeWindowOpen, windowData); }
    // Preset Editor
    if (windowData->isPresetsWindowOpen) { hd::showPresetEditor(&windowData->isPresetsWindowOpen, windowData); }
    // Debug Editor
    if (windowData->isDebugWindowOpen) { hd::showDebugEditor(&windowData->isDebugWindowOpen, windowData); }
    // AI Task Editor
    if (windowData->isAITaskWindowOpen) { hd::showAITaskEditor(&windowData->isAITaskWindowOpen, windowData); }

    // === Temporary, for referencing the components' names in imgui code files ===
    ImGui::ShowDemoWindow();
}

} // namespace hd
