/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "DramaEditor.h"
#include "hdImgui.h"

struct DramaEditorState {
    bool showBrowser = true;
    bool showProperties = true;
    bool showTimeline = true;
    bool showDebug = false;
    
    // Current selection/focus
    int selectedDramaSequence = -1;
    int selectedNode = -1;
    float currentTime = 0.0f;
    
    // View states
    bool isPlaying = false;
    bool showTensionCurve = true;
    bool showStateSpace = false;
    bool showPreview = false;
};

static DramaEditorState state;

// Forward declarations of the panel rendering functions
static void ShowDramaBrowser()
{
    ImGui::Text("Drama Browser");
    // Placeholder until Phase 2
}

static void ShowMainWorkspace()
{
    ImGui::Text("Main Workspace");
    // Placeholder until Phase 3
}

static void ShowPropertiesPanel()
{
    ImGui::Text("Properties Panel");
    // Placeholder until Phase 7
}

static void ShowTimeline()
{
    ImGui::Text("Timeline");
    // Placeholder until Phase 6
}

namespace hdImgui {
void ShowDramaEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Drama Editor", p_open, 
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
            if (ImGui::MenuItem("New Drama Sequence", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Import"))
            {
                if (ImGui::MenuItem("Drama Template...")) {}
                if (ImGui::MenuItem("State Space...")) {}
                if (ImGui::MenuItem("Tensor Config...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Export"))
            {
                if (ImGui::MenuItem("As Template...")) {}
                if (ImGui::MenuItem("State Configuration...")) {}
                if (ImGui::MenuItem("Analytics Data...")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Duplicate Sequence", "Ctrl+D")) {}
            if (ImGui::MenuItem("Delete", "Del")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Drama Browser", nullptr, &state.showBrowser);
            ImGui::MenuItem("Properties Panel", nullptr, &state.showProperties);
            ImGui::MenuItem("Timeline", nullptr, &state.showTimeline);
            ImGui::MenuItem("Debug Overlay", nullptr, &state.showDebug);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Main Layout
    const float browserWidth = 250.0f;
    const float propertiesWidth = 300.0f;
    const float timelineHeight = 150.0f;

    // Left Panel - Drama Browser
    if (state.showBrowser)
    {
        ImGui::BeginChild("DramaBrowser", ImVec2(browserWidth, -timelineHeight), true);
        ShowDramaBrowser();
        ImGui::EndChild();
    }

    ImGui::SameLine();

    // Center Panel - Main Workspace
    ImGui::BeginChild("MainWorkspace", 
        ImVec2(-propertiesWidth - 8.0f, -timelineHeight), true);
    ShowMainWorkspace();
    ImGui::EndChild();

    ImGui::SameLine();

    // Right Panel - Properties
    if (state.showProperties)
    {
        ImGui::BeginChild("Properties", ImVec2(0, -timelineHeight), true);
        ShowPropertiesPanel();
        ImGui::EndChild();
    }

    // Bottom Panel - Timeline
    if (state.showTimeline)
    {
        ImGui::BeginChild("Timeline", ImVec2(0, 0), true);
        ShowTimeline();
        ImGui::EndChild();
    }

    ImGui::End();
}
} // namespace hdImgui
