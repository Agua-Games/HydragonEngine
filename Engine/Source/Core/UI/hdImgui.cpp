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
#if 0
#include "MainMenuBar.h"
#include "TopToolbar.h"
#include "LeftToolbar.h"
#include "RightToolbar.h"
#include "SceneGraphEditor.h"
#include "NodeGraphEditor.h"
#include "ScriptEditor.h"
#include "ScriptsPalette.h"
#include "CommandsPalette.h"
#include "ConsoleEditor.h"
#include "AgentsEditor.h"
#include "Viewport3D.h"
#include "Viewport2D.h"
#include "PropertyEditor.h"
#include "AssetManager.h"
#include "LightingEditor.h"
#include "PhysicsEditor.h"
#include "BottomStatusBar.h"
#include "FileExplorer.h"
#include "PatternOrchestrator.h"
#include "Profiler.h"
#include "StreamingEditor.h"
#include "ImageEditor.h"
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
#include "VolumeEditor.h"
#include "PresetEditor.h"
#endif

namespace hdImgui {
    // === Input variables ===
    // Static variables for sleep/idle functionality
    bool s_isSleeping = false;
    static std::chrono::steady_clock::time_point s_lastInteractionTime;

    // === Styling variables ===
    float globalWindowBgAlpha = 0.35f;

    // === Rendering Variables ===
    struct HydragonEditorWindowData 
    {
        // Track the open/closed state of each window
        bool isTopToolbarWindowOpen = true;
        bool isLeftToolbarWindowOpen = true;
        bool isRightToolbarWindowOpen = true;
        bool isBottomToolbarWindowOpen = true;
        bool isSceneGraphWindowOpen = true;
        bool isNodeGraphWindowOpen = true;
        bool isScriptWindowOpen = true;
        bool isScriptsPaletteWindowOpen = true;
        bool isCommandsPaletteWindowOpen = true;
        bool isConsoleWindowOpen = true;
        bool isAgentsWindowOpen = true;
        bool isViewport3DWindowOpen = true;
        bool isViewport2DWindowOpen = true;
        bool isPropertyEditorWindowOpen = true;
        bool isAssetManagerWindowOpen = true;
        bool isLightingWindowOpen = true;
        bool isPhysicsWindowOpen = true;
        bool isBottomStatusBarWindowOpen = true;
        bool isFileExplorerWindowOpen = true;
        bool isPatternOrchestratorWindowOpen = true;
        bool isProfilerWindowOpen = true;
        bool isStreamingWindowOpen = true;
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
        bool isMeshEditorWindowOpen = true;
        bool isMonetizationWindowOpen = true;
        bool isVolumeEditorWindowOpen = true;
        bool isPresetEditorWindowOpen = true;
    };
    // Store data for each window
    static HydragonEditorWindowData hdEditorWindowData;

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
    void Sleep(bool enable) {
        s_isSleeping = enable;
    }

    void AutoSleepAfterInactivity(float idleTimeSeconds) {
        auto now = std::chrono::steady_clock::now();
        auto idleDuration = std::chrono::duration_cast<std::chrono::seconds>(now - s_lastInteractionTime).count();

        if (idleDuration >= idleTimeSeconds) {
            Sleep(true);
        }
    }

