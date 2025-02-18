/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "NetworkingEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowNetworkingEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    // Static buffers for network statistics
    static float bandwidthHistory[100] = {};
    static float latencyHistory[100] = {};
    static int values_offset = 0;
    static double refresh_time = 0.0f;

    // Update network statistics every 16ms (roughly 60fps)
    if (ImGui::GetTime() > refresh_time + 0.016f)
    {
        // Simulate bandwidth and latency values (replace with real data)
        float new_bandwidth = 0.5f + (float)(rand() % 100) / 100.0f; // Random value between 0.5 and 1.5 MB/s
        float new_latency = 20.0f + (float)(rand() % 60);           // Random value between 20 and 80ms
        
        bandwidthHistory[values_offset] = new_bandwidth;
        latencyHistory[values_offset] = new_latency;
        values_offset = (values_offset + 1) % IM_ARRAYSIZE(bandwidthHistory);
        refresh_time = ImGui::GetTime();
    }

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Networking", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Load Networking Config")) {}
                if (ImGui::MenuItem("Save Networking Config")) {}
                if (ImGui::MenuItem("Import Config...")) {}
                if (ImGui::MenuItem("Export Config...")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Share Config...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Server"))
            {
                if (ImGui::MenuItem("Start Server")) {}
                if (ImGui::MenuItem("Stop Server")) {}
                if (ImGui::MenuItem("Restart Server")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Server Settings...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Client"))
            {
                if (ImGui::MenuItem("Connect Client")) {}
                if (ImGui::MenuItem("Disconnect Client")) {}
                if (ImGui::MenuItem("Reconnect")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Client Settings...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Network Statistics")) {}
                if (ImGui::MenuItem("Bandwidth Monitor")) {}
                if (ImGui::MenuItem("Connection Inspector")) {}
                if (ImGui::MenuItem("Debug Console")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Main content area split into left and right panes
        ImGui::Columns(2, "NetworkingColumns", true);
        
        // Left column (60% width) - Connection status and controls
        ImGui::SetColumnWidth(0, ImGui::GetWindowWidth() * 0.6f);
        
        // Server Status Panel
        if (ImGui::CollapsingHeader("Server Status", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.1f, 0.1f, 0.1f, windowData->globalWindowBgAlpha));
            
            // Status indicators
            static bool serverRunning = false;
            ImGui::TextColored(serverRunning ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                             serverRunning ? "Server: Running" : "Server: Stopped");
            
            // Server info
            ImGui::Text("Port: 7777");
            ImGui::Text("Uptime: 02:34:15");
            ImGui::Text("Connected Clients: 3");
            
            // Quick controls
            if (ImGui::Button(serverRunning ? "Stop Server" : "Start Server"))
                serverRunning = !serverRunning;
            
            ImGui::PopStyleColor();
        }

        // Connected Clients List
        if (ImGui::CollapsingHeader("Connected Clients", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static ImGuiTableFlags flags = ImGuiTableFlags_Borders | ImGuiTableFlags_RowBg;
            if (ImGui::BeginTable("ClientsTable", 4, flags))
            {
                ImGui::TableSetupColumn("ID");
                ImGui::TableSetupColumn("Name");
                ImGui::TableSetupColumn("Ping");
                ImGui::TableSetupColumn("Status");
                ImGui::TableHeadersRow();

                // Example rows
                ImGui::TableNextRow();
                ImGui::TableNextColumn(); ImGui::Text("1");
                ImGui::TableNextColumn(); ImGui::Text("Client_01");
                ImGui::TableNextColumn(); ImGui::Text("45ms");
                ImGui::TableNextColumn(); ImGui::TextColored(ImVec4(0,1,0,1), "Connected");

                ImGui::EndTable();
            }
        }

        // LiveLink Status
        if (ImGui::CollapsingHeader("LiveLink Status", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static bool liveLinkActive = true;
            ImGui::TextColored(liveLinkActive ? ImVec4(0.0f, 1.0f, 0.0f, 1.0f) : ImVec4(1.0f, 0.0f, 0.0f, 1.0f),
                             liveLinkActive ? "LiveLink: Active" : "LiveLink: Inactive");
            
            // LiveLink statistics
            ImGui::Text("Updates/sec: 60");
            ImGui::Text("Bandwidth: 1.2 MB/s");
            ImGui::Text("Latency: 16ms");
        }

        ImGui::NextColumn();

        // Right column (40% width) - Monitoring and debug info
        // Network Statistics Graph
        if (ImGui::CollapsingHeader("Network Statistics", ImGuiTreeNodeFlags_DefaultOpen))
        {
            // Plot bandwidth usage
            ImGui::PlotLines("Bandwidth Usage", 
                           bandwidthHistory, 
                           IM_ARRAYSIZE(bandwidthHistory), 
                           values_offset, 
                           "MB/s", 
                           0.0f,    // scale min 
                           2.0f,    // scale max
                           ImVec2(0, 80));

            // Plot latency
            ImGui::PlotLines("Latency", 
                           latencyHistory, 
                           IM_ARRAYSIZE(latencyHistory), 
                           values_offset, 
                           "ms", 
                           0.0f,    // scale min
                           100.0f,  // scale max
                           ImVec2(0, 80));
        }

        // Debug Console
        if (ImGui::CollapsingHeader("Debug Console", ImGuiTreeNodeFlags_DefaultOpen))
        {
            static char consoleInput[256] = "";
            
            // Console output
            ImGui::BeginChild("ConsoleOutput", ImVec2(0, 200), true);
            ImGui::TextColored(ImVec4(0.4f,0.8f,0.4f,1.0f), "[INFO] Client connected from 192.168.1.105");
            ImGui::TextColored(ImVec4(0.8f,0.8f,0.4f,1.0f), "[WARN] High latency detected");
            ImGui::EndChild();

            // Console input
            ImGui::PushItemWidth(-1);
            if (ImGui::InputText("##ConsoleInput", consoleInput, IM_ARRAYSIZE(consoleInput), 
                               ImGuiInputTextFlags_EnterReturnsTrue))
            {
                // Handle console input
                consoleInput[0] = '\0';
            }
            ImGui::PopItemWidth();
        }

        ImGui::Columns(1);
    }
    ImGui::End();
}
} // namespace hdImgui
