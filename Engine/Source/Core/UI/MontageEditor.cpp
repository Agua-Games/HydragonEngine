/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "MontageEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {
// Forward declarations
static void ShowMontageToolbar(HdEditorWindowData* windowData);
static void ShowTimeControls(HdEditorWindowData* windowData);
static void ShowTrackView(HdEditorWindowData* windowData, bool isCollapsed);
static void ShowPropertyPanel(HdEditorWindowData* windowData);
static void ShowNodeGraph(HdEditorWindowData* windowData);

// State management - declare at namespace scope
struct MontageState {
    bool isCollapsedView = false;
    float currentTime = 0.0f;
    float startTime = 0.0f;
    float endTime = 100.0f;
    float frameRate = 30.0f;
    bool isPlaying = false;
    bool isLooping = true;
    bool showWaveform = true;
    bool snapToGrid = true;
    float gridSize = 1.0f;
    int selectedTrack = -1;
    int selectedClip = -1;
    bool showNodeGraph = true;
};

static MontageState state;  // Static instance at namespace scope

void ShowMontageEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Montage Editor", p_open, 
        ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Montage", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Import"))
            {
                if (ImGui::MenuItem("Animation...")) {}
                if (ImGui::MenuItem("Audio...")) {}
                if (ImGui::MenuItem("Physics Cache...")) {}
                if (ImGui::MenuItem("Video...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Export"))
            {
                if (ImGui::MenuItem("To USD...")) {}
                if (ImGui::MenuItem("To Alembic...")) {}
                if (ImGui::MenuItem("To FBX...")) {}
                if (ImGui::MenuItem("To Video...")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
            if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
            if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Collapsed View", nullptr, &state.isCollapsedView);
            ImGui::MenuItem("Show Node Graph", nullptr, &state.showNodeGraph);
            ImGui::MenuItem("Show Waveform", nullptr, &state.showWaveform);
            ImGui::MenuItem("Show Grid", nullptr, &state.snapToGrid);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Toolbar with integrated time controls
    ShowMontageToolbar(windowData);

    // Main content area
    ImGui::BeginChild("MontageContent", ImVec2(0, 0), false);
    {
        // Split view: Track View and Node Graph
        float nodeGraphHeight = state.showNodeGraph ? ImGui::GetContentRegionAvail().y * 0.4f : 0;
        
        // Track View
        ImGui::BeginChild("TrackView", ImVec2(0, ImGui::GetContentRegionAvail().y - nodeGraphHeight));
        ShowTrackView(windowData, state.isCollapsedView);
        ImGui::EndChild();

        // Node Graph (if enabled)
        if (state.showNodeGraph)
        {
            ImGui::BeginChild("NodeGraph", ImVec2(0, nodeGraphHeight));
            ShowNodeGraph(windowData);
            ImGui::EndChild();
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

static void ShowMontageToolbar(HdEditorWindowData* windowData)
{
    const float toolbarHeight = 30.0f;
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
    
    if (ImGui::BeginChild("MontageToolbar", ImVec2(-1, toolbarHeight), true, 
        ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));

        // Left side - Editor tools
        {
            float startX = ImGui::GetWindowPos().x + 4.0f;
            ImGui::SetCursorScreenPos(ImVec2(startX, ImGui::GetWindowPos().y + 2.0f));

            // Track Management Section
            if (ImGui::Button(ICON_MS_ADD "##New", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Track");
            ImGui::SameLine();
            
            if (ImGui::Button(ICON_MS_CONTENT_CUT "##Split", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Split Clip");
            ImGui::SameLine();
            
            if (ImGui::Button(ICON_MS_MERGE_TYPE "##Merge", windowData->iconDefaultSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Merge Clips");
            ImGui::SameLine();
            
            ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

            // View Controls Section
            if (ImGui::Button(ICON_MS_TIMELINE "##ToggleView", windowData->iconDefaultSize)) 
                state.isCollapsedView = !state.isCollapsedView;
            if (ImGui::IsItemHovered()) 
                ImGui::SetTooltip(state.isCollapsedView ? "Expand View" : "Collapse View");
            ImGui::SameLine();
            
            if (ImGui::Button(ICON_MS_ACCOUNT_TREE "##ToggleNode", windowData->iconDefaultSize))
                state.showNodeGraph = !state.showNodeGraph;
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip(state.showNodeGraph ? "Hide Node Graph" : "Show Node Graph");
            ImGui::SameLine();

            ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();

            // Grid and Snapping Section
            if (ImGui::Button(ICON_MS_GRID_ON "##ToggleGrid", windowData->iconDefaultSize))
                state.snapToGrid = !state.snapToGrid;
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip(state.snapToGrid ? "Disable Grid Snap" : "Enable Grid Snap");
        }

        // Center - Time Controls
        {
            float windowWidth = ImGui::GetWindowWidth();
            float controlsWidth = 200.0f; // Adjust based on your needs
            float startX = ImGui::GetWindowPos().x + (windowWidth - controlsWidth) * 0.5f;
            
            ImGui::SetCursorScreenPos(ImVec2(startX, ImGui::GetWindowPos().y + 2.0f));
            
            // Transport controls with consistent styling
            if (ImGui::Button(ICON_MS_SKIP_PREVIOUS "##First", windowData->iconDefaultSize)) 
                state.currentTime = state.startTime;
            if (ImGui::IsItemHovered()) 
                ImGui::SetTooltip("Jump to Start");
            
            ImGui::SameLine();
            if (ImGui::Button(state.isPlaying ? ICON_MS_PAUSE : ICON_MS_PLAY_ARROW "##Play", windowData->iconDefaultSize))
                state.isPlaying = !state.isPlaying;
            if (ImGui::IsItemHovered()) 
                ImGui::SetTooltip(state.isPlaying ? "Pause" : "Play");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_SKIP_NEXT "##Last", windowData->iconDefaultSize))
                state.currentTime = state.endTime;
            if (ImGui::IsItemHovered()) 
                ImGui::SetTooltip("Jump to End");
            
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(10, 0));
            ImGui::SameLine();
            
            // Loop toggle
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
            ImGui::Checkbox("Loop", &state.isLooping);
            ImGui::PopStyleVar();
        }

        ImGui::PopStyleColor();
        ImGui::PopStyleVar(4);
    }
    ImGui::EndChild();

    // Time slider below toolbar
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##Timeline", &state.currentTime, state.startTime, state.endTime);
    ImGui::PopItemWidth();
}

static void ShowTimeControls(HdEditorWindowData* windowData)
{
    // Transport controls container
    const float controlsHeight = 30.0f;
    ImGui::BeginChild("TransportControls", ImVec2(-1, controlsHeight), true);
    
    // Consistent styling
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 0));
    
    // Button styling
    ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));
    
    // Transport controls
    if (ImGui::Button(ICON_MS_SKIP_PREVIOUS "##First", windowData->iconDefaultSize)) 
        state.currentTime = state.startTime;
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip("Jump to Start");
    
    ImGui::SameLine();
    if (ImGui::Button(state.isPlaying ? ICON_MS_PAUSE : ICON_MS_PLAY_ARROW "##Play", windowData->iconDefaultSize))
        state.isPlaying = !state.isPlaying;
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip(state.isPlaying ? "Pause" : "Play");
    
    ImGui::SameLine();
    if (ImGui::Button(ICON_MS_SKIP_NEXT "##Last", windowData->iconDefaultSize))
        state.currentTime = state.endTime;
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip("Jump to End");
    
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(10, 0));
    ImGui::SameLine();
    
    // Loop toggle with consistent styling
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    ImGui::Checkbox("Loop", &state.isLooping);
    ImGui::PopStyleVar();
    
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
    ImGui::EndChild();

    // Time slider and numeric input
    ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x);
    ImGui::SliderFloat("##Timeline", &state.currentTime, state.startTime, state.endTime);
    ImGui::PopItemWidth();
}

static void ShowTrackView(HdEditorWindowData* windowData, bool isCollapsed)
{
    static const char* trackTypes[] = {
        "Animation", "Audio", "Physics", "Particles",
        "Camera", "Lighting", "Material", "Custom"
    };

    ImGui::BeginChild("Tracks", ImVec2(0, 0), true);
    
    if (isCollapsed)
    {
        // Collapsed view - single track with merged content
        ImGui::Text("Master Track");
        // TODO: Implement collapsed view visualization
    }
    else
    {
        // Expanded view - multiple tracks with detailed control
        for (int i = 0; i < IM_ARRAYSIZE(trackTypes); i++)
        {
            bool isSelected = (state.selectedTrack == i);
            if (ImGui::Selectable(trackTypes[i], isSelected))
                state.selectedTrack = i;

            if (ImGui::BeginDragDropTarget())
            {
                // TODO: Implement drag & drop for clips
                ImGui::EndDragDropTarget();
            }
        }
    }
    
    ImGui::EndChild();
}

static void ShowNodeGraph(HdEditorWindowData* windowData)
{
    ImGui::BeginChild("Nodes", ImVec2(0, 0), true);
    ImGui::Text("Node Graph View");
    // TODO: Implement node graph visualization and editing
    // This will be connected to ImNodes or a similar node editor system
    ImGui::EndChild();
}

} // namespace hdImgui
