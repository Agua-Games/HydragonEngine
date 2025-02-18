/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>

#include "IconsMaterialSymbols.h"

#include "StreamingEditor.h"
#include "hdImgui.h"


namespace hdImgui {

struct StreamingControlState {
    // Existing settings
    float memoryBudgetMB = 1024.0f;
    float distanceThreshold = 1000.0f;
    int maxConcurrentOperations = 4;
    
    struct SceneStreamingSettings {
        bool enabled = true;
        int priority = 1;
        float lodTransitionTime = 0.5f;
        float streamingRadius = 500.0f;
    };
    SceneStreamingSettings activeSceneSettings;

    // New monitoring state
    struct MonitoringData {
        float memoryUsage = 0.0f;
        float peakMemoryUsage = 0.0f;
        int activeOperations = 0;
        float streamingLatency = 0.0f;
        static const int HISTORY_LENGTH = 100;
        float memoryHistory[HISTORY_LENGTH] = {};
        float latencyHistory[HISTORY_LENGTH] = {};
        int historyIndex = 0;
    } monitoring;
};

static StreamingControlState s_streamingState;

void ShowStreamingEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Streaming", p_open, ImGuiWindowFlags_MenuBar))
    {
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

        // Main Controls
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

        // Global Settings
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

        // Per-scene Settings
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
            ImGui::BeginChild("MonitoringSection", ImVec2(0, 280), true);
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
                        StreamingControlState::MonitoringData::HISTORY_LENGTH, 
                        s_streamingState.monitoring.historyIndex,
                        nullptr, 0.0f, s_streamingState.memoryBudgetMB, 
                        ImVec2(-1, 60));

                    ImGui::Text("Latency History");
                    ImGui::PlotLines("##LatencyHistory", 
                        s_streamingState.monitoring.latencyHistory,
                        StreamingControlState::MonitoringData::HISTORY_LENGTH, 
                        s_streamingState.monitoring.historyIndex,
                        nullptr, 0.0f, 100.0f, 
                        ImVec2(-1, 60));
                }
            }
            ImGui::EndChild();
        }
    }
    ImGui::End();
}

} // namespace hdImgui

