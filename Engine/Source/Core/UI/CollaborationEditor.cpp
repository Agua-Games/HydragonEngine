/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

#include <string>
#include <vector>
#include <algorithm>

#include <imgui.h>
#include "IconsMaterialSymbols.h"

#include "CollaborationEditor.h"
#include "hdImgui.h"
#include "misc/cpp/imgui_stdlib.h"  // Fixed: Updated include path relative to imgui directory

namespace hdImgui {

struct ConnectedUser {
    std::string name;
    std::string currentTask;
    bool isActive = true;
};

struct CollaborationState {
    bool isConnected = false;
    std::string serverAddress;
    int serverPort = 8080;
    std::string userName;
    std::string sessionName;
    bool autoSync = false;
    float syncInterval = 5.0f;
    std::vector<ConnectedUser> connectedUsers;  // Changed from vector<string> to vector<ConnectedUser>
    
    enum class Mode {
        Realtime,
        AsyncCheckout,
        ReadOnly
    } currentMode = Mode::Realtime;
};

void ShowCollaborationEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    static CollaborationState state{};
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Collaboration", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("Session"))
        {
            if (ImGui::MenuItem("New Session")) {}
            if (ImGui::MenuItem("Join Session")) {}
            if (ImGui::MenuItem("Leave Session")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Save Session State")) {}
            if (ImGui::MenuItem("Export Session Log")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Reset Layout")) {}
            if (ImGui::MenuItem("Show Timeline")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Main Layout - Left Panel and Right Panel
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 200.0f;
    const float maxLeftPanelWidth = 400.0f;

    // Left Panel - Connection & Users
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    
    // Quick Actions Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(state.isConnected ? ICON_MS_LINK_OFF "##Disconnect" : ICON_MS_LINK "##Connect", 
                      ImVec2(24, 24))) 
        state.isConnected = !state.isConnected;
    if (ImGui::IsItemHovered()) 
        ImGui::SetTooltip(state.isConnected ? "Disconnect" : "Connect");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_SETTINGS "##Settings", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Connection Settings");
    ImGui::PopStyleVar();
    
    ImGui::Separator();

    // Connection Settings
    if (ImGui::CollapsingHeader("Connection", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::InputText("Server", &state.serverAddress);
        ImGui::InputInt("Port", &state.serverPort);
        ImGui::InputText("Username", &state.userName);
        
        ImGui::Separator();
        
        const char* modes[] = { "Realtime", "Async Checkout", "Read Only" };
        int currentMode = static_cast<int>(state.currentMode);
        if (ImGui::Combo("Mode", &currentMode, modes, IM_ARRAYSIZE(modes))) {
            state.currentMode = static_cast<CollaborationState::Mode>(currentMode);
        }
    }

    // Connected Users
    if (ImGui::CollapsingHeader("Users", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("UsersList", ImVec2(0, 200), true);
        if (state.connectedUsers.empty())
        {
            ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), "No users connected");
        }
        else
        {
            for (const auto& user : state.connectedUsers)
            {
                ImGui::TextColored(user.isActive ? 
                    ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f), 
                    "%s", user.name.c_str());
                ImGui::SameLine();
                ImGui::TextWrapped("- %s", user.currentTask.c_str());
            }
        }
        ImGui::EndChild();
    }
    
    ImGui::EndChild(); // End LeftPanel

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

    // Right Panel - Layer Management & Activity
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    
    // Status Bar
    ImGui::BeginChild("StatusBar", ImVec2(0, 30), true);
    ImGui::Text("Status: %s", state.isConnected ? "Connected" : "Disconnected");
    ImGui::SameLine(ImGui::GetWindowWidth() - 150);
    ImGui::Text("Session: %s", state.sessionName.c_str());
    ImGui::EndChild();

    // Layer Management
    if (ImGui::CollapsingHeader("Layer Management", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Auto-Sync", &state.autoSync);
        if (state.autoSync)
        {
            ImGui::SameLine();
            ImGui::SetNextItemWidth(100);
            ImGui::DragFloat("Interval (s)", &state.syncInterval, 0.1f, 1.0f, 300.0f);
        }
        
        ImGui::Separator();
        ImGui::TextWrapped("Active Layers:");
        ImGui::BeginChild("LayersList", ImVec2(0, 150), true);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), 
            "Connect to NodeGraph Editor to manage layers");
        ImGui::EndChild();
    }

    // Activity Log
    if (ImGui::CollapsingHeader("Activity Log", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("ActivityLog", ImVec2(0, 0), true);
        ImGui::TextColored(ImVec4(0.5f, 0.5f, 0.5f, 1.0f), 
            "Recent activity will appear here");
        ImGui::EndChild();
    }

    ImGui::EndChild(); // End RightPanel

    ImGui::End();
}

} // namespace hdImgui
