/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include <algorithm>  // for std::clamp
#include "misc/cpp/imgui_stdlib.h"  // Add this include for std::string support

#include "MontageEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

// Forward declarations
static void ShowMontageToolbar(HdEditorWindowData* windowData);
static void ShowTrackView(HdEditorWindowData* windowData, bool isCollapsed);
static void ShowPropertyPanel(HdEditorWindowData* windowData);

struct MontageTrack {
    std::string name;
    int type;
    bool isVisible = true;
    bool isLocked = false;
    bool isMuted = false;
    bool isSolo = false;
    std::vector<int> clips;  // Will be replaced with proper clip structure
};

struct MontageState {
    // View states
    bool isCollapsedView = false;
    bool showWaveform = true;
    bool snapToGrid = true;
    
    // Time control
    float currentTime = 0.0f;
    float startTime = 0.0f;
    float endTime = 100.0f;
    float frameRate = 30.0f;
    bool isPlaying = false;
    bool isLooping = true;
    
    // Editor settings
    float gridSize = 1.0f;
    int selectedTrack = -1;
    int selectedClip = -1;
    
    // Tracks
    std::vector<MontageTrack> tracks;
};

static MontageState state;

static void ShowMontageToolbar(HdEditorWindowData* windowData)
{
    // Style settings for a compact toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    
    // Left section - Track operations
    ImVec2 buttonSize = windowData->iconDefaultSize;
    
    if (ImGui::Button(ICON_MS_ADD "##NewTrack", buttonSize)) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Track");
    
    ImGui::SameLine();
    if (ImGui::Button(ICON_MS_DELETE "##DeleteTrack", buttonSize)) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Delete Track");
    
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_CONTENT_CUT "##Split", buttonSize)) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Split");
    
    // Add spacing between tools and time controls
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(10, 0));
    ImGui::SameLine();
    
    // Time Controls
    ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));
    
    if (ImGui::Button(ICON_MS_SKIP_PREVIOUS "##StepBack", buttonSize)) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Step Backward");
    ImGui::SameLine();
    
    if (ImGui::Button(state.isPlaying ? ICON_MS_PAUSE : ICON_MS_PLAY_ARROW "##PlayPause", buttonSize))
        state.isPlaying = !state.isPlaying;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(state.isPlaying ? "Pause" : "Play");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_SKIP_NEXT "##StepForward", buttonSize)) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Step Forward");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_STOP "##Stop", buttonSize))
        state.isPlaying = false;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Stop");
    ImGui::SameLine();
    
    ImGui::Dummy(ImVec2(5, 0)); 
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_LOOP "##Loop", buttonSize))
        state.isLooping = !state.isLooping;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Toggle Loop");
    
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(10, 0));
    ImGui::SameLine();
    
    // Timeline with scrub - now taller to match button height and filling remaining width
    float remainingWidth = ImGui::GetContentRegionAvail().x - 75.0f; // Reserve space for time input
    float timelineHeight = buttonSize.y;  // Match button height
    
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, (timelineHeight - ImGui::GetFrameHeight()) * 0.5f));
    ImGui::PushItemWidth(remainingWidth);
    ImGui::SliderFloat("##timeline", &state.currentTime, state.startTime, state.endTime, "%.2f");
    ImGui::PopItemWidth();
    ImGui::PopStyleVar();
    
    ImGui::SameLine();
    ImGui::SetNextItemWidth(70);
    if (ImGui::InputFloat("##Time", &state.currentTime, 1.0f, 10.0f, "%.2f"))
    {
        state.currentTime = std::clamp(state.currentTime, state.startTime, state.endTime);
    }
    
    ImGui::PopStyleColor();
    ImGui::PopStyleVar(2);
}

static void ShowTrackView(HdEditorWindowData* windowData, bool isCollapsed)
{
    ImGui::Text("Track View - Coming Soon");
    ImGui::Separator();
    
    // Placeholder for tracks list
    for (size_t i = 0; i < state.tracks.size(); i++)
    {
        ImGui::PushID(static_cast<int>(i));
        
        if (ImGui::Selectable(state.tracks[i].name.c_str(), state.selectedTrack == static_cast<int>(i)))
        {
            state.selectedTrack = static_cast<int>(i);
        }
        
        ImGui::PopID();
    }
    
    // Add track button
    if (ImGui::Button("Add Track"))
    {
        MontageTrack newTrack;
        newTrack.name = "Track " + std::to_string(state.tracks.size() + 1);
        state.tracks.push_back(newTrack);
    }
}

void ShowMontageEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    if (!ImGui::Begin("Montage Editor##MainWindow", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save As", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Exit")) {}
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
            ImGui::MenuItem("Waveform", nullptr, &state.showWaveform);
            ImGui::MenuItem("Snap to Grid", nullptr, &state.snapToGrid);
            ImGui::MenuItem("Collapsed View", nullptr, &state.isCollapsedView);
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }

    // Main content
    ImGui::PushID("MontageContent");
    {
        ImGui::BeginChild("MontageMainContent", ImVec2(0, 0), false);
        
        ShowMontageToolbar(windowData);  // Now includes time controls
        
        ImGui::BeginChild("TrackViewArea", ImVec2(0, 0), true);
        ShowTrackView(windowData, state.isCollapsedView);
        ImGui::EndChild();
        
        ImGui::EndChild();
    }
    ImGui::PopID();

    ImGui::End();
}

static void ShowPropertyPanel(HdEditorWindowData* windowData)
{
    ImGui::Text("Properties - Coming Soon");
    ImGui::Separator();
    
    if (state.selectedTrack >= 0 && state.selectedTrack < static_cast<int>(state.tracks.size()))
    {
        MontageTrack& track = state.tracks[state.selectedTrack];
        
        // Basic track properties
        ImGui::InputText("Name", &track.name);  // Now works with std::string
        ImGui::Checkbox("Visible", &track.isVisible);
        ImGui::Checkbox("Locked", &track.isLocked);
        ImGui::Checkbox("Muted", &track.isMuted);
        ImGui::Checkbox("Solo", &track.isSolo);
    }
    else
    {
        ImGui::TextDisabled("No track selected");
    }
}

} // namespace hdImgui
