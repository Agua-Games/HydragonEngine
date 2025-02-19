/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include <algorithm>
#include "IconsMaterialSymbols.h"

#include "Profiler.h"
#include "hdImgui.h"

namespace hdImgui {

// Data structures
struct ProfilerSettings {
    bool autoScroll = true;
    bool showGPUOverlay = false;
    bool showMemoryOverlay = false;
    float timelineHeight = 100.0f;

    // Graphics Pipeline settings
    bool drawCalls = false;
    bool shaderCompilation = false;
    bool textureStreaming = false;
    bool gpuMemory = false;

    // CPU Performance settings
    bool mainThread = false;
    bool renderThread = false;
    bool physics = false;
    bool animation = false;

    // Memory settings
    bool heapAllocations = false;
    bool assetLoading = false;
    bool resourceManagement = false;

    // Asset Pipeline settings
    bool contentStreaming = false;
    bool assetProcessing = false;
    bool cacheManagement = false;
};

struct ProfilerTimelineData {
    // Placeholder members
    std::vector<float> timestamps;
    std::vector<std::string> events;
};

// Forward declarations of helper functions
static void ShowProfilerToolbar(ProfilerSettings& settings);
static void ShowProfilerTimeline(const ProfilerTimelineData& data, const ProfilerSettings& settings);
static void ShowProfilerDetails(const ProfilerSettings& settings);
static void ShowProfilerOverlay(const ProfilerSettings& settings);

// Implementation of helper functions
static void ShowProfilerToolbar(ProfilerSettings& settings)
{
    // Match TopToolbar's style settings
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    // Style for buttons
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
    ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));

    // Control buttons section
    if (ImGui::Button(ICON_MS_PLAY_CIRCLE "##Start", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Start Profiling");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_PAUSE "##Pause", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Pause Profiling");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_STOP "##Stop", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Stop Profiling");
    ImGui::SameLine();

    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();  // Add spacing between sections

    // Recording options section
    if (ImGui::Button(ICON_MS_TIMER "##Capture", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Capture Frame");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_SAVE "##Save", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save Profile");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_FOLDER_OPEN "##Load", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load Profile");
    ImGui::SameLine();

    ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();  // Add spacing between sections

    // View options section
    if (ImGui::Button(ICON_MS_TIMELINE "##Timeline", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Timeline View");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_STACKED_LINE_CHART "##Stats", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Statistics View");
    ImGui::SameLine();

    if (ImGui::Button(ICON_MS_MEMORY "##Memory", ImVec2(30, 30))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Memory View");

    ImGui::PopStyleColor();
    ImGui::PopStyleVar(4);
}

static void ShowProfilerTimeline(const ProfilerTimelineData& data, const ProfilerSettings& settings)
{
    ImGui::Text("Timeline visualization coming soon...");
}

static void ShowProfilerDetails(const ProfilerSettings& settings)
{
    ImGui::Text("Details view coming soon...");
}

static void ShowProfilerOverlay(const ProfilerSettings& settings)
{
    ImGui::Text("Overlay coming soon...");
}

// Main Profiler implementation
void ShowProfiler(bool* p_open, HdEditorWindowData* windowData)
{
    static ProfilerSettings settings;
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 150.0f;
    const float maxLeftPanelWidth = 400.0f;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Profiler", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Session", "Ctrl+N")) {}
            if (ImGui::MenuItem("Load Session", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save Session", "Ctrl+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Export Data...", "Ctrl+E")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Auto-scroll", nullptr, &settings.autoScroll);
            ImGui::MenuItem("Show GPU Overlay", nullptr, &settings.showGPUOverlay);
            ImGui::MenuItem("Show Memory Overlay", nullptr, &settings.showMemoryOverlay);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ShowProfilerToolbar(settings);
    ImGui::Separator();

    // Left Panel - Categories and Filters
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    {
        if (ImGui::CollapsingHeader("Graphics Pipeline"))
        {
            ImGui::Checkbox("Draw Calls", &settings.drawCalls);
            ImGui::Checkbox("Shader Compilation", &settings.shaderCompilation);
            ImGui::Checkbox("Texture Streaming", &settings.textureStreaming);
            ImGui::Checkbox("GPU Memory", &settings.gpuMemory);
        }
        
        if (ImGui::CollapsingHeader("CPU Performance"))
        {
            ImGui::Checkbox("Main Thread", &settings.mainThread);
            ImGui::Checkbox("Render Thread", &settings.renderThread);
            ImGui::Checkbox("Physics", &settings.physics);
            ImGui::Checkbox("Animation", &settings.animation);
        }
        
        if (ImGui::CollapsingHeader("Memory"))
        {
            ImGui::Checkbox("Heap Allocations", &settings.heapAllocations);
            ImGui::Checkbox("Asset Loading", &settings.assetLoading);
            ImGui::Checkbox("Resource Management", &settings.resourceManagement);
        }
        
        if (ImGui::CollapsingHeader("Asset Pipeline"))
        {
            ImGui::Checkbox("Content Streaming", &settings.contentStreaming);
            ImGui::Checkbox("Asset Processing", &settings.assetProcessing);
            ImGui::Checkbox("Cache Management", &settings.cacheManagement);
        }
    }
    ImGui::EndChild();

    ImGui::SameLine();

    // Resizer
    ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
    ImGui::Button("##Resizer", ImVec2(4.0f, -1));
    if (ImGui::IsItemActive())
    {
        leftPanelWidth += ImGui::GetIO().MouseDelta.x;
        leftPanelWidth = std::clamp(leftPanelWidth, minLeftPanelWidth, maxLeftPanelWidth);
    }
    if (ImGui::IsItemHovered())
        ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
    ImGui::PopStyleColor(3);

    ImGui::SameLine();

    // Right Panel - Timeline and Details
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    {
        // Timeline Area
        if (ImGui::BeginChild("Timeline", ImVec2(0, settings.timelineHeight * 4), true))
        {
            ImGui::Text("Timeline View - Will contain time-based plots");
            // Placeholder for timeline visualization
            ImGui::EndChild();
        }

        // Details Area
        if (ImGui::BeginTabBar("ProfilerTabs"))
        {
            if (ImGui::BeginTabItem("Events"))
            {
                ImGui::Text("Hierarchical event list will go here");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Statistics"))
            {
                ImGui::Text("Performance statistics and metrics");
                ImGui::EndTabItem();
            }
            if (ImGui::BeginTabItem("Node Graph"))
            {
                ImGui::Text("Connection to NodeGraphEditor for profiling tasks");
                ImGui::EndTabItem();
            }
            ImGui::EndTabBar();
        }
    }
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui
