/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <string>
#include <vector>
#include <algorithm>

#include <imgui.h>
#include "misc/cpp/imgui_stdlib.h"
#include "IconsMaterialSymbols.h"

#include "CommunityEditor.h"
#include "hdImgui.h"

namespace hdImgui {

struct CommunityTask {
    std::string name;
    std::string description;
    bool isAutomated = false;
    std::string status = "Pending";
    float progress = 0.0f;
};

struct CommunityMetrics {
    int activeUsers = 0;
    int totalPosts = 0;
    int pendingResponses = 0;
    float engagementRate = 0.0f;
    std::vector<std::string> recentTrends;
};

struct CommunityState {
    bool isConnected = false;
    std::string platformName;
    std::vector<CommunityTask> tasks;
    CommunityMetrics metrics;
    bool autoModeration = false;
    bool analyticsEnabled = true;
    
    enum class ViewMode {
        Overview,
        Analytics,
        Automation,
        Moderation
    } currentView = ViewMode::Overview;
};

void ShowCommunityEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    static CommunityState state{};
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Community Manager", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Import Config")) {}
            if (ImGui::MenuItem("Export Config")) {}
            if (ImGui::MenuItem("Reset Settings")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Connect Platform")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Overview", nullptr, state.currentView == CommunityState::ViewMode::Overview)) 
                state.currentView = CommunityState::ViewMode::Overview;
            if (ImGui::MenuItem("Analytics", nullptr, state.currentView == CommunityState::ViewMode::Analytics)) 
                state.currentView = CommunityState::ViewMode::Analytics;
            if (ImGui::MenuItem("Automation", nullptr, state.currentView == CommunityState::ViewMode::Automation)) 
                state.currentView = CommunityState::ViewMode::Automation;
            if (ImGui::MenuItem("Moderation", nullptr, state.currentView == CommunityState::ViewMode::Moderation)) 
                state.currentView = CommunityState::ViewMode::Moderation;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Quick Actions Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_ANALYTICS "##Analytics", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("View Analytics");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_AUTO_AWESOME "##Automation", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Automation Rules");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_SHIELD "##Moderation", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Moderation Tools");
    ImGui::PopStyleVar();
    
    ImGui::Separator();

    // Main Layout - Left Panel and Right Panel
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 200.0f;
    const float maxLeftPanelWidth = 400.0f;

    // Left Panel - Tasks & Quick Stats
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    
    if (ImGui::CollapsingHeader("Quick Stats", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Text("Active Users: %d", state.metrics.activeUsers);
        ImGui::Text("Pending Responses: %d", state.metrics.pendingResponses);
        ImGui::Text("Engagement Rate: %.1f%%", state.metrics.engagementRate);
    }

    if (ImGui::CollapsingHeader("Automation Tasks", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("TasksList", ImVec2(0, 200), true);
        for (const auto& task : state.tasks)
        {
            ImGui::TextColored(
                task.isAutomated ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                "%s", task.name.c_str()
            );
            ImGui::ProgressBar(task.progress, ImVec2(-1, 2));
        }
        ImGui::EndChild();
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

    // Right Panel - Main Content Area
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    
    // Status Bar
    ImGui::BeginChild("StatusBar", ImVec2(0, 30), true);
    ImGui::Text("Status: %s", state.isConnected ? "Connected" : "Disconnected");
    ImGui::SameLine(ImGui::GetWindowWidth() - 150);
    ImGui::Text("Platform: %s", state.platformName.c_str());
    ImGui::EndChild();

    // Main Content based on current view
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);
    switch (state.currentView)
    {
        case CommunityState::ViewMode::Overview:
            ImGui::Text("Overview Dashboard");
            ImGui::TextWrapped("Connect to NodeGraph Editor for visualization and workflow automation");
            break;
            
        case CommunityState::ViewMode::Analytics:
            if (ImGui::CollapsingHeader("Engagement Metrics", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Text("Recent Trends:");
                for (const auto& trend : state.metrics.recentTrends)
                {
                    ImGui::BulletText("%s", trend.c_str());
                }
            }
            break;
            
        case CommunityState::ViewMode::Automation:
            ImGui::Text("Automation Rules");
            ImGui::TextWrapped("Use NodeGraph Editor to create and manage automation workflows");
            break;
            
        case CommunityState::ViewMode::Moderation:
            ImGui::Checkbox("Auto-Moderation", &state.autoModeration);
            ImGui::SameLine();
            ImGui::Checkbox("Analytics", &state.analyticsEnabled);
            break;
    }
    ImGui::EndChild();

    ImGui::EndChild(); // End RightPanel
    ImGui::End();
}

} // namespace hdImgui
