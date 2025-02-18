/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>

#include "StreamingEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

// Static state for the editor
struct StreamingControlState {
    // Global Settings
    float memoryBudgetMB = 1024.0f;
    float distanceThreshold = 1000.0f;
    int maxConcurrentOperations = 4;
    
    // Per-scene Settings
    struct SceneStreamingSettings {
        bool enabled = true;
        int priority = 1;  // 0: Low, 1: Medium, 2: High
        float lodTransitionTime = 0.5f;
        float streamingRadius = 500.0f;
    };
    SceneStreamingSettings activeSceneSettings;
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
    }
    ImGui::End();
}

} // namespace hdImgui

