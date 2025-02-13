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
#include "SceneGraphEditor.h"
#include "NodeGraphEditor.h"
#include "ScriptEditor.h"
#include "ScriptsPalette.h"
#include "CommandsPalette.h"
#include "ConsoleEditor.h"
#include "AgentsEditor.h"
#if 0
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
        // SceneGraph Editor
        if (hdEditorWindowData.isSceneGraphWindowOpen) { hdImgui::ShowSceneGraphEditor(&hdEditorWindowData.isSceneGraphWindowOpen, &hdEditorWindowData); }
        // NodeGraph Editor
        if (hdEditorWindowData.isNodeGraphWindowOpen) { hdImgui::ShowNodeGraphEditor(&hdEditorWindowData.isNodeGraphWindowOpen, &hdEditorWindowData); }

        // === Script Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Commands Palette", nullptr);
        ImGui::Button("Run Command");
        ImGui::End();

        // === Console Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Console", nullptr);
        ImGui::Button("Clear Console");
        ImGui::Button("Run Command");
        ImGui::End();

        // === Agents Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        if (hdEditorWindowData.isViewport3DToolsWindowOpen)
        {
            ImGui::Begin("Viewport 3D Tools", &hdEditorWindowData.isViewport3DToolsWindowOpen, ImGuiWindowFlags_MenuBar);
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
                ImGui::Checkbox("Show Grid", &hdEditorWindowData.showViewport2DGrid);
                ImGui::Checkbox("Snap to Grid", &hdEditorWindowData.snapToViewport2DGrid);
            }
            if (ImGui::CollapsingHeader("Axes")) {
                ImGui::Checkbox("Show Axes", &hdEditorWindowData.showViewport2DAxes);
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
                ImGui::SliderInt("AntiAliasing", &hdEditorWindowData.viewport3D_AntiAliasing, 0, 8);
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
                ImGui::SliderFloat("Brush Size", &hdEditorWindowData.viewport3D_BrushSize, hdEditorWindowData.viewport3D_BrushMinSize, hdEditorWindowData.viewport3D_BrushMaxSize, nullptr, ImGuiSliderFlags_AlwaysClamp);
                ImGui::SliderFloat("Brush Opacity", &hdEditorWindowData.viewport3D_BrushOpacity, hdEditorWindowData.viewport3D_BrushMinOpacity, hdEditorWindowData.viewport3D_BrushMaxOpacity, nullptr, ImGuiSliderFlags_AlwaysClamp);
                ImGui::SliderFloat("Brush Hardness", &hdEditorWindowData.viewport3D_BrushHardness, hdEditorWindowData.viewport3D_BrushMinHardness, hdEditorWindowData.viewport3D_BrushMaxHardness, nullptr, ImGuiSliderFlags_AlwaysClamp);
                ImGui::Button("Load Brush Alpha");
                float brushColor[3] = { 0.0f, 0.0f, 0.0f };
                ImGui::ColorEdit3("Brush Color", &hdEditorWindowData.viewport3D_BrushColor[0], ImGuiColorEditFlags_Float);
                ImGui::Button("Brush Mode");
                ImGui::Button("Brush Tools");
            }
            if (ImGui::CollapsingHeader("UVs")) {
                ImGui::Checkbox("Show UVs", &hdEditorWindowData.showViewport2DUVs);
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
        }
        

        // === Viewport 2D ===
        ImGui::SetNextWindowBgAlpha(1.0f);
        ImGui::Begin("Viewport 2D", nullptr);
        ImGui::End();

        // === Viewport 2D Tools ===
        // Most of the settings below should actually be in a separate floating, dockable tab, like "Viewport 2D Settings". Preferrably with a
        // semi-transparent background, to be automatically called as an overlay when the user is in the Viewport 2D tab. With each category
        // under separate headers (like "Brush Settings", "Grid Settings", etc). Or maybe even each category under separate semi-transparent tabs,
        // to be used as overlays, collapsed by default.
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
            ImGui::Checkbox("Show UVs", &hdEditorWindowData.showViewport2DUVs);
            ImGui::Button("UV Visibility Mode");
            ImGui::Button("UV Tools");
            ImGui::Button("UV Unwrap");
            ImGui::Button("Reproject Texture Keeping UVs");
            ImGui::Button("Add UV Set");
            ImGui::Button("Remove UV Set");
            ImGui::Button("Rename UV Set");
        }
        if (ImGui::CollapsingHeader("Grid")) {
            ImGui::Checkbox("Show Grid", &hdEditorWindowData.showViewport2DGrid);
            ImGui::Checkbox("Snap to Grid", &hdEditorWindowData.snapToViewport2DGrid);
        }
        if (ImGui::CollapsingHeader("Axes")) {
            ImGui::Checkbox("Show Axes", &hdEditorWindowData.showViewport2DAxes);
        }
        if (ImGui::CollapsingHeader("Helpers")) {
            ImGui::Button("Show Helpers");
        }
        ImGui::End();

        // === Properties Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Properties", nullptr);
        ImGui::Button("Load Template");
        ImGui::End();

        // === Asset Manager ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Lighting Editor", nullptr);
        ImGui::Button("Import Light Rig");
        ImGui::End();

        // === Physics Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Physics Editor", nullptr);
        ImGui::Button("Import Physics Rig");
        ImGui::End();

        // === Bottom status bar ===
        ImGui::Begin("Status Bar", nullptr);
        ImGui::Button("FPS");
        ImGui::End();

        // === File Explorer ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Pattern Orchestrator", nullptr);
        ImGui::Button("Load Pattern");
        ImGui::End();

        // === Profiler ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Profiler", nullptr);
        ImGui::Checkbox("Recording Enabled", &hdEditorWindowData.profilerRecordingEnabled);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Streaming", nullptr);
        ImGui::Checkbox("Streaming Enabled", &hdEditorWindowData.streamingEnabled);
        ImGui::Button("Start Streaming Engine");
        ImGui::Button("Refresh Streaming Engine");
        ImGui::Button("Streaming Engine Settings");
        ImGui::End();

        // === Image Editor ===
        // Used to display and edit textures, texture settings, edit UVs, packed textures, apply adjustments,
        // configure procedurals, use AI-assisted texture generation etc.
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Text", nullptr);
        ImGui::Button("Load Text");
        ImGui::End();

        // === Font Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Macros", nullptr);
        ImGui::Button("Load Macro");
        ImGui::End();
        //test
        // === Drama Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Project Insights", nullptr);
        ImGui::Button("Run Project Analysis");
        ImGui::Button("Stop Project Analysis");
        ImGui::Button("Send Analysis Results to Agent");
        ImGui::Button("Export Project Insights");
        ImGui::Button("Save Project Insights Config");
        ImGui::Button("Load Project Insights Config");
        ImGui::End();

        // === Input Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Input", nullptr);
        ImGui::Button("Import Input Config");
        ImGui::Button("New Input Config");
        ImGui::Button("Load Input Config");
        ImGui::Button("Save Input Config");
        ImGui::Button("Export Input Config");
        ImGui::Button("Share Input Config");
        ImGui::End();

        // === Properties Matrix Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Properties Matrix", nullptr);
        ImGui::Button("Load Selected Objects");
        ImGui::Button("Diff Selected");
        ImGui::End();

        // === Localization Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
        ImGui::Begin("Settings", nullptr);
        ImGui::Button("Save Settings");
        ImGui::End();

        // === Mesh Editor ===
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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
        ImGui::SetNextWindowBgAlpha(hdEditorWindowData.globalWindowBgAlpha);
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