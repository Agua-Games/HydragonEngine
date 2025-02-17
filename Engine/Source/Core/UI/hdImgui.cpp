/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <chrono>
#include <iostream>
#include <stdio.h>          // printf, fprintf
#include <stdlib.h>         // abort
#include <GLFW/glfw3.h>     // Include GLFW header for GLFWwindow
#include <imgui_internal.h>  // For internal ImGui functions if needed
#include <imgui.h>

#include "hdImgui.h"
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
#include "AssetManager.h"
#include "LightingEditor.h"
#include "PhysicsEditor.h"
#include "BottomStatusBar.h"
#include "FileExplorer.h"
#include "PatternOrchestrator.h"
#include "Profiler.h"
#include "StreamingEditor.h"
#include "ImageTools.h"
#include "AudioEditor.h"
#include "MontageEditor.h"
#include "TextEditor.h"
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

namespace hdImgui {

// Static instance of window data
static HdEditorWindowData hdEditorWindowData;
// Static variables for sleep/idle functionality
static std::chrono::steady_clock::time_point s_lastInteractionTime;
// rendering vars


// =========== Initialization ===========
void InitializeWindows(){
    #if 0
    hdEditor::Initialize();
    hdTopToolbar::Initialize();
    hdLeftToolbar::Initialize();
    hdRightToolbar::Initialize();
    hdBottomToolbar::Initialize();
    hdCommandsConsole::Initialize();
    hdScriptsPalette::Initialize();
    hdViewport3D::Initialize();
    hdViewport2D::Initialize();
    hdImageEditor::Initialize();
    hdSceneGraphEditor::Initialize();
    hdScriptEditor::Initialize();
    hdMacroEditor::Initialize();
    hdPropertyEditor::Initialize();
    hdSetttingsEditor::Initialize();
    #endif
}
// =========== Styling ===========
void StyleColorsHydragonDark(){
    // Start with ImGui's default dark style
    ImGui::StyleColorsDark();

    // Get a referenc to the style structure
    ImGuiStyle& style = ImGui::GetStyle();

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
    style.Colors[ImGuiCol_Text]                   = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
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
    StyleColorsHydragonDark();

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
void LoadIconFonts(const std::string& iconFontPath, float iconFontSize) {
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
void Sleep(hdEditorWindowData.enable) {
    s_isSleeping = enable;
}

void AutoSleepAfterInactivity(float idleTimeSeconds) {
    auto now = std::chrono::steady_clock::now();
    auto idleDuration = std::chrono::duration_cast<std::chrono::seconds>(now - s_lastInteractionTime).count();

    if (idleDuration >= idleTimeSeconds) {
        Sleep(true);
    }
}

hdEditorWindowData.IsSleeping() {
    return s_isSleeping;
}

// Function to reset interaction time and wake up the application
void ResetInteractionTime() {
    s_lastInteractionTime = std::chrono::steady_clock::now();
    if (s_isSleeping) {
        Sleep(false);  // Wake up
    }
}

// GLFW key callback
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS || action == GLFW_REPEAT) {
        ResetInteractionTime();
    }
}

// GLFW mouse button callback
void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (action == GLFW_PRESS) {
        ResetInteractionTime();
    }
}
#endif
// =========== Rendering ===================
void InitializeIconFont(HdEditorWindowData* windowData) {
    if (!windowData) return;
    
    auto& resourceManager = hd::ResourceManager::GetInstance();
    windowData->iconFont = resourceManager.GetIconFont();
}

