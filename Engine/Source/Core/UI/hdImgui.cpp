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
    // === Input variables ===
    // Static variables for sleep/idle functionality
    
    static std::chrono::steady_clock::time_point s_lastInteractionTime;

    // === Rendering Variables, structs ===
    HdEditorWindowData hdEditorWindowData;

    #if 0
    // =========== Initialization ===========
    void Initialize(GLFWwindow* window) {
        // === GLFW ===
        // Initialize ImGui context.
        // Temporary implementation.
        // *TODO: set up a proper ImGui context (window and rendering backend).
        // I will do it later, as soon as I replace imgui's off-the-shelf Vulkan backend
        // with a custom one, tailored for Hydragon, modern pipeline, Compute shaders etc.
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO(); (void)io;

        // Initialize glfw - window and input handling, for Vulkan backend.
        ImGui_ImplGlfw_InitForVulkan(window, true);
        // Set up GLFW key callback
        glfwSetKeyCallback(window, KeyCallback);
        glfwSetMouseButtonCallback(window, MouseButtonCallback);

        // === ImGui ===
        // Set default ImGui style (or your custom default)
        StyleColorsHydragonDark();
        // Load default fonts
        LoadFonts();

        // === Idle sleep ===
        // Initialize last interaction time - used for idle sleep
        s_lastInteractionTime = std::chrono::steady_clock::now();
        
        // === Windows, sub-editors ===
        // Initialize each ImGui window (Editor windows, sub-editors, etc.)
        InitializeWindows();
    }
    #endif

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
    #if 0
    void StyleColorsHydragonLight() {
        ImGui::StyleColorsLight();  // Start with ImGui's default light style

        // Customize colors
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.9f, 0.9f, 0.9f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        // Add more customizations here...
    }

    void StyleColorsHydragonClassic() {
        ImGui::StyleColorsClassic();  // Start with ImGui's default classic style

        // Customize colors
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.5f, 0.5f, 0.5f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.6f, 0.6f, 0.6f, 1.0f);
        // Add more customizations here...
    }

    void StyleColorsHydragonModern() {
        // Start with a base style (e.g., dark)
        StyleColorsHydragonDark();

        // Add modern customizations
        ImGuiStyle& style = ImGui::GetStyle();
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

    // =========== Rendering ===================
    #endif
    void RenderHydragonEditor() {
        // Store data for each window
        static HdEditorWindowData hdEditorWindowData;

        // === Docking ===
        // Enable docking universally for imgui windows
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // Create the main DockSpace
        ImGui::DockSpaceOverViewport(ImGui::GetID("MainDockSpace"));

        // === Render Windows ===  
        if (hdEditorWindowData.isMainMenuBarWindowOpen) { hdImgui::ShowMainMenuBar(&hdEditorWindowData.isMainMenuBarWindowOpen, &hdEditorWindowData); }
        // Top Toolbar
        if (hdEditorWindowData.isTopToolbarWindowOpen) { hdImgui::ShowTopToolbar(&hdEditorWindowData.isTopToolbarWindowOpen, &hdEditorWindowData); }
        // Left toolbar
        if (hdEditorWindowData.isLeftToolbarWindowOpen) { hdImgui::ShowLeftToolbar(&hdEditorWindowData.isLeftToolbarWindowOpen, &hdEditorWindowData); }
        // Right toolbar
        if (hdEditorWindowData.isRightToolbarWindowOpen) { hdImgui::ShowRightToolbar(&hdEditorWindowData.isRightToolbarWindowOpen, &hdEditorWindowData); }
        // Bottom toolbar
        if (hdEditorWindowData.isBottomToolbarWindowOpen) { hdImgui::ShowBottomToolbar(&hdEditorWindowData.isBottomToolbarWindowOpen, &hdEditorWindowData); }
        // SceneGraph Editor
        if (hdEditorWindowData.isSceneGraphWindowOpen) { hdImgui::ShowSceneGraphEditor(&hdEditorWindowData.isSceneGraphWindowOpen, &hdEditorWindowData); }
        // NodeGraph Editor
        if (hdEditorWindowData.isNodeGraphWindowOpen) { hdImgui::ShowNodeGraphEditor(&hdEditorWindowData.isNodeGraphWindowOpen, &hdEditorWindowData); }
        // Script Editor
        if (hdEditorWindowData.isScriptWindowOpen) { hdImgui::ShowScriptEditor(&hdEditorWindowData.isScriptWindowOpen, &hdEditorWindowData); }
        // Scripts Palette=
        if (hdEditorWindowData.isScriptsPaletteWindowOpen) { hdImgui::ShowScriptsPalette(&hdEditorWindowData.isScriptsPaletteWindowOpen, &hdEditorWindowData); }
        // Commands Palette
        if (hdEditorWindowData.isCommandsPaletteWindowOpen) { hdImgui::ShowCommandsPalette(&hdEditorWindowData.isCommandsPaletteWindowOpen, &hdEditorWindowData); }
        // Console Editor
        if (hdEditorWindowData.isConsoleWindowOpen) { hdImgui::ShowConsole(&hdEditorWindowData.isConsoleWindowOpen, &hdEditorWindowData); }
        // Agents Editor
        if (hdEditorWindowData.isAgentsWindowOpen) { hdImgui::ShowAgentsEditor(&hdEditorWindowData.isAgentsWindowOpen, &hdEditorWindowData); }
        // Viewport 3D
        if (hdEditorWindowData.isViewport3DWindowOpen) { hdImgui::ShowViewport3D(&hdEditorWindowData.isViewport3DWindowOpen, &hdEditorWindowData); }
        // Viewport 3D Tools
        if (hdEditorWindowData.isViewport3DToolsWindowOpen) { hdImgui::ShowViewport3DTools(&hdEditorWindowData.isViewport3DToolsWindowOpen, &hdEditorWindowData); }
        // Viewport 2D
        if (hdEditorWindowData.isViewport2DWindowOpen) { hdImgui::ShowViewport2D(&hdEditorWindowData.isViewport2DWindowOpen, &hdEditorWindowData); }
        // Viewport 2D Tools
        // Most of the settings below should actually be in a separate floating, dockable tab, like "Viewport 2D Settings". Preferrably with a
        // semi-transparent background, to be automatically called as an overlay when the user is in the Viewport 2D tab. With each category
        // under separate headers (like "Brush Settings", "Grid Settings", etc). Or maybe even each category under separate semi-transparent tabs,
        // to be used as overlays, collapsed by default.
        if (hdEditorWindowData.isViewport2DToolsWindowOpen) { hdImgui::ShowViewport2DTools(&hdEditorWindowData.isViewport2DToolsWindowOpen, &hdEditorWindowData); }
        // Properties Editor
        if (hdEditorWindowData.isPropertiesWindowOpen) { hdImgui::ShowPropertyEditor(&hdEditorWindowData.isPropertiesWindowOpen, &hdEditorWindowData); }
        // Asset Manager
        if (hdEditorWindowData.isAssetManagerWindowOpen) { hdImgui::ShowAssetManager(&hdEditorWindowData.isAssetManagerWindowOpen, &hdEditorWindowData); }
        // Lighting Editor
        if (hdEditorWindowData.isLightingWindowOpen) { hdImgui::ShowLightingEditor(&hdEditorWindowData.isLightingWindowOpen, &hdEditorWindowData); }
        // Physics Editor
        if (hdEditorWindowData.isPhysicsWindowOpen) { hdImgui::ShowPhysicsEditor(&hdEditorWindowData.isPhysicsWindowOpen, &hdEditorWindowData); }
        // Bottom status bar
        if (hdEditorWindowData.isBottomStatusBarWindowOpen) { hdImgui::ShowBottomStatusBar(&hdEditorWindowData.isBottomStatusBarWindowOpen, &hdEditorWindowData); }
        // File Explorer
        if (hdEditorWindowData.isFileExplorerWindowOpen) { hdImgui::ShowFileExplorer(&hdEditorWindowData.isFileExplorerWindowOpen, &hdEditorWindowData); }
        // Pattern Orchestrator
        if (hdEditorWindowData.isPatternOrchestratorWindowOpen) { hdImgui::ShowPatternOrchestrator(&hdEditorWindowData.isPatternOrchestratorWindowOpen, &hdEditorWindowData); }
        // Profiler
        if (hdEditorWindowData.isProfilerWindowOpen) { hdImgui::ShowProfiler(&hdEditorWindowData.isProfilerWindowOpen, &hdEditorWindowData); }
        // Streaming Editor
        if (hdEditorWindowData.isStreamingWindowOpen) { hdImgui::ShowStreamingEditor(&hdEditorWindowData.isStreamingWindowOpen, &hdEditorWindowData); }
        // Image Editor
        // Used to display and edit textures, texture settings, edit UVs, packed textures, apply adjustments,
        // configure procedurals, use AI-assisted texture generation etc.
        if (hdEditorWindowData.isImageWindowOpen) { hdImgui::ShowImageTools(&hdEditorWindowData.isImageWindowOpen, &hdEditorWindowData); }
        // Audio Editor
        if (hdEditorWindowData.isAudioWindowOpen) { hdImgui::ShowAudioEditor(&hdEditorWindowData.isAudioWindowOpen, &hdEditorWindowData); }
        // Montage Editor
        // Timeline with tracks, blending, transitions, etc. Used to compose animations, cutscenes, video clips, 
        // audio clips, images and other time varying media.
        // Whereas DCC apps usually rely on a simple timeline by default, Hydragon uses a more advanced montage editor,
        // with two visualization modes: collapsed and expanded (defaults to expanded).
        if (hdEditorWindowData.isMontageWindowOpen) { hdImgui::ShowMontageEditor(&hdEditorWindowData.isMontageWindowOpen, &hdEditorWindowData); }
        // Text Editor
        if (hdEditorWindowData.isTextWindowOpen) { hdImgui::ShowTextEditor(&hdEditorWindowData.isTextWindowOpen, &hdEditorWindowData); }
        // Font Editor
        if (hdEditorWindowData.isFontWindowOpen) { hdImgui::ShowFontEditor(&hdEditorWindowData.isFontWindowOpen, &hdEditorWindowData); }
        // Plugin Editor
        if (hdEditorWindowData.isPluginWindowOpen) { hdImgui::ShowPluginEditor(&hdEditorWindowData.isPluginWindowOpen, &hdEditorWindowData); }
        // Extensions Editor
        if (hdEditorWindowData.isExtensionsWindowOpen) { hdImgui::ShowExtensionsEditor(&hdEditorWindowData.isExtensionsWindowOpen, &hdEditorWindowData); }
        // Macros Editor
        if (hdEditorWindowData.isMacrosWindowOpen) { hdImgui::ShowMacrosEditor(&hdEditorWindowData.isMacrosWindowOpen, &hdEditorWindowData); }
        // Drama Editor
        if (hdEditorWindowData.isDramaWindowOpen) { hdImgui::ShowDramaEditor(&hdEditorWindowData.isDramaWindowOpen, &hdEditorWindowData); }
        // Chimera Pipeline Editor
        if (hdEditorWindowData.isChimeraPipelineWindowOpen) { hdImgui::ShowChimeraPipelineEditor(&hdEditorWindowData.isChimeraPipelineWindowOpen, &hdEditorWindowData); }
        // UI Editor
        if (hdEditorWindowData.isUIEditorWindowOpen) { hdImgui::ShowUIEditor(&hdEditorWindowData.isUIEditorWindowOpen, &hdEditorWindowData); }
        // Networking Editor
        if (hdEditorWindowData.isNetworkingWindowOpen) { hdImgui::ShowNetworkingEditor(&hdEditorWindowData.isNetworkingWindowOpen, &hdEditorWindowData); }
        // Performance Scalability Editor
        if (hdEditorWindowData.isPerformanceScalabilityWindowOpen) { hdImgui::ShowPerformanceScalabilityEditor(&hdEditorWindowData.isPerformanceScalabilityWindowOpen, &hdEditorWindowData); }
        // Reflection Editor
        if (hdEditorWindowData.isReflectionWindowOpen) { hdImgui::ShowReflectionEditor(&hdEditorWindowData.isReflectionWindowOpen, &hdEditorWindowData); }
        // Collaboration Editor
        if (hdEditorWindowData.isCollaborationWindowOpen) { hdImgui::ShowCollaborationEditor(&hdEditorWindowData.isCollaborationWindowOpen, &hdEditorWindowData); }
        // Community Editor
        if (hdEditorWindowData.isCommunityWindowOpen) { hdImgui::ShowCommunityEditor(&hdEditorWindowData.isCommunityWindowOpen, &hdEditorWindowData); }
        // Monetization Editor
        if (hdEditorWindowData.isMonetizationWindowOpen) { hdImgui::ShowMonetizationEditor(&hdEditorWindowData.isMonetizationWindowOpen, &hdEditorWindowData); }
        // Project Insights Editor
        if (hdEditorWindowData.isProjectInsightsWindowOpen) { hdImgui::ShowProjectInsightsEditor(&hdEditorWindowData.isProjectInsightsWindowOpen, &hdEditorWindowData); }
        // Input Editor
        if (hdEditorWindowData.isInputWindowOpen) { hdImgui::ShowInputEditor(&hdEditorWindowData.isInputWindowOpen, &hdEditorWindowData); }
        // Properties Matrix Editor
        if (hdEditorWindowData.isPropertiesMatrixWindowOpen) { hdImgui::ShowPropertiesMatrixEditor(&hdEditorWindowData.isPropertiesMatrixWindowOpen, &hdEditorWindowData); }
        // Localization Editor
        if (hdEditorWindowData.isLocalizationWindowOpen) { hdImgui::ShowLocalizationEditor(&hdEditorWindowData.isLocalizationWindowOpen, &hdEditorWindowData); }
        // Settings Editor
        if (hdEditorWindowData.isSettingsWindowOpen) { hdImgui::ShowSettingsEditor(&hdEditorWindowData.isSettingsWindowOpen, &hdEditorWindowData); }
        // Mesh Editor
        if (hdEditorWindowData.isMeshWindowOpen) { hdImgui::ShowMeshEditor(&hdEditorWindowData.isMeshWindowOpen, &hdEditorWindowData); }
        // Volume Editor
        if (hdEditorWindowData.isVolumeWindowOpen) { hdImgui::ShowVolumeEditor(&hdEditorWindowData.isVolumeWindowOpen, &hdEditorWindowData); }
        // Preset Editor
         if (hdEditorWindowData.isPresetsWindowOpen) { hdImgui::ShowPresetEditor(&hdEditorWindowData.isPresetsWindowOpen, &hdEditorWindowData); }
        // Debug Editor
         if (hdEditorWindowData.isDebugWindowOpen) { hdImgui::ShowDebugEditor(&hdEditorWindowData.isDebugWindowOpen, &hdEditorWindowData); }

        // === Temporary, for referencing the components' names in imgui code files ===
        ImGui::ShowDemoWindow();
    }
} // namespace hdImgui