    bool IsSleeping() {
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
        // === Docking ===
        // Enable docking universally for imgui windows
        ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        // Create the main DockSpace
        ImGui::DockSpaceOverViewport(ImGui::GetID("MainDockSpace"));

        // === Top Menu bar ===  
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New")) {}
                if (ImGui::MenuItem("Open Project", "Ctrl+O")) {}
                if (ImGui::BeginMenu("Open Recent Project"))
                {
                    ImGui::MenuItem("StGeorgeKnights.hdproj");
                    ImGui::MenuItem("Kendo.hdproj");
                    ImGui::MenuItem("SoulOfSaka.hdproj");
                    if (ImGui::BeginMenu("More.."))
                    {
                        ImGui::MenuItem("ActionAdventureTemplate.hdproj");
                        ImGui::MenuItem("FightGameTemplate.hdproj"); 
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Close Project", "Ctrl+W")) {}
                if (ImGui::MenuItem("Save Project", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save Project As..")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open SceneGraph", "Ctrl+Shift+O")) {}
                if (ImGui::MenuItem("Open Recent SceneGraph", "Ctrl+Shift+O")) {}
                if (ImGui::MenuItem("Close SceneGraph", "Ctrl+Shift+O")) {}
                if (ImGui::MenuItem("Save SceneGraph As..", "Ctrl+Shift+O")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open NodeGraph", "Ctrl+M")) {}
                if (ImGui::MenuItem("Open Recent NodeGraph", "Ctrl+M")) {}
                if (ImGui::MenuItem("Close NodeGraph", "Ctrl+M")) {}
                if (ImGui::MenuItem("Save NodeGraph As..", "Ctrl+M")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Asset Manager", "Ctrl+A")) {}
                if (ImGui::MenuItem("Import Asset", "Ctrl+I")) {}
                if (ImGui::MenuItem("Export Asset", "Ctrl+E")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                if (ImGui::MenuItem("Procedural Paste", "CTRL+V")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Rename", "CTRL+R")) {}
                if (ImGui::MenuItem("Add Prefix", "CTRL+R")) {}
                if (ImGui::MenuItem("Add Suffix", "CTRL+R")) {}
                if (ImGui::MenuItem("Procedural Rename", "CTRL+R")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Delete", "CTRL+D")) {}
                if (ImGui::BeginMenu("Delete By Type")) 
                {   
                    if (ImGui::MenuItem("Icons")){}
                    if (ImGui::MenuItem("Gizmos")){}
                    if (ImGui::MenuItem("Deformers")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Meshes")){}
                    if (ImGui::MenuItem("Joints")){}
                    if (ImGui::MenuItem("Lights")){}
                    if (ImGui::MenuItem("Cameras")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("VDB Volumes")){}
                    //if (ImGui::MenuItem("Alembic Volumes")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Points")){}
                    if (ImGui::MenuItem("Particles")){}
                    if (ImGui::MenuItem("Fields")){}
                    if (ImGui::MenuItem("Simulation Boundaries")){}
                    if (ImGui::MenuItem("Springs")){}
                    if (ImGui::MenuItem("Rigid Bodies")){}
                    if (ImGui::MenuItem("Dynamic Joints")){}
                    if (ImGui::MenuItem("Dynamic Constraints")){}
                    if (ImGui::MenuItem("Clothes")){}
                    if (ImGui::MenuItem("Fluids")){}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Delete All By Type")) 
                {   
                    if (ImGui::MenuItem("Icons")){}
                    if (ImGui::MenuItem("Gizmos")){}
                    if (ImGui::MenuItem("Deformers")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Meshes")){}
                    if (ImGui::MenuItem("Joints")){}
                    if (ImGui::MenuItem("Lights")){}
                    if (ImGui::MenuItem("Cameras")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("VDB Volumes")){}
                    //if (ImGui::MenuItem("Alembic Volumes")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Points")){}
                    if (ImGui::MenuItem("Particles")){}
                    if (ImGui::MenuItem("Fields")){}
                    if (ImGui::MenuItem("Simulation Boundaries")){}
                    if (ImGui::MenuItem("Springs")){}
                    if (ImGui::MenuItem("Rigid Bodies")){}
                    if (ImGui::MenuItem("Dynamic Joints")){}
                    if (ImGui::MenuItem("Dynamic Constraints")){}
                    if (ImGui::MenuItem("Clothes")){}
                    if (ImGui::MenuItem("Fluids")){}
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Frame Selected", "CTRL+F")) {}
                if (ImGui::MenuItem("Frame All", "CTRL+A")) {}
                if (ImGui::MenuItem("Lock Viewport to Selected", "CTRL+L")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Play Runtime", "CTRL+F")) {}
                if (ImGui::MenuItem("Pause Runtime", "CTRL+A")) {}
                if (ImGui::MenuItem("Step Runtime", "CTRL+L")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Duplicate", "CTRL+I")) {}
                if (ImGui::MenuItem("Procedural Duplicate", "CTRL+I")) {}
                if (ImGui::MenuItem("Duplicate With Transform", "CTRL+I")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Group", "CTRL+A")) {}
                if (ImGui::MenuItem("Ungroup", "CTRL+A")) {}
                if (ImGui::MenuItem("Create LOD NodeGraph", "CTRL+A")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Parent/Compose SceneGraph As Layer", "CTRL+A")) {}
                if (ImGui::MenuItem("Unparent SceneGraph", "CTRL+A")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Find", "CTRL+F")) {}   // Search in UI - menu item, command etc.
                if (ImGui::MenuItem("Explain", "CTRL+X")) {}   // Opens Agent to explain any UI element - its function, use etc.
                ImGui::Separator();
                if (ImGui::MenuItem("Keyboard Shortcuts")) {} 
                if (ImGui::MenuItem("Menus")) {} 
                if (ImGui::MenuItem("Project Settings")) {} 
                if (ImGui::MenuItem("Editor Preferences")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Create"))
            {
                if (ImGui::BeginMenu("SceneGraph")) 
                {
                    if (ImGui::MenuItem("Procedural SceneGraph")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("L-Tree")){}
                    if (ImGui::MenuItem("Fractal")){}
                    if (ImGui::MenuItem("Debris")){}     // a quickstart rig with some physics simulation, baking etc.
                    if (ImGui::MenuItem("Raycast Rules")){}
                    if (ImGui::MenuItem("Terrain Rules")){}
                    if (ImGui::MenuItem("Spline Path")){}
                    if (ImGui::MenuItem("Proc Building")){}
                    if (ImGui::MenuItem("Proc Settlement")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Basic SceneGraph")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open SceneGraph Library")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("NodeGraph")) 
                {
                    if (ImGui::MenuItem("Procedural NodeGraph")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("L-Tree")){}
                    if (ImGui::MenuItem("Fractal")){}
                    if (ImGui::MenuItem("Debris")){}     // a quickstart rig with some physics simulation, baking etc.
                    if (ImGui::MenuItem("Raycast Rules")){}
                    if (ImGui::MenuItem("Terrain Rules")){}
                    if (ImGui::MenuItem("Spline Path")){}
                    if (ImGui::MenuItem("Proc Building")){}
                    if (ImGui::MenuItem("Proc Settlement")){}
                    if (ImGui::MenuItem("Physics Destruction")){}
                    if (ImGui::MenuItem("Physics Destructible")){}
                    if (ImGui::MenuItem("Physics FX Field")){}
                    if (ImGui::MenuItem("Physicalised Anim")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Proc Prop Rig")){}
                    if (ImGui::MenuItem("Proc Weapon Rig")){}
                    if (ImGui::MenuItem("Proc Character Rig")){}
                    if (ImGui::MenuItem("Proc Vehicle Rig")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Proc Audio Source")){}
                    if (ImGui::MenuItem("Proc Audio Listener")){}
                    if (ImGui::MenuItem("Proc Audio Ambience")){}
                    if (ImGui::MenuItem("Proc Audio Soundtrack")){}
                    if (ImGui::MenuItem("Proc Character Audio")){}
                    if (ImGui::MenuItem("Proc Audio Mixer")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Basic Animation")){}
                    if (ImGui::MenuItem("Biped Character Anim")){}
                    if (ImGui::MenuItem("Quadruped Character Anim")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Basic NodeGraph")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open NodeGraph Library")) {}
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Transform/Space Locator")){}
                ImGui::Separator();
                if (ImGui::BeginMenu("Curve Primitive")) 
                {
                    if (ImGui::MenuItem("Procedural Spline")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Spline L-Tree")){}
                    if (ImGui::MenuItem("Spline Crawling")){}
                    if (ImGui::MenuItem("Spline Fractal")){}
                    if (ImGui::MenuItem("Spline Extrusion")){}
                    if (ImGui::MenuItem("Spline")){}
                    if (ImGui::MenuItem("Spline Rig")){}    // includes path constraints, optional physics etc. From roads to hanging cables, fences.
                    if (ImGui::MenuItem("Spline Path")){}
                    if (ImGui::MenuItem("Spline Cable")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Linear Spline")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open Proc Spline Library")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Mesh Primitive")) 
                {
                    if (ImGui::MenuItem("Procedural Mesh")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Box")){}
                    if (ImGui::MenuItem("Sphere")){}
                    if (ImGui::MenuItem("Capsule")){}
                    if (ImGui::MenuItem("Cylinder")){}
                    if (ImGui::MenuItem("Cone")){}
                    if (ImGui::MenuItem("Plane")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Platonic Solid")){}
                    if (ImGui::MenuItem("Manifold Place")){}
                    if (ImGui::MenuItem("Manifold Solid")){}
                    if (ImGui::MenuItem("Pyramid")){}
                    if (ImGui::MenuItem("Prism")){}
                    if (ImGui::MenuItem("Pipe")){}
                    if (ImGui::MenuItem("Helix")){}
                    if (ImGui::MenuItem("Gear")){}
                    if (ImGui::MenuItem("Gear")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open Proc Mesh Library")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Volume Primitive")) 
                {
                    if (ImGui::MenuItem("Procedural Volume")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Voxels Array Structure")){}
                    if (ImGui::MenuItem("Voxels Rock")){}
                    if (ImGui::MenuItem("Voxels Cliff")){}
                    if (ImGui::MenuItem("Voxels Debris")){}
                    if (ImGui::MenuItem("Voxels Cave")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Box")){}
                    if (ImGui::MenuItem("Sphere")){}
                    if (ImGui::MenuItem("Capsule")){}
                    if (ImGui::MenuItem("Cylinder")){}
                    if (ImGui::MenuItem("Cone")){}
                    if (ImGui::MenuItem("Plane")){}     // for 2D fluid sim, sliced sim, for example.
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open Proc Volume Library")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Light Primitive")) 
                {
                    if (ImGui::MenuItem("Procedural Light")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Ambient")){}
                    if (ImGui::MenuItem("Directional")){}
                    if (ImGui::MenuItem("Point")){}
                    if (ImGui::MenuItem("Spot")){}
                    if (ImGui::MenuItem("Area")){}
                    if (ImGui::MenuItem("Volume")){}     // for 2D fluid sim, sliced sim, for example.
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open Proc Light Library")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Camera Primitive")) 
                {
                    if (ImGui::MenuItem("Procedural Camera")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Camera & Cam Rig")){}
                    if (ImGui::MenuItem("Camera & Light Rig")){}
                    if (ImGui::MenuItem("Camera Film Set")){}
                    if (ImGui::MenuItem("Physics Action Camera")){}
                    if (ImGui::MenuItem("Physics Vehicle Camera")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Orthographic")){}
                    if (ImGui::MenuItem("Perspective")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open Proc Camera Library")) {}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Texture Primitive")) 
                {
                    if (ImGui::MenuItem("Procedural Texture")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Nature Ground Texture")) {}
                    if (ImGui::MenuItem("Snow Texture")) {}
                    if (ImGui::MenuItem("Ice Texture")) {}
                    if (ImGui::MenuItem("Rock Texture")) {}
                    if (ImGui::MenuItem("Cliff Texture")) {}
                    if (ImGui::MenuItem("Bark Texture")) {}
                    if (ImGui::MenuItem("Branch Texture")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Architectural Ground Texture")) {}
                    if (ImGui::MenuItem("Architectural Wall Texture")) {}
                    if (ImGui::MenuItem("Architectural Roof Texture")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Aged Metal Texture")) {}
                    if (ImGui::MenuItem("Aged Armor Texture")) {}
                    if (ImGui::MenuItem("Aged Fabric Texture")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Skin Texture")) {}
                    if (ImGui::MenuItem("Hair Texture")) {}
                    if (ImGui::MenuItem("Fur Texture")) {}
                    if (ImGui::MenuItem("Eye Texture")) {}
                    if (ImGui::MenuItem("Scales Skin Texture")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Render Target/Texture")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open Proc Texture Library")) {}
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::BeginMenu("Utilities")) 
                {
                    if (ImGui::MenuItem("Measure Distance")){}
                    if (ImGui::MenuItem("Arc Length")){}
                    if (ImGui::MenuItem("SceneGraph Note")){}
                    if (ImGui::MenuItem("Construction Plane")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Open Proc Utility Library")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Generate"))
            {
                if (ImGui::MenuItem("Call Agent", "Ctrl+Shift+N")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("NodeGraph", "Ctrl+Shift+N")) {}
                if (ImGui::MenuItem("SceneGraph", "Ctrl+Shift+N")) {}
                if (ImGui::MenuItem("Script", "Ctrl+Shift+S")) {}
                if (ImGui::MenuItem("Procedural", "Ctrl+P")) {}
                if (ImGui::MenuItem("Macro", "Ctrl+Shift+M")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Image", "Ctrl+Shift+I")) {}        
                if (ImGui::MenuItem("Mesh", "Ctrl+Shift+I")) {}        
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Select"))
            {
                if (ImGui::MenuItem("All")) {}
                if (ImGui::BeginMenu("All By Type")) 
                {   
                    if (ImGui::MenuItem("Icons")){}
                    if (ImGui::MenuItem("Gizmos")){}
                    if (ImGui::MenuItem("Deformers")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Meshes")){}
                    if (ImGui::MenuItem("Joints")){}
                    if (ImGui::MenuItem("Lights")){}
                    if (ImGui::MenuItem("Cameras")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("VDB Volumes")){}
                    //if (ImGui::MenuItem("Alembic Volumes")){}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Points")){}
                    if (ImGui::MenuItem("Particles")){}
                    if (ImGui::MenuItem("Fields")){}
                    if (ImGui::MenuItem("Simulation Boundaries")){}
                    if (ImGui::MenuItem("Springs")){}
                    if (ImGui::MenuItem("Rigid Bodies")){}
                    if (ImGui::MenuItem("Dynamic Joints")){}
                    if (ImGui::MenuItem("Dynamic Constraints")){}
                    if (ImGui::MenuItem("Clothes")){}
                    if (ImGui::MenuItem("Fluids")){}
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Deselect All", "Ctrl+D")) {}
                if (ImGui::MenuItem("SceneGraph Hierarchy", "Ctrl+H")) {}
                if (ImGui::MenuItem("SceneGraph Children", "Ctrl+H")) {}
                if (ImGui::MenuItem("SceneGraph Root", "Ctrl+H")) {}
                if (ImGui::MenuItem("Invert Selection", "Ctrl+I")) {}
                if (ImGui::MenuItem("Similar", "Ctrl+S")) {}
                if (ImGui::MenuItem("Selection Sets", "Ctrl+S")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Grow", ">")) {}
                if (ImGui::MenuItem("Shrink", "<")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Object/Component", "Ctrl+O")) {}
                if (ImGui::BeginMenu("Polygon Components")) 
                {
                    if (ImGui::MenuItem("Vertex")){}
                    if (ImGui::MenuItem("Edge")){}
                    if (ImGui::MenuItem("Face")){}
                    if (ImGui::MenuItem("UV")){}
                    if (ImGui::MenuItem("UV Shell")){}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Volume Components")) 
                {
                    if (ImGui::MenuItem("Voxel")){}
                    if (ImGui::MenuItem("Boundaries")){}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Physics Components")) 
                {
                    if (ImGui::MenuItem("Particle/Point")){}
                    if (ImGui::MenuItem("Emitter")){}
                    if (ImGui::MenuItem("Spring")){}
                    if (ImGui::MenuItem("Rigid Body")){}
                    if (ImGui::MenuItem("Dynamic Joint")){}
                    if (ImGui::MenuItem("Dynamic Constraint")){}
                    if (ImGui::MenuItem("Cloth Mesh")){}
                    if (ImGui::MenuItem("Fluid")){}
                    if (ImGui::MenuItem("Destructible Collection")){}
                    if (ImGui::MenuItem("Destructible Chunk")){}
                    if (ImGui::MenuItem("Collider")){}
                    if (ImGui::MenuItem("Field")){}
                    if (ImGui::MenuItem("Simulation Boundary")){}
                    if (ImGui::MenuItem("System")){}
                    if (ImGui::MenuItem("Solver & Assigned")){}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Asset"))
            {
                if (ImGui::MenuItem("Import", "Ctrl+I")) {}
                if (ImGui::MenuItem("Export", "Ctrl+E")) {}
                if (ImGui::MenuItem("Reimport", "Ctrl+R")) {}
                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::MenuItem("Duplicate", "Ctrl+D")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Procedural Asset Batch Processor", "Ctrl+O")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Asset Library", "Ctrl+O")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("NodeGraph"))
            {  
                if (ImGui::MenuItem("Create Node", "Ctrl+N")) {}
                if (ImGui::MenuItem("Delete Node", "Ctrl+Del")) {}
                if (ImGui::MenuItem("Rename Node", "Ctrl+R")) {}
                if (ImGui::MenuItem("Duplicate Node", "Ctrl+D")) {}
                if (ImGui::MenuItem("Replace Selected Node", "Ctrl+D")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open NodeGraph Library", "Ctrl+D")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("SceneGraph"))
            {
                if (ImGui::MenuItem("Create Node", "Ctrl+N")) {}
                if (ImGui::MenuItem("Delete Node", "Ctrl+Del")) {}
                if (ImGui::MenuItem("Rename Node", "Ctrl+R")) {}
                if (ImGui::MenuItem("Duplicate Node", "Ctrl+D")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Replace Selected Node", "Ctrl+D")) {}
                if (ImGui::MenuItem("Override Selected Instance", "Ctrl+D")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open SceneGraph Library", "Ctrl+O")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Rendering"))
            {
                static int s_lightingType = 3;
                if (ImGui::Combo("Lighting Type", &s_lightingType, "No Lighting\0Lighting Only\0Lighting\0Lighting + Detail")) {}
                ImGui::Separator();
                static bool s_enableGI = true;
                static bool s_enableSSAO = true;
                static bool s_enableSkylight = true;
                static bool s_enableAtmosphere = true;
                static bool s_enableClouds = true;
                static int s_cloudsMethod = 0;
                static bool s_enableFog = true;
                static int s_fogMethod = 0;
                static bool s_enableShadows = true;
                static int s_shadowMethod = 0;
                if (ImGui::Checkbox("Enable Shadows", &s_enableShadows)) {}
                if (ImGui::Combo("Shadows Method", &s_shadowMethod, "Depth Map Shadows\0Virtual Shadows Simple\0Virtual Shadows Advanced")) {}
                if (ImGui::Checkbox("Enable GI", &s_enableGI)) {}
                if (ImGui::Checkbox("Enable SSAO", &s_enableSSAO)) {}
                if (ImGui::Checkbox("Enable Skylight", &s_enableSkylight)) {}
                if (ImGui::Checkbox("Enable Atmosphere", &s_enableAtmosphere)) {}
                if (ImGui::Checkbox("Enable Clouds", &s_enableClouds)) {}
                if (ImGui::Combo("Clouds Method", &s_cloudsMethod, "Flat Layer\0Basic Volumetric\0Detailed Volumetric", 3)) {}
                if (ImGui::Checkbox("Enable Fog", &s_enableFog)) {}
                if (ImGui::Combo("Fog Type", &s_fogMethod, "Simple Depth-based\0Basic Volumetric\0Detailed Volumetric", 3)) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Lighting Editor", "Ctrl+O")) {}      
                if (ImGui::MenuItem("Lighting Profiler", "Ctrl+O")) {}      // Opens the Lighting tab in the Profiler Editor
                if (ImGui::MenuItem("Rendering Settings", "Ctrl+O")) {} 
                ImGui::Separator();
                if (ImGui::MenuItem("Open Lighting Library", "Ctrl+O")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Physics"))
            {
                static bool s_enablePhysics = true;
                static bool s_enableLocalDetail = true;
                static bool s_enablePhysicalisedAnimation = true;
                static int s_PhysicsSolverType = 0;
                static int s_PhysLocalDetailType = 0;
                static int s_PhysAnimType = 0;
                if (ImGui::Checkbox("Enable Physics", &s_enablePhysics)) {}
                if (ImGui::Checkbox("Enable Local Detail", &s_enablePhysics)) {}
                if (ImGui::Checkbox("Enable Physicalised Animation", &s_enablePhysicalisedAnimation)) {}
                ImGui::Separator();
                if (ImGui::Combo("Physics Solver Type", &s_PhysicsSolverType, "Standard Solver\0Fast Solver\0GPU-Compute Solver")) {}
                if (ImGui::Combo("Local Detail Solver Type", &s_PhysLocalDetailType, "Standard Solver\0Fast Solver\0GPU-Compute Solver")) {}
                if (ImGui::Combo("Physicalized Animation Type", &s_PhysAnimType, "Standard PhysAnim\0Fast PhysAnim\0GPU-Compute PhysAnim")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Proc Prop Physics Rig", "Ctrl+O")) {}
                if (ImGui::MenuItem("Proc Destructible Physics Rig", "Ctrl+O")) {}
                if (ImGui::MenuItem("Proc Character Physics Rig", "Ctrl+O")) {}
                if (ImGui::MenuItem("Proc Vehicle Physics Rig", "Ctrl+O")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Add Procedural Physics Rig to SceneGraph", "Ctrl+O")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Physics Editor", "Ctrl+O")) {}
                if (ImGui::MenuItem("Physics Profiler", "Ctrl+O")) {}
                if (ImGui::MenuItem("Physics Settings", "Ctrl+O")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Physics Library", "Ctrl+O")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Run"))
            {
                if (ImGui::MenuItem("Open Script", "Ctrl+O")) {}
                if (ImGui::MenuItem("Run Script", "Ctrl+S")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open Editor Scripts Library", "Ctrl+O")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                static bool s_showGrid = true;
                if (ImGui::Checkbox("Grid", &s_showGrid)) {}
                if (ImGui::MenuItem("Contextual Info Overlay", "Ctrl+O")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("Hide")) 
                {   if (ImGui::MenuItem("Hide Selected")){}
                        if (ImGui::MenuItem("Hide Unselected")){}
                        if (ImGui::MenuItem("All")){}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Hide Icons")){}
                        if (ImGui::MenuItem("Hide Gizmos")){}
                        if (ImGui::MenuItem("Hide Deformers")){}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Hide Meshes")){}
                        if (ImGui::MenuItem("Hide Joints")){}
                        if (ImGui::MenuItem("Hide Lights")){}
                        if (ImGui::MenuItem("Hide Cameras")){}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Hide VDB Volumes")){}
                        //if (ImGui::MenuItem("Hide Alembic Volumes")){}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Hide Points")){}
                        if (ImGui::MenuItem("Hide Particles")){}
                        if (ImGui::MenuItem("Hide Fields")){}
                        if (ImGui::MenuItem("Hide Simulation Boundaries")){}
                        if (ImGui::MenuItem("Hide Springs")){}
                        if (ImGui::MenuItem("Hide Colliders")){}
                        if (ImGui::MenuItem("Hide Rigid Bodies")){}
                        if (ImGui::MenuItem("Hide Dynamic Joints")){}
                        if (ImGui::MenuItem("Hide Dynamic Constraints")){}
                        if (ImGui::MenuItem("Hide Clothes")){}
                        if (ImGui::MenuItem("Hide Fluids")){}
                    ImGui::EndMenu();
                }
                if (ImGui::BeginMenu("Show"))
                {   if (ImGui::MenuItem("Show Selected")){}
                        if (ImGui::MenuItem("Show Unselected")){}
                        if (ImGui::MenuItem("All")){}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Show Icons")){}
                        if (ImGui::MenuItem("Show Gizmos")){}
                        if (ImGui::MenuItem("Show Deformers")){}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Show Meshes")){}
                        if (ImGui::MenuItem("Show Joints")){}
                        if (ImGui::MenuItem("Show Lights")){}
                        if (ImGui::MenuItem("Show Cameras")){}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Show VDB Volumes")){}
                        //if (ImGui::MenuItem("Show Alembic Volumes")){}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Show Points")){}
                        if (ImGui::MenuItem("Show Particles")){}
                        if (ImGui::MenuItem("Show Fields")){}
                        if (ImGui::MenuItem("Show Simulation Boundaries")){}
                        if (ImGui::MenuItem("Show Springs")){}
                        if (ImGui::MenuItem("Show Colliders")){}
                        if (ImGui::MenuItem("Show Rigid Bodies")){}
                        if (ImGui::MenuItem("Show Dynamic Joints")){}
                        if (ImGui::MenuItem("Show Dynamic Constraints")){}
                        if (ImGui::MenuItem("Show Clothes")){}
                        if (ImGui::MenuItem("Show Fluids")){}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Window"))
            {
                if (ImGui::MenuItem("Top Toolbar", "Ctrl+I")) {}
                if (ImGui::MenuItem("Left Toolbar", "Ctrl+I")) {}
                if (ImGui::MenuItem("Right Toolbar", "Ctrl+I")) {}
                if (ImGui::MenuItem("Bottom Toolbar", "Ctrl+I")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("NodeGraph", "Ctrl+M")) {}
                if (ImGui::MenuItem("SceneGraph", "Ctrl+O")) {hdEditorWindowData.isSceneGraphWindowOpen = true;}
                ImGui::Separator();
                if (ImGui::MenuItem("Viewport 3D", "Ctrl+Shift+V")) {}
                if (ImGui::MenuItem("Viewport 2D", "Ctrl+Shift+D")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Asset Manager", "Ctrl+A")) {}
                if (ImGui::MenuItem("Chimera Pipeline", "Ctrl+Shift+C")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Agents", "Ctrl+Shift+A")) {}        
                ImGui::Separator();         
                if (ImGui::MenuItem("Mesh Editor", "Ctrl+Shift+M")) {}
                if (ImGui::MenuItem("Volume Editor", "Ctrl+Shift+V")) {}
                if (ImGui::MenuItem("Montage Editor", "Ctrl+Shift+T")) {}
                if (ImGui::MenuItem("Audio Editor", "Ctrl+Shift+A")) {}
                if (ImGui::MenuItem("Drama Editor", "Ctrl+Shift+D")) {}
                if (ImGui::MenuItem("Streaming Editor", "Ctrl+Shift+M")) {}
                if (ImGui::MenuItem("Networking Editor", "Ctrl+Shift+N")) {}
                if (ImGui::MenuItem("Input Editor", "Ctrl+Shift+T")) {}
                if (ImGui::MenuItem("Localization Editor", "Ctrl+Shift+E")) {}
                if (ImGui::MenuItem("Properties Matrix", "Ctrl+Shift+E")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("UI Editor", "Ctrl+Shift+U")) {}
                if (ImGui::MenuItem("Font Editor", "Ctrl+Shift+F")) {}
                if (ImGui::MenuItem("Text Editor", "Ctrl+Shift+X")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Command Palette", "Ctrl+Shift+P")) {}
                if (ImGui::MenuItem("Console", "Ctrl+Shift+L")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("Developer"))
                {
                    if (ImGui::MenuItem("Macros Editor", "Ctrl+Shift+M")) {}
                    if (ImGui::MenuItem("Script Editor", "Ctrl+Shift+O")) {}
                    if (ImGui::MenuItem("Reflection Editor", "Ctrl+Shift+R")) {}
                    if (ImGui::MenuItem("Plugin Editor", "Ctrl+Shift+L")) {}
                    if (ImGui::MenuItem("Extensions Editor", "Ctrl+Shift+E")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Collaboration Editor", "Ctrl+Shift+E")) {}
                    if (ImGui::MenuItem("Community Editor", "Ctrl+Shift+E")) {}
                    if (ImGui::MenuItem("Monetization Editor", "Ctrl+Shift+E")) {}
                    ImGui::Separator();
                    if (ImGui::MenuItem("Performance Scalability", "Ctrl+Shift+B")) {}
                    if (ImGui::MenuItem("Project Insights", "Ctrl+Shift+B")) {}
                    if (ImGui::MenuItem("Profiler Editor", "Ctrl+Shift+P")) {}
                    if (ImGui::MenuItem("Debugger Editor", "Ctrl+Shift+D")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Hydragon Documentation", "F1")) {}
                if (ImGui::MenuItem("What's New")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Intro Tutorials")) {}
                if (ImGui::MenuItem("Learn", "F3")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Support Center")) {}
                if (ImGui::MenuItem("Report a Problem")) {}
                if (ImGui::MenuItem("Contribute")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("About Hydragon")) {}
                if (ImGui::MenuItem("Hydragon Home Page")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }

        // === Top Toolbar ===
        ImGui::Begin("Top Toolbar", nullptr);
        ImGui::End();

        // === Left sidebar ====
        ImGui::Begin("Left Sidebar", nullptr);
        ImGui::End();

        // === Right sidebar ====
        ImGui::Begin("Right Sidebar", nullptr);
        ImGui::End();

        // === SceneGraph Editor ====
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        if (hdEditorWindowData.isSceneGraphWindowOpen)
        {
            if (ImGui::Begin("SceneGraph ", &hdEditorWindowData.isSceneGraphWindowOpen, ImGuiWindowFlags_MenuBar))
            {
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::BeginMenu("File"))
                    {
                        if (ImGui::MenuItem("New SceneGraph")) {}
                        if (ImGui::MenuItem("Import SceneGraph", "Ctrl+O")) {}
                        if (ImGui::MenuItem("Open SceneGraph", "Ctrl+A")) {}
                        if (ImGui::BeginMenu("Open Recent SceneGraph"))
                        {
                            ImGui::MenuItem("Museum.usd");
                            ImGui::MenuItem("Restaurant.husd");
                            ImGui::MenuItem("WarHorse.usd");
                            if (ImGui::BeginMenu("More.."))
                            {
                                ImGui::MenuItem("Submarine.usd");
                                ImGui::MenuItem("ArcticStation.usd"); 
                                ImGui::EndMenu();
                            }
                            ImGui::EndMenu();
                        }
                        ImGui::Separator();
                        if (ImGui::MenuItem("Close SceneGraph", "Ctrl+W")) {}
                        if (ImGui::MenuItem("Save SceneGraph", "Ctrl+S")) {}
                        if (ImGui::MenuItem("Save SceneGraph As..")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Add SceneGraph As Layer", "Ctrl+Shift+A")) {}
                        if (ImGui::MenuItem("Remove Layer", "Ctrl+R")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Open in NodeGraph", "Ctrl+M")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Exit", "Alt+F4")) {}
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Edit"))
                    {
                        if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                        if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                        ImGui::Separator();
                        if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                        if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                        if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Compose"))
                    {
                        if (ImGui::MenuItem("Add Layer", "CTRL+A")) {}
                        if (ImGui::MenuItem("Duplicate Layer", "CTRL+D")) {} 
                        ImGui::Separator();
                        if (ImGui::MenuItem("Replace Layer", "CTRL+R")) {}
                        if (ImGui::MenuItem("Remove Layer", "CTRL+R")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Move Layer Up", "CTRL+UP")) {}
                        if (ImGui::MenuItem("Move Layer Down", "CTRL+DOWN")) {}
                        ImGui::Separator();
                        
                        ImGui::EndMenu();
                    }
                    if (ImGui::BeginMenu("Instance"))
                    {
                        if (ImGui::MenuItem("Add Instance", "CTRL+A")) {}
                        if (ImGui::MenuItem("Duplicate Instance", "CTRL+D")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Replace Instance", "CTRL+R")) {}
                        if (ImGui::MenuItem("Remove Instance", "CTRL+R")) {}
                        ImGui::Separator();
                        if (ImGui::MenuItem("Open in NodeGraph", "CTRL+M")) {}
                        ImGui::EndMenu();
                    }   
                    ImGui::EndMenuBar();
                }
                // Example: TreeNode with flags
                ImGuiTreeNodeFlags SceneGraphTreeNodeFlags = 
                    ImGuiTreeNodeFlags_DefaultOpen | 
                    ImGuiTreeNodeFlags_Framed | 
                    ImGuiTreeNodeFlags_OpenOnArrow |
                    ImGuiTreeNodeFlags_OpenOnDoubleClick;
                if (ImGui::TreeNodeEx("Castle", SceneGraphTreeNodeFlags)) {
                    // Child nodes
                    if (ImGui::TreeNode("Tower 1")) {
                        if (ImGui::TreeNode("Door 1")){
                            ImGui::Text("Handle 1");
                            ImGui::TreePop();
                        }
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Tower 2")) {
                        ImGui::Text("Bullwark 1");
                        ImGui::TreePop();
                    }
                    ImGui::TreePop();
                }
                if (ImGui::TreeNodeEx("Bridge", SceneGraphTreeNodeFlags)) {
                    // Child nodes
                    if (ImGui::TreeNode("Ending Node 1")) {
                        ImGui::Text("Tower 1");
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Shaft 1")) {
                        ImGui::Text("This is the second child node.");
                        ImGui::TreePop();
                    }
                    if (ImGui::TreeNode("Ending Node 2")) {
                        ImGui::Text("Tower 1");
                        ImGui::TreePop();
                    }
                    // End the root node
                    ImGui::TreePop();
                }
                ImGui::Separator();
                if (ImGui::CollapsingHeader("SceneGraph Layers"))
                {
                    ImGui::Button("Add Layer");
                    ImGui::Button("Duplicate Layer");
                    ImGui::Button("Replace Layer");
                    ImGui::Button("Remove Layer");
                }
                if (ImGui::CollapsingHeader("Streaming"))
                {
                    static int s_streamingMethod = 1;
                    static int s_streamingPriority = 0;
                    ImGui::Combo("Method", &s_streamingMethod, "Lazy\0Standard\0Priority\0", 3);
                    ImGui::Combo("Priority", &s_streamingPriority, "Low\0Medium\0High\0", 3);
                    ImGui::Separator();
                    ImGui::Button("Open in NodeGraph");
                }
                ImGui::End(); 
            }    
        }
        
        
        // === NodeGraph Editor ====
        // At first, materials will also be edited in the NodeGraph Editor.
        // Also other objects can be edited in the NodeGraph Editor, like Meshes, Textures,
        // Audio mixing graphs, rendering graphs, post-processing graphs, physics graphs,
        // effects graphs, etc.
        // using  context-sensitive overlays, menus. 
        // The base asset in Hydragon is a graph of nodes.
        // The properties/params of the inner nodes can be exposed to the parent graph's properties,
        // thus consolidating the inner nodes into a single node with a single UI (properties, overlays, menus),
        // which in effect is the same as the NodeGraph Editor (is an editor in itself).
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("NodeGraph", nullptr);
        ImGui::Button("Open Graph");
        ImGui::Button("New Graph");
        ImGui::Button("Save Graph");
        ImGui::Button("Add Node");
        ImGui::Button("Group Selected Nodes");
        bool autoCompile = true;
        ImGui::Checkbox("Auto-Compile", &autoCompile);
        ImGui::Button("Compile Graph");
        ImGui::Button("Run Graph");
        ImGui::Button("Validate Graph");
        ImGui::End();

        // === Script Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Scripting", nullptr);
        ImGui::Button("Import Script");
        ImGui::Button("New Script");
        ImGui::Button("Record Macro");
        ImGui::Button("Save Script");
        ImGui::Button("Compile Script");
        ImGui::Button("Run Script");
        ImGui::Button("Validate Script");
        ImGui::Button("Go To Line");
        ImGui::Button("Debug Script");
        ImGui::Button("Add Script");
        ImGui::Button("Ask Agent");
        ImGui::Button("Run Agent");
        ImGui::Button("Validate Agent");
        ImGui::End();

        // === Scripts Palette ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Scripts Palette", nullptr);  
        if (ImGui::BeginTabBar("Scripts Palette Tabs"))
        {
            if (ImGui::BeginTabItem("Hydragon"))
            {
                if (ImGui::CollapsingHeader("Physics")) {
                    ImGui::Button("Optimize Simulation For Solver");
                    ImGui::Button("Setup Basic Destructible");
                }
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("User"))
            {
                ImGui::Button("Add Script");    // Migrate to consolidated Menu bar later
                ImGui::Button("Customize UI for my project"); 
                ImGui::Button("Add My Project's Macros"); 
                ImGui::Button("Add, Open My Project's UI"); 
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
        ImGui::End();

        // === Commands Palette ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Commands Palette", nullptr);
        ImGui::Button("Run Command");
        ImGui::End();

        // === Console Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Console", nullptr);
        ImGui::Button("Clear Console");
        ImGui::Button("Run Command");
        ImGui::End();

        // === Agents Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Agents", nullptr);
        ImGui::Button("Add Agent");
        ImGui::Button("Load Model");
        ImGui::Button("Start Server");
        ImGui::Button("Stop Server");
        ImGui::Button("Start Client");
        ImGui::Button("Stop Client");
        ImGui::Button("Run Agent");
        ImGui::Button("Validate Agent");
        ImGui::Button("Ask Agent");
        ImGui::Button("Run Command");
        ImGui::Button("Validate Command");
        ImGui::Button("Add Command");
        ImGui::Button("Run Macro");
        ImGui::Button("Validate Macro");
        ImGui::Button("Add Macro");
        ImGui::Button("Load Macro");
        ImGui::End();

        // === Viewport 3D ===
        ImGui::Begin("Viewport 3D", nullptr);
        ImGui::End();

        // === Viewport 3D Tools ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Viewport 3D Tools", nullptr, ImGuiWindowFlags_MenuBar);

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New")) {}
                if (ImGui::MenuItem("Import SceneGraph", "Ctrl+O")) {}
                if (ImGui::MenuItem("Open SceneGraph", "Ctrl+A")) {}
                if (ImGui::BeginMenu("Open Recent SceneGraph"))
                {
                    ImGui::MenuItem("Museum.usd");
                    ImGui::MenuItem("Restaurant.husd");
                    ImGui::MenuItem("WarHorse.usd");
                    if (ImGui::BeginMenu("More.."))
                    {
                        ImGui::MenuItem("Submarine.usd");
                        ImGui::MenuItem("ArcticStation.usd"); 
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Close SceneGraph", "Ctrl+W")) {}
                if (ImGui::MenuItem("Save SceneGraph", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save SceneGraph As..")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Add SceneGraph As Layer", "Ctrl+Shift+A")) {}
                if (ImGui::MenuItem("Remove Layer", "Ctrl+R")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open in NodeGraph", "Ctrl+M")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "Alt+F4")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::CollapsingHeader("SceneGraph")) {
            if (ImGui::CollapsingHeader("Main"))
            {
                ImGui::Button("Import SceneGraph");
            }
            if (ImGui::CollapsingHeader("Layers"))
            {
                ImGui::Button("Add Layer");
                ImGui::Button("Remove Layer");
            }
            if (ImGui::CollapsingHeader("Nodes")) 
            {
                ImGui::Button("Add Node");
                ImGui::Button("Remove Node");
                ImGui::Button("Replace Node");
                ImGui::Button("Duplicate Node");
                ImGui::Button("Rename Node");
                ImGui::Button("Edit Node");
                ImGui::Button("Create Node");
            }
            if (ImGui::CollapsingHeader("Instances")) {
                ImGui::Button("Add Instance");
                ImGui::Button("Remove Instance");
                ImGui::Button("Duplicate Instance");
                ImGui::Button("Override Instance Inherits");
                ImGui::Button("Rename Instance");
                ImGui::Button("Edit Instance");
                ImGui::Button("Create Instance");
            }
            if (ImGui::CollapsingHeader("Compositions")) {
                ImGui::Button("Create Composition");
                ImGui::Button("Duplicate Composition");
                ImGui::Button("Remove Composition");
                ImGui::Button("Rename Composition");
                ImGui::Button("Edit Composition");
            }
            if (ImGui::CollapsingHeader("Variants")) {
                ImGui::Button("Create Variants");
                ImGui::Button("Duplicate Variant");
            }
        }
        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::Button("Perspective On");
            ImGui::Button("Camera Mode");
            ImGui::Button("Camera Orbit");
            ImGui::Button("Camera Pan");
            ImGui::Button("Camera Zoom");
            ImGui::Button("Camera Track");
            ImGui::Button("Camera Walk");
            ImGui::Button("Camera Fly");
            ImGui::Button("Camera Free");
            ImGui::Button("Camera Target");
            ImGui::Button("Camera Follow");
        }
        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::Button("Transform Mode");
            ImGui::Button("Transform Translate");
            ImGui::Button("Transform Rotate");
            ImGui::Button("Transform Scale");
        }
        if (ImGui::CollapsingHeader("Grid")) {
            bool showViewport2DGrid = true;
            ImGui::Checkbox("Show Grid", &showViewport2DGrid);
            bool snapToViewport2DGrid = true;
            ImGui::Checkbox("Snap to Grid", &snapToViewport2DGrid);
        }
        if (ImGui::CollapsingHeader("Axes")) {
            bool showViewport2DAxes = true;
            ImGui::Checkbox("Show Axes", &showViewport2DAxes);
        }
        if (ImGui::CollapsingHeader("Helpers")) {
            ImGui::Button("Show Helpers");
        }
        if (ImGui::CollapsingHeader("Gizmos")) {
            ImGui::Button("Show Gizmos");
            ImGui::Button("Gizmo Mode");
            ImGui::Button("Gizmo Tools");
        }
        if (ImGui::CollapsingHeader("Selection")) {
            ImGui::Button("Selection Mode");
            ImGui::Button("Selection Tools");
        }
        if (ImGui::CollapsingHeader("Shading")) {
            ImGui::Button("Shading Mode");
            ImGui::Button("Shading Settings");
            int viewport3DAntiAliasing = 2;
            ImGui::SliderInt("AntiAliasing", &viewport3DAntiAliasing, 0, 2);
        }
        if (ImGui::CollapsingHeader("Lighting")) {
            ImGui::Button("Lighting Mode");
            ImGui::Button("Lighting Settings");
        }
        if (ImGui::CollapsingHeader("Fog")) {
            ImGui::Button("Fog Mode");
            ImGui::Button("Fog Settings");
        }
        if (ImGui::CollapsingHeader("Environment")) {
            ImGui::Button("Environment Mode");
            ImGui::Button("Environment Settings");
        }
        if (ImGui::CollapsingHeader("Skybox")) {
            ImGui::Button("Skybox Mode");
            ImGui::Button("Skybox Settings");
        }
        if (ImGui::CollapsingHeader("Rendering")) {
            ImGui::Button("Rendering Mode");
            ImGui::Button("Buffer Mode");
            ImGui::Button("Rendering Settings");
        }
        if (ImGui::CollapsingHeader("Post-Processing")) {
            ImGui::Button("Post-Processing Mode");
            ImGui::Button("Post-Processing Settings");
        }
        if (ImGui::CollapsingHeader("Effects")) {
            ImGui::Button("Effects Mode");
            ImGui::Button("Effects Settings");
        }
        if (ImGui::CollapsingHeader("Audio")) {
            ImGui::Button("Audio Mode");
            ImGui::Button("Audio Settings");
        }
        if (ImGui::CollapsingHeader("Physics")) {
            ImGui::Button("Physics Mode");
            ImGui::Button("Physics Settings");
        }
        if (ImGui::CollapsingHeader("Brush")) {
            ImGui::Button("Load Brush");
            ImGui::Button("Save Brush");
            float brushSizeMin = 0.0f, brushSizeMax = 100.0f, brushSize = 1.0f;
            ImGui::SliderFloat("Brush Size", &brushSize, brushSizeMin, brushSizeMax, nullptr, ImGuiSliderFlags_AlwaysClamp);
            float brushOpacityMin = 0.0f, brushOpacityMax = 1.0f, brushOpacity = 1.0f;
            ImGui::SliderFloat("Brush Opacity", &brushSize, brushSizeMin, brushSizeMax, nullptr, ImGuiSliderFlags_AlwaysClamp);
            float brushHardnessMin = 0.0f, brushHardnessMax = 1.0f, brushHardness = 1.0f;
            ImGui::SliderFloat("Brush Hardness", &brushHardness, brushHardnessMin, brushHardnessMax, nullptr, ImGuiSliderFlags_AlwaysClamp);
            ImGui::Button("Load Brush Alpha");
            float brushColor[3] = { 0.0f, 0.0f, 0.0f };
            ImGui::ColorEdit3("Brush Color", (float*)&brushColor, ImGuiColorEditFlags_Float);
            ImGui::Button("Brush Mode");
            ImGui::Button("Brush Tools");
        }
        if (ImGui::CollapsingHeader("UVs")) {
            bool showViewport2DUVs = true;
            ImGui::Checkbox("Show UVs", &showViewport2DUVs);
            ImGui::Button("UV Visibility Mode");
            ImGui::Button("UV Tools");
            ImGui::Button("UV Unwrap");
            ImGui::Button("Reproject Texture Keeping UVs");
            ImGui::Button("Add UV Set");
            ImGui::Button("Remove UV Set");
            ImGui::Button("Rename UV Set");
        }
        if (ImGui::CollapsingHeader("Navigation")) {
            ImGui::Button("Navigation Mode");
            ImGui::Button("Navigation Tools");
        }
        if (ImGui::CollapsingHeader("Timeline")) {
            ImGui::Button("Timeline Mode");
            ImGui::Button("Timeline Tools");
        }
        if (ImGui::CollapsingHeader("Animation")) {
            ImGui::Button("Animation Mode");
            ImGui::Button("Animation Tools");
        }
        if (ImGui::CollapsingHeader("Visibility")) {
            ImGui::Button("Visible Only");
            ImGui::Button("Visible Selected");
            ImGui::Button("Visible All");
            ImGui::Button("Visible Layers");
        }
        ImGui::End();

        // === Viewport 2D ===
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGui::Begin("Viewport 2D", nullptr);
        ImGui::End();

        // === Viewport 2D Tools ===
        // Most of the settings below should actually be in a separate floating, dockable tab, like "Viewport 2D Settings". Preferrably with a
        // semi-transparent background, to be automatically called as an overlay when the user is in the Viewport 2D tab. With each category
        // under separate headers (like "Brush Settings", "Grid Settings", etc). Or maybe even each category under separate semi-transparent tabs,
        // to be used as overlays, collapsed by default.
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Viewport 2D Tools", nullptr);

        if (ImGui::CollapsingHeader("Brush")) {
            ImGui::Button("Load Brush");
            ImGui::Button("Save Brush");
            float brushSizeMin = 0.0f, brushSizeMax = 100.0f, brushSize = 1.0f;
            ImGui::SliderFloat("Brush Size", &brushSize, brushSizeMin, brushSizeMax, nullptr, ImGuiSliderFlags_AlwaysClamp);
            float brushOpacityMin = 0.0f, brushOpacityMax = 1.0f, brushOpacity = 1.0f;
            ImGui::SliderFloat("Brush Opacity", &brushSize, brushSizeMin, brushSizeMax, nullptr, ImGuiSliderFlags_AlwaysClamp);
            float brushHardnessMin = 0.0f, brushHardnessMax = 1.0f, brushHardness = 1.0f;
            ImGui::SliderFloat("Brush Hardness", &brushHardness, brushHardnessMin, brushHardnessMax, nullptr, ImGuiSliderFlags_AlwaysClamp);
            ImGui::Button("Load Brush Alpha");
            float brushColor[3] = { 0.0f, 0.0f, 0.0f };
            ImGui::ColorEdit3("Brush Color", (float*)&brushColor, ImGuiColorEditFlags_Float);
            ImGui::Button("Brush Mode");
            ImGui::Button("Brush Tools");
        }
        if (ImGui::CollapsingHeader("UVs")) {
            bool showViewport2DUVs = true;
            ImGui::Checkbox("Show UVs", &showViewport2DUVs);
            ImGui::Button("UV Visibility Mode");
            ImGui::Button("UV Tools");
            ImGui::Button("UV Unwrap");
            ImGui::Button("Reproject Texture Keeping UVs");
            ImGui::Button("Add UV Set");
            ImGui::Button("Remove UV Set");
            ImGui::Button("Rename UV Set");
        }
        if (ImGui::CollapsingHeader("Grid")) {
            bool showViewport2DGrid = true;
            ImGui::Checkbox("Show Grid", &showViewport2DGrid);
            bool snapToViewport2DGrid = true;
            ImGui::Checkbox("Snap to Grid", &snapToViewport2DGrid);
        }
        if (ImGui::CollapsingHeader("Axes")) {
            bool showViewport2DAxes = true;
            ImGui::Checkbox("Show Axes", &showViewport2DAxes);
        }
        if (ImGui::CollapsingHeader("Helpers")) {
            ImGui::Button("Show Helpers");
        }
        ImGui::End();

        // === Properties Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Properties", nullptr);
        ImGui::Button("Load Template");
        ImGui::End();

        // === Asset Manager ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Asset Manager", nullptr);
        ImGui::Button("Import Asset");
        ImGui::Button("New Asset");
        ImGui::Button("Export Asset");
        ImGui::Button("Share Asset");
        ImGui::Button("Import Library");
        ImGui::Button("New Library");
        ImGui::Button("Library Settings");
        ImGui::Button("Add to Library");
        ImGui::Button("Remove from Library");
        ImGui::Button("Rename Library");
        ImGui::Button("Delete Library");
        ImGui::Button("Export Library");
        ImGui::Button("Share Library");
        ImGui::End();

        // === Lighting Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Lighting Editor", nullptr);
        ImGui::Button("Import Light Rig");
        ImGui::End();

        // === Physics Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Physics Editor", nullptr);
        ImGui::Button("Import Physics Rig");
        ImGui::End();

        // === Bottom status bar ===
        ImGui::Begin("Status Bar", nullptr);
        ImGui::Button("FPS");
        ImGui::End();

        // === File Explorer ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("File Explorer", nullptr);
        ImGui::Button("New...");
        ImGui::Button("Open...");
        ImGui::Button("Open As...");
        ImGui::Button("Open Recent");
        ImGui::Button("Close");
        ImGui::Button("Close All");
        ImGui::Button("Save");
        ImGui::Button("Save As...");
        ImGui::Button("Import");
        ImGui::Button("Export");
        ImGui::Button("Automate");
        ImGui::Button("File Info");
        ImGui::Button("Bookmarks");
        ImGui::End();

        // === Pattern Orchestrator ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Pattern Orchestrator", nullptr);
        ImGui::Button("Load Pattern");
        ImGui::End();

        // === Profiler ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Profiler", nullptr);
        bool profilerRecordingEnabled = true;
        ImGui::Checkbox("Recording Enabled", &profilerRecordingEnabled);
        ImGui::Button("Start Profiler");
        ImGui::Button("Pause Profiler");
        ImGui::Button("Stop Profiler");
        ImGui::Button("Clear Profiler");
        ImGui::Button("Export Profiler");
        ImGui::Button("Share Profiler");
        ImGui::Button("Save Profiler");
        ImGui::Button("Export Profiling Session");
        ImGui::Button("Profiler Settings");
        ImGui::End();

        // === Streaming Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Streaming", nullptr);
        bool streamingEnabled = true;
        ImGui::Checkbox("Streaming Enabled", &streamingEnabled);
        ImGui::Button("Start Streaming Engine");
        ImGui::Button("Refresh Streaming Engine");
        ImGui::Button("Streaming Engine Settings");
        ImGui::End();

        // === Image Editor ===
        // Used to display and edit textures, texture settings, edit UVs, packed textures, apply adjustments,
        // configure procedurals, use AI-assisted texture generation etc.
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Image Tools", nullptr);
        if (ImGui::CollapsingHeader("Image")) {
            ImGui::Button("Import Image");
            ImGui::Button("New Image");
            ImGui::Button("Save Image");
            ImGui::Button("Load Image");
            ImGui::Button("Export Image");
            ImGui::Button("Share Image");
        }
        if (ImGui::CollapsingHeader("Layers")) {
            ImGui::Button("Add Image as Layer");
            ImGui::Button("Add Layer");
            ImGui::Button("Duplicate Layer");
            ImGui::Button("Rename Layer");
            ImGui::Button("Delete Layer");
            ImGui::Button("Layer Settings");
        }
        if (ImGui::CollapsingHeader("Channels")) {
            ImGui::Button("Add Image as Channel");
            ImGui::Button("Add Channel");
            ImGui::Button("Duplicate Channel");
            ImGui::Button("Rename Channel");
        }
        if (ImGui::CollapsingHeader("Procedurals")) {
            ImGui::Button("Generate Procedural");
            ImGui::Button("Add Image as Procedural");
            ImGui::Button("Add Procedural");
            ImGui::Button("Duplicate Procedural");
            ImGui::Button("Rename Procedural");
            ImGui::Button("Delete Procedural");
            ImGui::Button("Procedural Settings");
        }
        if (ImGui::CollapsingHeader("Packed Texture")) {
            ImGui::Button("Pack Layers as Packed Texture");
            ImGui::Button("Pack Layers as Texture Set Node");
        }
        if (ImGui::CollapsingHeader("Adjustments")) {
            ImGui::Button("Add Adjustment");
            ImGui::Button("Duplicate Adjustment");
            ImGui::Button("Rename Adjustment");
            ImGui::Button("Delete Adjustment");
        }
        if (ImGui::CollapsingHeader("Image Generation")) {
            ImGui::Button("Generate Image");
            ImGui::Button("Upscale Image");
            ImGui::Button("Send to Texture Set Node");
            ImGui::Button("Upscale to Packed Texture");
            ImGui::Button("Send to Layer");
            ImGui::Button("Send to Channel");
            ImGui::Button("Send to Procedural");
            ImGui::Button("Synthesis Texture Set Node");
            ImGui::Button("Synthesis Packed Texture");
        }
        if (ImGui::CollapsingHeader("Texture Settings")) {
            ImGui::Button("Maximum Size");
            ImGui::Button("Mipmaps");
            ImGui::Button("Generate Mipmaps");
            ImGui::Button("Filtering");
            ImGui::Button("Anisotropic Filtering");
        }
        if (ImGui::CollapsingHeader("Texture Compression")) {
            ImGui::Button("Lossless Compression");
            ImGui::Button("Lossy Compression");
        }
        if (ImGui::CollapsingHeader("Image Sequence & Atlas")) {
            ImGui::Button("Import Image Sequence");
            ImGui::Button("Convert Layers to Atlas");
            ImGui::Button("Add Image to Atlas");
            ImGui::Button("Remove Image from Atlas");
        }
        ImGui::End();

        // === Audio Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Audio", nullptr);
        ImGui::Button("Import Audio");
        ImGui::Button("New Audio");
        ImGui::Button("Save Audio");
        ImGui::Button("Load Audio");
        ImGui::Button("Play Audio");
        ImGui::Button("Stop Audio");
        ImGui::Button("Export Audio");
        ImGui::Button("Share Audio");
        ImGui::Button("Add Audio");
        ImGui::Button("Remove Audio");
        ImGui::End();

        // === Montage Editor ===
        // Timeline with tracks, blending, transitions, etc. Used to compose animations, cutscenes, video clips, 
        // audio clips, images and other time varying media.
        // Whereas DCC apps usually rely on a simple timeline by default, Hydragon uses a more advanced montage editor,
        // with two visualization modes: collapsed and expanded (defaults to expanded).
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Montage", nullptr);
        ImGui::Button("Import Montage");
        ImGui::Button("New Montage");
        ImGui::Button("Save Montage");
        ImGui::Button("Load Montage");
        ImGui::Button("Play Montage");
        ImGui::Button("Stop Montage");
        ImGui::Button("Export Montage");
        ImGui::Button("Share Montage");
        ImGui::Button("Add Montage");
        ImGui::Button("Remove Montage");
        ImGui::End();

        // === Text Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Text", nullptr);
        ImGui::Button("Load Text");
        ImGui::End();

        // === Font Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Font", nullptr);
        ImGui::Button("Import Font");
        ImGui::Button("New Font");
        ImGui::Button("Save Font");
        ImGui::Button("Load Font");
        ImGui::Button("Export Font");
        ImGui::Button("Share Font");
        ImGui::Button("Add Font");
        ImGui::Button("Remove Font");
        ImGui::Button("Font Settings");
        ImGui::Button("Compile Font");
        ImGui::End();

        // === Plugin Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Plugin", nullptr);
        ImGui::Button("Import Plugin");
        ImGui::Button("Add Plugin");
        ImGui::Button("Save Plugin");
        ImGui::Button("Load Plugin");
        ImGui::Button("Unload Plugin");
        ImGui::Button("Reload Plugin");
        ImGui::Button("Export Plugin");
        ImGui::Button("Share Plugin");
        ImGui::Button("Remove Plugin");
        ImGui::Button("Plugin Settings");
        ImGui::End();

        // === Extensions Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Extensions", nullptr);
        ImGui::Button("Add Extension");
        ImGui::Button("Save Extension");
        ImGui::Button("Load Extension");
        ImGui::Button("Unload Extension");
        ImGui::Button("Reload Extension");
        ImGui::Button("Export Extension");
        ImGui::Button("Share Extension");
        ImGui::Button("Remove Extension");
        ImGui::Button("Extension Settings");
        ImGui::End();

        // === Macros Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Macros", nullptr);
        ImGui::Button("Load Macro");
        ImGui::End();
        //test
        // === Drama Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Drama", nullptr);
        ImGui::Button("Import Drama");
        ImGui::Button("New Drama");
        ImGui::Button("Save Drama");
        ImGui::Button("Load Drama");
        ImGui::Button("Play Drama");
        ImGui::Button("Stop Drama");
        ImGui::Button("Export Drama");
        ImGui::Button("Share Drama");
        ImGui::Button("Add Drama");
        ImGui::Button("Remove Drama");
        ImGui::Button("Drama Settings");
        ImGui::End();

        // === Chimera Pipeline Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Chimera Pipeline", nullptr);
        ImGui::Button("Start Server");
        ImGui::Button("Refresh Server");
        ImGui::Button("Stop Server");
        ImGui::Button("Import Chimera Pipeline");
        ImGui::Button("New Chimera Pipeline");
        ImGui::Button("Save Chimera Pipeline");
        ImGui::Button("Load Livelink");
        ImGui::Button("Watch Livelink");
        ImGui::Button("Synchronize");
        ImGui::End();

        // === UI Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("UI", nullptr);
        ImGui::Button("Import UI");
        ImGui::Button("New UI");
        ImGui::Button("Save UI");
        ImGui::Button("Load UI");
        ImGui::Button("Export UI");
        ImGui::Button("Share UI");
        ImGui::Button("Add UI");
        ImGui::Button("Remove UI");
        ImGui::Button("UI Settings");
        ImGui::End();

        // === Networking Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Networking", nullptr);
        ImGui::Button("Start Server");
        ImGui::Button("Refresh Server");
        ImGui::Button("Stop Server");
        ImGui::Button("Load Networking Config");
        ImGui::Button("Share Networking Config");
        ImGui::Button("Networking Settings");
        ImGui::Button("Start Client");
        ImGui::Button("Refresh Client");
        ImGui::Button("Stop Client");
        ImGui::Button("Load Client Config");
        ImGui::Button("Share Client Config");
        ImGui::Button("Client Settings");
        ImGui::Button("Export Client Config");
        ImGui::End();

        // === Performance Scalability Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Performance Scalability", nullptr);
        ImGui::Button("Import Performance Scalability Config");
        ImGui::Button("New Performance Scalability Config");
        ImGui::Button("Save Performance Scalability Config");
        ImGui::Button("Load Performance Scalability Config");
        ImGui::Button("Export Performance Scalability Config");
        ImGui::Button("Share Performance Scalability Config");
        ImGui::Button("Performance Scalability Settings");
        ImGui::End();

        // === Reflection Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Reflection", nullptr);
        ImGui::Button("Import Reflection Config");
        ImGui::Button("New Reflection Config");
        ImGui::Button("Save Reflection Config");
        ImGui::Button("Load Reflection Config");
        ImGui::Button("Export Reflection Config");
        ImGui::Button("Share Reflection Config");
        ImGui::Button("Reflection Settings");
        ImGui::End();

        // === Collaboration Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Collaboration", nullptr);
        ImGui::Button("Connect to Server");
        ImGui::Button("Disconnect from Server");
        ImGui::Button("Connect to Client Node");
        ImGui::Button("Disconnect from Client Node");
        ImGui::Button("Load Collaboration Config");
        ImGui::Button("Share Collaboration Config");
        ImGui::Button("Collaboration Settings");
        ImGui::End();

        // === Community Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Community", nullptr);
        ImGui::Button("Import Community Config");
        ImGui::Button("New Community Config");
        ImGui::Button("Save Community Config");
        ImGui::Button("Load Community Config");
        ImGui::Button("Export Community Config");
        ImGui::Button("Share Community Config");
        ImGui::Button("Report Issue");
        ImGui::Button("Report Bug");
        ImGui::Button("Ask Question");
        ImGui::Button("Community Settings");
        ImGui::Button("Request Feature");
        ImGui::Button("Request Support");
        ImGui::Button("Request Documentation");
        ImGui::Button("Request Training");
        ImGui::Button("Request Sponsorship");           // If the user has a Patreon account; have a project needing sponsorship
        ImGui::Button("Request Sponsorship Package");
        ImGui::Button("Request Sponsorship Support");
        ImGui::End();

        /// === Monetization Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Monetization", nullptr);
        ImGui::Button("Import Monetization Config");
        ImGui::Button("New Monetization Config");
        ImGui::Button("Save Monetization Config");
        ImGui::Button("Load Monetization Config");
        ImGui::Button("Export Monetization Config");
        ImGui::Button("Share Monetization Config");
        ImGui::Button("Monetization Settings");
        ImGui::Button("Connect Monetization Service");
        ImGui::Button("Connect to Sponsorship");
        ImGui::End();

        /// === Project Insights Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Project Insights", nullptr);
        ImGui::Button("Run Project Analysis");
        ImGui::Button("Stop Project Analysis");
        ImGui::Button("Send Analysis Results to Agent");
        ImGui::Button("Export Project Insights");
        ImGui::Button("Save Project Insights Config");
        ImGui::Button("Load Project Insights Config");
        ImGui::End();

        // === Input Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Input", nullptr);
        ImGui::Button("Import Input Config");
        ImGui::Button("New Input Config");
        ImGui::Button("Load Input Config");
        ImGui::Button("Save Input Config");
        ImGui::Button("Export Input Config");
        ImGui::Button("Share Input Config");
        ImGui::End();

        // === Properties Matrix Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Properties Matrix", nullptr);
        ImGui::Button("Load Selected Objects");
        ImGui::Button("Diff Selected");
        ImGui::End();

        // === Localization Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Localization", nullptr);
        ImGui::Button("Import Localization Config");
        ImGui::Button("New Localization Config");
        ImGui::Button("Load Localization Config");
        ImGui::Button("Save Localization Config");
        ImGui::Button("Export Localization Config");
        ImGui::Button("Share Localization Config");
        ImGui::Button("Localization Settings");
        ImGui::End();

        // === Settings Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Settings", nullptr);
        ImGui::Button("Save Settings");
        ImGui::End();

        // === Mesh Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Mesh", nullptr);
        ImGui::Button("Edit Mesh");
        ImGui::Button("Import Mesh");
        ImGui::Button("Load Mesh");
        ImGui::Button("Save Mesh");
        ImGui::Button("Procedural Tools");      // overlay, context menus
        ImGui::Button("Generate Mesh");
        ImGui::Button("Decimate Mesh");
        ImGui::Button("Voxelize Mesh");
        ImGui::Button("Simplify Mesh");
        ImGui::Button("Optimize Mesh");
        ImGui::Button("Mesh From Points");
        ImGui::Button("Mesh From Point Cloud");
        ImGui::Button("Export Mesh");
        ImGui::Button("Mesh Settings");
        ImGui::Button("Load Mesh Settings");
        ImGui::End();

        // === Volume Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Volume", nullptr);
        ImGui::Button("Import VDB Volume");
        ImGui::Button("New VDB Volume");
        ImGui::Button("Save Volume");
        ImGui::Button("Edit Volume");
        ImGui::Button("Load Volume");
        ImGui::Button("Procedural VDB Nodes");      // overlay, context menus
        ImGui::Button("Volumetric VDB Tools");          // overlay, context menus
        ImGui::Button("Volume Settings");
        ImGui::End();

        // === Preset Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Preset Editor", nullptr);
        ImGui::Button("Import Presets File");
        ImGui::Button("New Presets File");
        ImGui::Button("Save Presets File");
        ImGui::Button("Edit Presets");
        ImGui::Button("Load Presets File");
        ImGui::Button("Extract Presets from Current Config");      // overlay, context menus
        ImGui::End();

        // === Temporary, for referencing the components' names in imgui code files ===
        ImGui::ShowDemoWindow();
    }
} // namespace hdImgui