bool Initialize(GLFWwindow* window, HdEditorWindowData* windowData) {
    if (!window) return false;

    #if 0
    // Initialize ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    // Initialize ImGui GLFW and Vulkan implementation
    ImGui_ImplGlfw_InitForVulkan(window, true);
    #endif

    // Initialize icon font
    auto& resourceManager = hd::ResourceManager::GetInstance();
    windowData->iconFont = resourceManager.GetIconFont();

    // Set default style
    StyleColorsHydragonDark();

    // === Idle sleep ===
    // Initialize last interaction time - used for idle sleep
    s_lastInteractionTime = std::chrono::steady_clock::now();
    
    // === Windows, sub-editors ===
    // Initialize each ImGui window (Editor windows, sub-editors, etc.)
    InitializeWindows();

    return true;
}
void RenderHydragonEditor(HdEditorWindowData* windowData) {
    // Create docking space
    ImGui::DockSpaceOverViewport(ImGui::GetID("MainDockSpace"));

    // === Render each window ===
    // Main Menu Bar
    if (windowData->isMainMenuBarWindowOpen) { hdImgui::ShowMainMenuBar(&windowData->isMainMenuBarWindowOpen, windowData); }
    // Top Toolbar
    if (windowData->isTopToolbarWindowOpen) { hdImgui::ShowTopToolbar(&windowData->isTopToolbarWindowOpen, windowData); }
    // Left Toolbar
    if (windowData->isLeftToolbarWindowOpen) { hdImgui::ShowLeftToolbar(&windowData->isLeftToolbarWindowOpen, windowData); }
    // Right Toolbar
    if (windowData->isRightToolbarWindowOpen) { hdImgui::ShowRightToolbar(&windowData->isRightToolbarWindowOpen, windowData); }
    // Bottom Toolbar
    if (windowData->isBottomToolbarWindowOpen) { hdImgui::ShowBottomToolbar(&windowData->isBottomToolbarWindowOpen, windowData); }
    // Scripts Palette
    if (windowData->isScriptsPaletteWindowOpen) { hdImgui::ShowScriptsPalette(&windowData->isScriptsPaletteWindowOpen, windowData); }
    // Commands Palette
    if (windowData->isCommandsPaletteWindowOpen) { hdImgui::ShowCommandsPalette(&windowData->isCommandsPaletteWindowOpen, windowData); }
    // Console Editor
    if (windowData->isConsoleWindowOpen) { hdImgui::ShowConsole(&windowData->isConsoleWindowOpen, windowData); }
    // Script Editor
    if (windowData->isScriptWindowOpen) { hdImgui::ShowScriptEditor(&windowData->isScriptWindowOpen, windowData); }
    // Viewport 3D
    if (windowData->isViewport3DWindowOpen) { hdImgui::ShowViewport3D(&windowData->isViewport3DWindowOpen, windowData); }
    // Viewport 3D Tools
    if (windowData->isViewport3DToolsWindowOpen) { hdImgui::ShowViewport3DTools(&windowData->isViewport3DToolsWindowOpen, windowData); }
    // Viewport 2D
    if (windowData->isViewport2DWindowOpen) { hdImgui::ShowViewport2D(&windowData->isViewport2DWindowOpen, windowData); }
    // Viewport 2D Tools
    if (windowData->isViewport2DToolsWindowOpen) { hdImgui::ShowViewport2DTools(&windowData->isViewport2DToolsWindowOpen, windowData); }
    // Scene Graph Editor
    if (windowData->isSceneGraphWindowOpen) { hdImgui::ShowSceneGraphEditor(&windowData->isSceneGraphWindowOpen, windowData); }
    // Node Graph Editor
    if (windowData->isNodeGraphWindowOpen) { hdImgui::ShowNodeGraphEditor(&windowData->isNodeGraphWindowOpen, windowData); }
    // Properties Editor
    if (windowData->isPropertiesWindowOpen) { hdImgui::ShowPropertyEditor(&windowData->isPropertiesWindowOpen, windowData); }
    // Asset Manager
    if (windowData->isAssetManagerWindowOpen) { hdImgui::ShowAssetManager(&windowData->isAssetManagerWindowOpen, windowData); }
    // Chimera Pipeline Editor
    if (windowData->isChimeraPipelineWindowOpen) { hdImgui::ShowChimeraPipelineEditor(&windowData->isChimeraPipelineWindowOpen, windowData); }
    // Agents Editor
    if (windowData->isAgentsWindowOpen) { hdImgui::ShowAgentsEditor(&windowData->isAgentsWindowOpen, windowData); }
    // Lighting Editor
    if (windowData->isLightingWindowOpen) { hdImgui::ShowLightingEditor(&windowData->isLightingWindowOpen, windowData); }
    // Physics Editor
    if (windowData->isPhysicsWindowOpen) { hdImgui::ShowPhysicsEditor(&windowData->isPhysicsWindowOpen, windowData); }
    // Bottom status bar
    if (windowData->isBottomStatusBarWindowOpen) { hdImgui::ShowBottomStatusBar(&windowData->isBottomStatusBarWindowOpen, windowData); }
    // File Explorer
    if (windowData->isFileExplorerWindowOpen) { hdImgui::ShowFileExplorer(&windowData->isFileExplorerWindowOpen, windowData); }
    // Pattern Orchestrator
    if (windowData->isPatternOrchestratorWindowOpen) { hdImgui::ShowPatternOrchestrator(&windowData->isPatternOrchestratorWindowOpen, windowData); }
    // Profiler
    if (windowData->isProfilerWindowOpen) { hdImgui::ShowProfiler(&windowData->isProfilerWindowOpen, windowData); }
    // Streaming Editor
    if (windowData->isStreamingWindowOpen) { hdImgui::ShowStreamingEditor(&windowData->isStreamingWindowOpen, windowData); }
    // Image Editor
    // Used to display and edit textures, texture settings, edit UVs, packed textures, apply adjustments,
    // configure procedurals, use AI-assisted texture generation etc.
    if (windowData->isImageWindowOpen) { hdImgui::ShowImageTools(&windowData->isImageWindowOpen, windowData); }
    // Audio Editor
    if (windowData->isAudioWindowOpen) { hdImgui::ShowAudioEditor(&windowData->isAudioWindowOpen, windowData); }
    // Montage Editor
    // Timeline with tracks, blending, transitions, etc. Used to compose animations, cutscenes, video clips, 
    // audio clips, images and other time varying media.
    // Whereas DCC apps usually rely on a simple timeline by default, Hydragon uses a more advanced montage editor,
    // with two visualization modes: collapsed and expanded (defaults to expanded).
    if (windowData->isMontageWindowOpen) { hdImgui::ShowMontageEditor(&windowData->isMontageWindowOpen, windowData); }
    // Text Editor
    if (windowData->isTextWindowOpen) { hdImgui::ShowTextEditor(&windowData->isTextWindowOpen, windowData); }
    // Font Editor
    if (windowData->isFontWindowOpen) { hdImgui::ShowFontEditor(&windowData->isFontWindowOpen, windowData); }
    // Plugin Editor
    if (windowData->isPluginWindowOpen) { hdImgui::ShowPluginEditor(&windowData->isPluginWindowOpen, windowData); }
    // Extensions Editor
    if (windowData->isExtensionsWindowOpen) { hdImgui::ShowExtensionsEditor(&windowData->isExtensionsWindowOpen, windowData); }
    // Macros Editor
    if (windowData->isMacrosWindowOpen) { hdImgui::ShowMacrosEditor(&windowData->isMacrosWindowOpen, windowData); }
    // Drama Editor
    if (windowData->isDramaWindowOpen) { hdImgui::ShowDramaEditor(&windowData->isDramaWindowOpen, windowData); }
    // UI Editor
    if (windowData->isUIEditorWindowOpen) { hdImgui::ShowUIEditor(&windowData->isUIEditorWindowOpen, windowData); }
    // Networking Editor
    if (windowData->isNetworkingWindowOpen) { hdImgui::ShowNetworkingEditor(&windowData->isNetworkingWindowOpen, windowData); }
    // Performance Scalability Editor
    if (windowData->isPerformanceScalabilityWindowOpen) { hdImgui::ShowPerformanceScalabilityEditor(&windowData->isPerformanceScalabilityWindowOpen, windowData); }
    // Reflection Editor
    if (windowData->isReflectionWindowOpen) { hdImgui::ShowReflectionEditor(&windowData->isReflectionWindowOpen, windowData); }
    // Collaboration Editor
    if (windowData->isCollaborationWindowOpen) { hdImgui::ShowCollaborationEditor(&windowData->isCollaborationWindowOpen, windowData); }
    // Community Editor
    if (windowData->isCommunityWindowOpen) { hdImgui::ShowCommunityEditor(&windowData->isCommunityWindowOpen, windowData); }
    // Monetization Editor
    if (windowData->isMonetizationWindowOpen) { hdImgui::ShowMonetizationEditor(&windowData->isMonetizationWindowOpen, windowData); }
    // Project Insights Editor
    if (windowData->isProjectInsightsWindowOpen) { hdImgui::ShowProjectInsightsEditor(&windowData->isProjectInsightsWindowOpen, windowData); }
    // Input Editor
    if (windowData->isInputWindowOpen) { hdImgui::ShowInputEditor(&windowData->isInputWindowOpen, windowData); }
    // Properties Matrix Editor
    if (windowData->isPropertiesMatrixWindowOpen) { hdImgui::ShowPropertiesMatrixEditor(&windowData->isPropertiesMatrixWindowOpen, windowData); }
    // Localization Editor
    if (windowData->isLocalizationWindowOpen) { hdImgui::ShowLocalizationEditor(&windowData->isLocalizationWindowOpen, windowData); }
    // Settings Editor
    if (windowData->isSettingsWindowOpen) { hdImgui::ShowSettingsEditor(&windowData->isSettingsWindowOpen, windowData); }
    // Mesh Editor
    if (windowData->isMeshWindowOpen) { hdImgui::ShowMeshEditor(&windowData->isMeshWindowOpen, windowData); }
    // Volume Editor
    if (windowData->isVolumeWindowOpen) { hdImgui::ShowVolumeEditor(&windowData->isVolumeWindowOpen, windowData); }
    // Preset Editor
    if (windowData->isPresetsWindowOpen) { hdImgui::ShowPresetEditor(&windowData->isPresetsWindowOpen, windowData); }
    // Debug Editor
    if (windowData->isDebugWindowOpen) { hdImgui::ShowDebugEditor(&windowData->isDebugWindowOpen, windowData); }

    // === Temporary, for referencing the components' names in imgui code files ===
    ImGui::ShowDemoWindow();
}

} // namespace hdImgui
