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
    std::string name;  // Using std::string since imgui_stdlib.h is available
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
    
    // Add spacing between tools and fold/expand toggle
    ImGui::SameLine();
    ImGui::Dummy(ImVec2(10, 0));
    ImGui::SameLine();
    
    // Fold/Expand toggle
    if (ImGui::Button(state.isCollapsedView ? ICON_MS_UNFOLD_MORE : ICON_MS_UNFOLD_LESS "##FoldExpand", buttonSize))
        state.isCollapsedView = !state.isCollapsedView;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip(state.isCollapsedView ? "Expand View" : "Collapse View");
    
    // Add spacing between fold toggle and time controls
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
    
    // Timeline with scrub
    float remainingWidth = ImGui::GetContentRegionAvail().x - 75.0f;
    float timelineHeight = buttonSize.y;
    
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
    const float trackHeight = 24.0f;
    static float headerWidth = 200.0f;  // Now static to preserve width between frames
    const float minHeaderWidth = 150.0f;  // Minimum width
    const float maxHeaderWidth = 400.0f;  // Maximum width
    const ImVec2 buttonSize(20, 20);
    
    // Track headers (names and controls)
    ImGui::BeginChild("TrackHeaders", ImVec2(headerWidth, 0), true);
    for (size_t i = 0; i < state.tracks.size(); i++)
    {
        ImGui::PushID(static_cast<int>(i));
        
        MontageTrack& track = state.tracks[i];
        bool isSelected = state.selectedTrack == static_cast<int>(i);
        
        ImGui::BeginGroup();
        // Track name - using std::string with imgui_stdlib.h
        ImGui::SetNextItemWidth(100);
        ImGui::InputText("##name", &track.name);
        
        ImGui::SameLine();
        // Track controls
        if (ImGui::Button(track.isSolo ? ICON_MS_VOLUME_UP : ICON_MS_VOLUME_OFF "##solo", buttonSize))
            track.isSolo = !track.isSolo;
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Solo");
        
        ImGui::SameLine();
        if (ImGui::Button(track.isMuted ? ICON_MS_VOLUME_MUTE : ICON_MS_VOLUME_UP "##mute", buttonSize))
            track.isMuted = !track.isMuted;
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Mute");
        
        ImGui::SameLine();
        if (ImGui::Button(track.isLocked ? ICON_MS_LOCK : ICON_MS_LOCK_OPEN "##lock", buttonSize))
            track.isLocked = !track.isLocked;
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Lock");
        
        ImGui::EndGroup();
        
        ImGui::PopID();
        ImGui::Dummy(ImVec2(0, 4)); // Spacing between tracks
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Splitter
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
    ImGui::Button("##Splitter", ImVec2(8.0f, -1));
    ImGui::PopStyleColor(3);

    if (ImGui::IsItemActive())
    {
        float deltaX = ImGui::GetIO().MouseDelta.x;
        headerWidth += deltaX;
        if (headerWidth < minHeaderWidth) headerWidth = minHeaderWidth;
        if (headerWidth > maxHeaderWidth) headerWidth = maxHeaderWidth;
    }
    else if (ImGui::IsItemHovered())
    {
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    }
    
    ImGui::SameLine();
    
    // Track timelines
    ImGui::BeginChild("TrackTimelines", ImVec2(0, 0), true);
    float timelineWidth = ImGui::GetContentRegionAvail().x;
    
    for (size_t i = 0; i < state.tracks.size(); i++)
    {
        ImGui::PushID(static_cast<int>(i));
        
        // Track timeline bar
        ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.2f, 0.2f, 0.2f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, ImVec4(0.3f, 0.3f, 0.3f, 1.0f));
        ImGui::PushStyleColor(ImGuiCol_FrameBgActive, ImVec4(0.4f, 0.4f, 0.4f, 1.0f));
        
        ImGui::Button("##timeline", ImVec2(timelineWidth, trackHeight));
        
        ImGui::PopStyleColor(3);
        ImGui::PopID();
        
        ImGui::Dummy(ImVec2(0, 4)); // Spacing between tracks
    }
    ImGui::EndChild();
}

void ShowMontageEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    ImGui::SetNextWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    
    ImGuiWindowFlags windowFlags = ImGuiWindowFlags_None;
    if (!state.isCollapsedView)  // Changed condition: show menu bar when NOT collapsed
        windowFlags |= ImGuiWindowFlags_MenuBar;
    
    if (!ImGui::Begin("Montage##MainWindow", p_open, windowFlags))
    {
        ImGui::End();
        return;
    }

    // Menu Bar (only in expanded mode)
    if (!state.isCollapsedView && ImGui::BeginMenuBar())  // This condition is correct
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
            ImGui::EndMenu();
        }
        
        ImGui::EndMenuBar();
    }

    // Main content
    ImGui::PushID("MontageContent");
    {
        ShowMontageToolbar(windowData);
        
        // Add some example tracks if none exist
        if (state.tracks.empty())
        {
            for (int i = 0; i < 3; i++)
            {
                MontageTrack track;
                track.name = "Track " + std::to_string(i + 1);
                state.tracks.push_back(track);
            }
        }
        
        if (!state.isCollapsedView)
        {
            ImGui::Spacing();
            ShowTrackView(windowData, state.isCollapsedView);
        }
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
