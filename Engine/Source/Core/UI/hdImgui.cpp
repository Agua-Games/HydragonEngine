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
#include "CommandsConsole.h"
#include "AgentsEditor.h"
#include "Viewport3D.h"
#include "Viewport2D.h"
#include "PropertyEditor.h"
#include "AssetManager.h"
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
#include "LocalizationEditor.h"
#include "SettingsEditor.h"
#endif

namespace hdImgui {
    // === Input variables ===
    // Static variables for sleep/idle functionality
    bool s_isSleeping = false;
    static std::chrono::steady_clock::time_point s_lastInteractionTime;

    // === Styling variables ===
    float globalWindowBgAlpha = 0.35f;

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
        style.Colors[ImGuiCol_PopupBg]                = ImVec4(0.35f, 0.35f, 0.35f, 1.00f);
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
        ImGui::BeginMenuBar();
        //ImGui::EndMenuBar();

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
        ImGui::Begin("SceneGraph ", nullptr);
        ImGui::Button("Add Layer");

        // Example: TreeNode with flags
        ImGuiTreeNodeFlags SceneGraphTreeNodeFlags = 
            ImGuiTreeNodeFlags_DefaultOpen | 
            ImGuiTreeNodeFlags_Framed | 
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (ImGui::TreeNodeEx("Castle", SceneGraphTreeNodeFlags)) {
            // Child nodes
            if (ImGui::TreeNode("Child Node 1")) {
                ImGui::Text("Tower 1");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Tower 2")) {
                ImGui::Text("This is the second child node.");
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
        ImGui::End(); 
        
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
        ImGui::Button("Add Node");
        ImGui::End();

        // === Scripting Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Scripting", nullptr);
        ImGui::Button("Run Script");
        ImGui::End();

        // === Scripts Palette ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Scripts Palette", nullptr);   
        ImGui::Button("Add Script"); 
        ImGui::Button("My Macro Script 1"); 
        ImGui::Button("My Macro Script 2"); 
        ImGui::End();

        // === Commands Console ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Commands Console", nullptr);
        ImGui::Button("Run Command");
        ImGui::End();

        // === Agents Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Agents", nullptr);
        ImGui::Button("Add Agent");
        ImGui::Button("Load Model");
        ImGui::Button("Start Server");
        ImGui::End();

        // === Viewports ===
        ImGui::Begin("Viewport 3D");
        ImGui::Button("Shading Mode");
        ImGui::End();

        ImGui::Begin("Viewport 2D");
        ImGui::Button("Brush Size");
        ImGui::End();

        // === Properties Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Properties", nullptr);
        ImGui::Button("Load Template");
        ImGui::End();

        // === Asset Manager ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Asset Manager", nullptr);
        ImGui::Button("Load Asset");
        ImGui::End();

        // === Bottom status bar ===
        ImGui::Begin("Status Bar", nullptr);
        ImGui::Button("FPS");
        ImGui::End();

        // === File Explorer ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("File Explorer", nullptr);
        ImGui::Button("Load File");
        ImGui::End();

        // === Pattern Orchestrator ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Pattern Orchestrator", nullptr);
        ImGui::Button("Load Pattern");
        ImGui::End();

        // === Profiler ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Profiler", nullptr);
        ImGui::Button("Clear");
        ImGui::End();

        // === Streaming Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Streaming", nullptr);
        ImGui::Button("Start Streaming");
        ImGui::End();

        // === Image Editor ===
        // Used to display and edit textures, texture settings, edit UVs, packed textures, apply adjustments,
        // configure procedurals, use AI-assisted texture generation etc.
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Image", nullptr);
        ImGui::Button("Load Texture");
        ImGui::End();

        // === Audio Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Audio", nullptr);
        ImGui::Button("Load Audio");
        ImGui::End();

        // === Montage Editor ===
        // Timeline with tracks, blending, transitions, etc. Used to compose animations, cutscenes, video clips, 
        // audio clips, images and other time varying media.
        // Whereas DCC apps usually rely on a simple timeline by default, Hydragon uses a more advanced montage editor,
        // with two visualization modes: collapsed and expanded (defaults to expanded).
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Montage", nullptr);
        ImGui::Button("Load Video");
        ImGui::End();

        // === Text Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Text", nullptr);
        ImGui::Button("Load Text");
        ImGui::End();

        // === Font Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Font", nullptr);
        ImGui::Button("Load Font");
        ImGui::End();

        // === Plugin Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Plugin", nullptr);
        ImGui::Button("Load Plugin");
        ImGui::End();

        // === Extensions Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Extensions", nullptr);
        ImGui::Button("Load Extension");
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
        ImGui::Button("Load Drama");
        ImGui::End();

        // === Chimera Pipeline Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Chimera Pipeline", nullptr);
        ImGui::Button("Load Livelink");
        ImGui::End();

        // === UI Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("UI", nullptr);
        ImGui::Button("Load UI");
        ImGui::End();

        // === Networking Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Networking", nullptr);
        ImGui::Button("Load Networking Config");
        ImGui::End();

        // === Performance Scalability Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Performance Scalability", nullptr);
        ImGui::Button("Load Performance Scalability Config");
        ImGui::End();

        // === Reflection Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Reflection", nullptr);
        ImGui::Button("Load Reflection Config");
        ImGui::End();

        // === Collaboration Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Collaboration", nullptr);
        ImGui::Button("Load Collaboration Config");
        ImGui::End();

        // === Community Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Community", nullptr);
        ImGui::Button("Load Community Config");
        ImGui::End();

        /// === Project Insights Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Project Insights", nullptr);
        ImGui::Button("Load Project Insights Config");
        ImGui::End();

        // === Input Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Input", nullptr);
        ImGui::Button("Load Input Config");
        ImGui::End();

        // === Localization Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Localization", nullptr);
        ImGui::Button("Load Localization Config");
        ImGui::End();

        // === Settings Editor ===
        ImGui::SetNextWindowBgAlpha(globalWindowBgAlpha);
        ImGui::Begin("Settings", nullptr);
        ImGui::Button("Save Settings");
        ImGui::End();

        // === Temporary, for referencing the components' names in imgui code files ===
        ImGui::ShowDemoWindow();
    }
} // namespace hdImgui