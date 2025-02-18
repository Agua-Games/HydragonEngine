/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include "imgui.h"  // Include ImGui's header
#include <string>

#include "ChimeraPipelineEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowChimeraPipelineEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    // Early out if window is not open
    if (!p_open || !*p_open)
        return;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    // Create window and check if it's visible
    bool window_visible = ImGui::Begin("Chimera Pipeline", p_open, ImGuiWindowFlags_MenuBar);
    if (!window_visible)
    {
        ImGui::End();
        return;
    }

    // Main Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Pipeline", "Ctrl+N")) {}
            if (ImGui::MenuItem("Import Pipeline", "Ctrl+I")) {}
            if (ImGui::MenuItem("Save Pipeline", "Ctrl+S")) {}
            if (ImGui::MenuItem("Save Pipeline As..", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Recent Pipelines"))
            {
                ImGui::MenuItem("character_rig.chpipe");
                ImGui::MenuItem("fx_simulation.chpipe");
                ImGui::MenuItem("terrain_gen.chpipe");
                ImGui::EndMenu();
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Exit", "Alt+F4")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Server"))
        {
            if (ImGui::MenuItem("Start Server", "F5")) {}
            if (ImGui::MenuItem("Stop Server", "Shift+F5")) {}
            if (ImGui::MenuItem("Refresh Server", "F5")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Server Settings...")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("LiveLink"))
        {
            if (ImGui::MenuItem("Load LiveLink")) {}
            if (ImGui::MenuItem("Watch LiveLink")) {}
            if (ImGui::MenuItem("Synchronize")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("LiveLink Settings...")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Status Bar
    ImGui::BeginGroup();
    {
        ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "●"); 
        ImGui::SameLine();
        ImGui::Text("Server Status: Running"); 
        ImGui::SameLine(200.0f);
        ImGui::Text("LiveLink: Active"); 
        ImGui::SameLine(400.0f);
        ImGui::Text("Pipeline: character_rig.chpipe");
    }
    ImGui::EndGroup();
    ImGui::Separator();

    // Main Content Area with Tabs
    if (ImGui::BeginTabBar("ChimeraTabs", ImGuiTabBarFlags_None))
    {
        // Pipeline Graph Tab
        if (ImGui::BeginTabItem("Pipeline Graph"))
        {
            // Left Panel - Node Library
            ImGui::BeginChild("NodeLibrary", ImVec2(200, 0), true);
            {
                ImGui::Text("Node Library");
                ImGui::Separator();
                
                if (ImGui::CollapsingHeader("Generators"))
                {
                    ImGui::Selectable("Mesh Generator");
                    ImGui::Selectable("Terrain Generator");
                    ImGui::Selectable("Particle System");
                }
                
                if (ImGui::CollapsingHeader("Processors"))
                {
                    ImGui::Selectable("Mesh Processor");
                    ImGui::Selectable("UV Unwrapper");
                    ImGui::Selectable("Normal Calculator");
                }
                
                if (ImGui::CollapsingHeader("Outputs"))
                {
                    ImGui::Selectable("FBX Exporter");
                    ImGui::Selectable("USD Exporter");
                    ImGui::Selectable("Alembic Cache");
                }
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Main Graph Area
            ImGui::BeginChild("GraphArea", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            {
                ImGui::Text("Graph View");
                // Placeholder for node graph
            }
            ImGui::EndChild();

            // Bottom Toolbar
            ImGui::BeginChild("BottomBar", ImVec2(0, ImGui::GetFrameHeightWithSpacing()), true);
            {
                if (ImGui::Button("Zoom In")) {}
                ImGui::SameLine();
                if (ImGui::Button("Zoom Out")) {}
                ImGui::SameLine();
                if (ImGui::Button("Fit")) {}
                ImGui::SameLine();
                ImGui::Text("Zoom: 100%%");
                ImGui::SameLine(ImGui::GetWindowWidth() - 100);
                if (ImGui::Button("Auto Layout")) {}
            }
            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        // Settings Tab
        if (ImGui::BeginTabItem("Settings"))
        {
            static int thread_count = 4;
            static bool auto_execute = true;
            static int cache_size_gb = 2;
            static char host[128] = "localhost";
            static int port = 9000;
            static bool auto_sync = true;
            static float sync_interval = 1.0f;
            
            ImGui::BeginChild("SettingsScroll");
            {
                if (ImGui::CollapsingHeader("Execution Settings", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    ImGui::InputInt("Thread Count", &thread_count);
                    ImGui::Checkbox("Auto-Execute on Change", &auto_execute);
                    ImGui::SliderInt("Cache Size (GB)", &cache_size_gb, 1, 32);
                }

                if (ImGui::CollapsingHeader("LiveLink Settings"))
                {
                    ImGui::InputText("Host", host, IM_ARRAYSIZE(host));
                    ImGui::InputInt("Port", &port);
                    ImGui::Checkbox("Auto Synchronize", &auto_sync);
                    ImGui::SliderFloat("Sync Interval (s)", &sync_interval, 0.1f, 10.0f);
                }
            }
            ImGui::EndChild();
            
            ImGui::EndTabItem();
        }

        // Monitor Tab
        if (ImGui::BeginTabItem("Monitor"))
        {
            ImGui::BeginChild("Metrics", ImVec2(0, 200), true);
            {
                ImGui::Text("Performance Metrics");
                ImGui::Separator();
                ImGui::Text("Processing Time: 1.23s");
                ImGui::Text("Memory Usage: 1.45 GB");
                ImGui::Text("Active Nodes: 12");
                ImGui::Text("Cache Size: 234 MB");
            }
            ImGui::EndChild();

            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    ImGui::End();
}
} // namespace hdImgui

