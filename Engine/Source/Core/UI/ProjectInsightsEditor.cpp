/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <algorithm>

#include <imgui.h>
#include "IconsMaterialSymbols.h"
#include "ProjectInsightsEditor.h"
#include "hdImgui.h"

namespace hdImgui {

void ShowProjectInsightsEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    static ProjectInsightsState state;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    if (!ImGui::Begin("Project Insights", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Analysis Profile")) {}
            if (ImGui::MenuItem("Load Profile", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save Profile", "Ctrl+S")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Export Report", "Ctrl+E")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Analysis"))
        {
            if (ImGui::MenuItem("Run Analysis", "F5")) { state.isAnalyzing = true; }
            if (ImGui::MenuItem("Stop Analysis", "Esc")) { state.isAnalyzing = false; }
            ImGui::Separator();
            if (ImGui::MenuItem("Configure Rules")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Reset Layout")) {}
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Main Layout - Left Panel and Right Panel
    static float leftPanelWidth = 300.0f;
    const float minLeftPanelWidth = 200.0f;
    const float maxLeftPanelWidth = 500.0f;

    // Left Panel
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    
    // Quick Actions Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_PLAY_CIRCLE "##Run", ImVec2(24, 24))) 
        state.isAnalyzing = true;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Run Analysis");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_STOP_CIRCLE "##Stop", ImVec2(24, 24))) 
        state.isAnalyzing = false;
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Stop Analysis");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_SETTINGS "##Settings", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Analysis Settings");
    ImGui::PopStyleVar();
    ImGui::Separator();

    // Analysis Categories
    if (ImGui::BeginTabBar("InsightCategories"))
    {
        if (ImGui::BeginTabItem("Performance"))
        {
            ImGui::BeginChild("PerformanceChecks", ImVec2(0, 0), true);
            ImGui::Checkbox("Memory Usage Analysis", &state.memoryAnalysis);
            ImGui::Checkbox("CPU Performance Profiling", &state.cpuProfiling);
            ImGui::Checkbox("GPU Resource Management", &state.gpuResourceMgmt);
            ImGui::Checkbox("Asset Loading Optimization", &state.assetLoadingOpt);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Best Practices"))
        {
            ImGui::BeginChild("BestPractices", ImVec2(0, 0), true);
            ImGui::Checkbox("Code Style Compliance", &state.codeStyleCheck);
            ImGui::Checkbox("Project Structure", &state.projectStructure);
            ImGui::Checkbox("Asset Naming Conventions", &state.namingConventions);
            ImGui::Checkbox("Documentation Coverage", &state.documentationCoverage);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        if (ImGui::BeginTabItem("Workflows"))
        {
            ImGui::BeginChild("Workflows", ImVec2(0, 0), true);
            ImGui::Checkbox("Build Pipeline Analysis", &state.buildPipelineAnalysis);
            ImGui::Checkbox("Version Control Usage", &state.versionControlUsage);
            ImGui::Checkbox("Team Collaboration Patterns", &state.teamCollabPatterns);
            ImGui::EndChild();
            ImGui::EndTabItem();
        }
        ImGui::EndTabBar();
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

    // Right Panel - Insights and Recommendations
    ImGui::BeginChild("RightPanel", ImVec2(0, 0), true);
    
    // Status Bar at the top
    ImGui::BeginChild("StatusBar", ImVec2(0, 30), true);
    ImGui::Text("Status: %s", state.isAnalyzing ? "Analyzing..." : "Ready");
    ImGui::SameLine(ImGui::GetWindowWidth() - 100);
    ImGui::Text("Progress: %.0f%%", state.analysisProgress);
    ImGui::EndChild();

    ImGui::BeginChild("InsightsContent", ImVec2(0, 0), true);
    if (ImGui::CollapsingHeader("Quick Start Guide"))
    {
        ImGui::TextWrapped("1. Select analysis categories from the left panel");
        ImGui::TextWrapped("2. Click the Run button to start analysis");
        ImGui::TextWrapped("3. Review insights and recommendations");
        ImGui::TextWrapped("4. Export report or connect to Node Graph for detailed visualization");
    }

    if (ImGui::CollapsingHeader("Recent Insights"))
    {
        ImGui::TextColored(ImVec4(0.2f, 0.7f, 0.2f, 1.0f), "No recent analysis results");
        ImGui::TextWrapped("Run an analysis to see insights and recommendations.");
    }

    if (ImGui::CollapsingHeader("AI Assistant Integration"))
    {
        ImGui::TextWrapped("Connect to AI services for advanced insights and recommendations.");
        if (ImGui::Button("Configure AI Services")) {}
        if (ImGui::Button("View AI Recommendations")) {}
    }
    ImGui::EndChild();
    
    ImGui::EndChild(); // End RightPanel

    ImGui::End();
}

} // namespace hdImgui
