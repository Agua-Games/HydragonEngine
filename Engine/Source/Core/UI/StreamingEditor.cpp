/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include <algorithm>  // for std::count_if
#include "StreamingEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

// Independent structure definitions
struct SceneStreamingSettings {
    bool enabled = true;
    int priority = 1;
    float lodTransitionTime = 0.5f;
    float streamingRadius = 500.0f;
};

static const char* priorities[] = { "Low", "Medium", "High" };

struct MonitoringData {
    float memoryUsage = 0.0f;
    float peakMemoryUsage = 0.0f;
    int activeOperations = 0;
    float streamingLatency = 0.0f;
    static const int HISTORY_LENGTH = 100;
    float memoryHistory[HISTORY_LENGTH] = {};
    float latencyHistory[HISTORY_LENGTH] = {};
    int historyIndex = 0;
    
    // Streaming rate monitoring
    static const int RATE_HISTORY_LENGTH = 100;
    float streamingRateHistory[RATE_HISTORY_LENGTH] = {};  // MB/s
    float currentStreamingRate = 0.0f;
    float peakStreamingRate = 0.0f;
};

struct StreamingRequest {
    std::string name;
    int priority;
    float size;
    std::string status;
    bool selected;
};

struct StreamingControlState {
    // Settings
    float memoryBudgetMB = 1024.0f;
    float distanceThreshold = 1000.0f;
    int maxConcurrentOperations = 4;
    
    SceneStreamingSettings activeSceneSettings;
    MonitoringData monitoring;
    std::vector<StreamingRequest> activeRequests = {
        {"Terrain_LOD0", 2, 256.0f, "In Progress", false},
        {"Character_HD", 1, 128.0f, "Queued", false},
        {"Props_Block_A", 0, 64.0f, "Pending", false}
    };
    bool showCompleted = false;
    int selectedPriority = 1;
};

static StreamingControlState s_streamingState;

void ShowStreamingEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    // Store the current padding to restore it later
    const ImVec2 originalPadding = ImGui::GetStyle().WindowPadding;
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    if (!ImGui::Begin("Streaming", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::PopStyleVar();
        ImGui::End();
        return;
    }

    // Menu bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Save Configuration")) {}
            if (ImGui::MenuItem("Load Configuration")) {}
            if (ImGui::MenuItem("Reset to Defaults")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Restore padding for main content
    ImGui::PopStyleVar();
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, originalPadding);
    
    // Streaming Toolbar
    ImGui::BeginChild("StreamingToolbar", ImVec2(0, 40), true);
    {
        if (ImGui::Button(ICON_MS_REFRESH " Reset Streaming")) {}
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_MEMORY " Clear Cache")) {}
        ImGui::SameLine();
        
        ImGui::SameLine();
        ImGui::Separator();
        ImGui::SameLine();
        
        // Quick presets dropdown
        ImGui::SetNextItemWidth(120);
        static int currentPreset = 1;
        const char* presets[] = { "High Performance", "Memory Optimized", "Balanced" };
        if (ImGui::Combo("Preset", &currentPreset, presets, IM_ARRAYSIZE(presets)))
        {
            // TODO: Apply preset settings
        }
    }
    ImGui::EndChild();

    // Main Controls (existing)
    {
        ImGui::BeginChild("MainControls", ImVec2(0, 60), true);
        {
            ImGui::Checkbox("Streaming Enabled", &windowData->streamingEnabled);
            ImGui::SameLine(200);
            if (ImGui::Button(ICON_MS_PLAY_CIRCLE " Start Streaming Engine")){}
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_REFRESH " Refresh")){}
        }
        ImGui::EndChild();
    }

    // Global Settings (existing)
    if (ImGui::CollapsingHeader("Global Streaming Settings", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("GlobalSettings", ImVec2(0, 140), true);
        {
            ImGui::PushItemWidth(200);
            ImGui::SliderFloat("Memory Budget (MB)", &s_streamingState.memoryBudgetMB, 
                128.0f, 8192.0f, "%.0f MB");
            ImGui::SliderFloat("Distance Threshold", &s_streamingState.distanceThreshold, 
                100.0f, 5000.0f, "%.0f units");
            ImGui::SliderInt("Max Concurrent Operations", &s_streamingState.maxConcurrentOperations, 
                1, 16);
            ImGui::PopItemWidth();

            ImGui::Separator();
            if (ImGui::Button("Apply Global Settings")) {}
            ImGui::SameLine();
            if (ImGui::Button("Reset Global Settings")) {}
        }
        ImGui::EndChild();
    }

    // Per-scene Settings (existing)
    if (ImGui::CollapsingHeader("Per-scene Configuration", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("SceneSettings", ImVec2(0, 160), true);
        {
            ImGui::Text("Active Scene: Museum.usd");
            ImGui::Separator();

            ImGui::PushItemWidth(200);
            ImGui::Checkbox("Scene Streaming Enabled", &s_streamingState.activeSceneSettings.enabled);
            const char* priorities[] = { "Low", "Medium", "High" };
            ImGui::Combo("Priority", &s_streamingState.activeSceneSettings.priority, 
                priorities, IM_ARRAYSIZE(priorities));
            ImGui::SliderFloat("LOD Transition Time", &s_streamingState.activeSceneSettings.lodTransitionTime, 
                0.1f, 2.0f, "%.1f sec");
            ImGui::SliderFloat("Streaming Radius", &s_streamingState.activeSceneSettings.streamingRadius, 
                100.0f, 2000.0f, "%.0f units");
            ImGui::PopItemWidth();

            ImGui::Separator();
            if (ImGui::Button("Apply Scene Settings")) {}
            ImGui::SameLine();
            if (ImGui::Button("Reset Scene Settings")) {}
        }
        ImGui::EndChild();
    }

    // New Monitoring Section
    if (ImGui::CollapsingHeader("Streaming Monitoring", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("MonitoringSection", ImVec2(0, 320), true);
        {
            // Memory Usage
            {
                ImGui::BeginGroup();
                float progress = s_streamingState.monitoring.memoryUsage / s_streamingState.memoryBudgetMB;
                ImGui::ProgressBar(progress, ImVec2(-1, 0), 
                    std::to_string((int)s_streamingState.monitoring.memoryUsage).c_str());
                ImGui::Text("Memory Usage: %.1f MB / %.1f MB", 
                    s_streamingState.monitoring.memoryUsage, 
                    s_streamingState.memoryBudgetMB);
                ImGui::Text("Peak Memory: %.1f MB", s_streamingState.monitoring.peakMemoryUsage);
                ImGui::EndGroup();
            }

            ImGui::Separator();

            // Streaming Rate
            {
                ImGui::BeginGroup();
                ImGui::Text("Streaming Rate: %.1f MB/s", s_streamingState.monitoring.currentStreamingRate);
                ImGui::Text("Peak Rate: %.1f MB/s", s_streamingState.monitoring.peakStreamingRate);
                
                ImGui::Text("Streaming Rate History");
                ImGui::PlotLines("##StreamingRateHistory", 
                    s_streamingState.monitoring.streamingRateHistory,
                    MonitoringData::RATE_HISTORY_LENGTH, 
                    s_streamingState.monitoring.historyIndex,
                    nullptr, 0.0f, s_streamingState.monitoring.peakStreamingRate * 1.2f, 
                    ImVec2(-1, 60));
                ImGui::EndGroup();
            }

            ImGui::Separator();

            // Performance Metrics
            {
                ImGui::BeginGroup();
                ImGui::Text("Active Operations: %d / %d", 
                    s_streamingState.monitoring.activeOperations,
                    s_streamingState.maxConcurrentOperations);
                ImGui::Text("Streaming Latency: %.2f ms", s_streamingState.monitoring.streamingLatency);
                ImGui::EndGroup();
            }

            ImGui::Separator();

            // Graphs
            {
                ImGui::Text("Memory Usage History");
                ImGui::PlotLines("##MemoryHistory", 
                    s_streamingState.monitoring.memoryHistory,
                    MonitoringData::HISTORY_LENGTH, 
                    s_streamingState.monitoring.historyIndex,
                    nullptr, 0.0f, s_streamingState.memoryBudgetMB, 
                    ImVec2(-1, 60));

                ImGui::Text("Latency History");
                ImGui::PlotLines("##LatencyHistory", 
                    s_streamingState.monitoring.latencyHistory,
                    MonitoringData::HISTORY_LENGTH, 
                    s_streamingState.monitoring.historyIndex,
                    nullptr, 0.0f, 100.0f, 
                    ImVec2(-1, 60));
            }
        }
        ImGui::EndChild();
    }

    // New Priority Management Section
    if (ImGui::CollapsingHeader("Priority Management", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("PrioritySection", ImVec2(0, 300), true);
        {
            // Control Bar
            {
                ImGui::BeginGroup();
                ImGui::SetNextItemWidth(120);
                ImGui::Combo("Set Priority", &s_streamingState.selectedPriority, priorities, IM_ARRAYSIZE(priorities));
                
                ImGui::SameLine();
                if (ImGui::Button("Apply Priority")) {
                    // Apply selected priority to selected requests
                }
                
                ImGui::SameLine();
                if (ImGui::Button("Cancel Selected")) {
                    // Cancel selected streaming requests
                }
                
                ImGui::SameLine();
                ImGui::Checkbox("Show Completed", &s_streamingState.showCompleted);
                ImGui::EndGroup();
            }

            ImGui::Separator();

            // Requests Table
            {
                if (ImGui::BeginTable("StreamingRequests", 5, 
                    ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg | ImGuiTableFlags_Resizable))
                {
                    // Headers
                    ImGui::TableSetupColumn("", ImGuiTableColumnFlags_WidthFixed, 30.0f);
                    ImGui::TableSetupColumn("Asset", ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupColumn("Priority", ImGuiTableColumnFlags_WidthFixed, 70.0f);
                    ImGui::TableSetupColumn("Size (MB)", ImGuiTableColumnFlags_WidthFixed, 80.0f);
                    ImGui::TableSetupColumn("Status", ImGuiTableColumnFlags_WidthFixed, 100.0f);
                    ImGui::TableHeadersRow();

                    // Rows
                    for (auto& request : s_streamingState.activeRequests)
                    {
                        ImGui::TableNextRow();
                        
                        // Checkbox column
                        ImGui::TableNextColumn();
                        ImGui::PushID(&request);
                        ImGui::Checkbox("##select", &request.selected);
                        ImGui::PopID();

                        // Asset name
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(request.name.c_str());

                        // Priority
                        ImGui::TableNextColumn();
                        ImGui::TextUnformatted(priorities[request.priority]);

                        // Size
                        ImGui::TableNextColumn();
                        ImGui::Text("%.1f", request.size);

                        // Status
                        ImGui::TableNextColumn();
                        if (request.status == "In Progress") {
                            ImGui::TextColored(ImVec4(0.2f, 0.7f, 0.2f, 1.0f), "%s", request.status.c_str());
                        } else if (request.status == "Queued") {
                            ImGui::TextColored(ImVec4(0.7f, 0.7f, 0.1f, 1.0f), "%s", request.status.c_str());
                        } else {
                            ImGui::TextUnformatted(request.status.c_str());
                        }
                    }
                    ImGui::EndTable();
                }
            }

            // Status Bar
            {
                ImGui::Separator();
                ImGui::Text("Active Requests: %zu", s_streamingState.activeRequests.size());
                ImGui::SameLine(200);
                ImGui::Text("Selected: %zu", 
                    std::count_if(s_streamingState.activeRequests.begin(), 
                                s_streamingState.activeRequests.end(),
                                [](const auto& req) { return req.selected; }));
            }
        }
        ImGui::EndChild();
    }
    ImGui::PopStyleVar();
    ImGui::End();
}

} // namespace hdImgui
