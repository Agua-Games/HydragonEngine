/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include <algorithm>

#include "misc/cpp/imgui_stdlib.h"
#include "IconsMaterialSymbols.h"

#include "ReflectionEditor.h"
#include "hdImgui.h"

namespace hdImgui {

struct ReflectionType {
    std::string name;
    std::string category;  // "class", "struct", "enum", "property"
    bool isExposed = false;
    std::string scope;     // "public", "protected", "private"
    std::vector<std::string> attributes;
    bool isEditable = true;
};

struct ReflectionFilter {
    bool showClasses = true;
    bool showStructs = true;
    bool showEnums = true;
    bool showProperties = true;
    bool onlyExposed = false;
    std::string searchQuery;
};

struct ReflectionState {
    std::vector<ReflectionType> types;
    ReflectionFilter filter;
    bool autoRefresh = true;
    bool showInherited = true;
    
    enum class ViewMode {
        Types,
        Hierarchy,
        Rules,
        Settings
    } currentView = ViewMode::Types;
};

void ShowReflectionEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    static ReflectionState state{};
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Reflection Manager", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("Refresh Types")) {}
            if (ImGui::MenuItem("Export Definitions")) {}
            if (ImGui::MenuItem("Import Rules")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Generate Headers")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Types", nullptr, state.currentView == ReflectionState::ViewMode::Types)) 
                state.currentView = ReflectionState::ViewMode::Types;
            if (ImGui::MenuItem("Hierarchy", nullptr, state.currentView == ReflectionState::ViewMode::Hierarchy)) 
                state.currentView = ReflectionState::ViewMode::Hierarchy;
            if (ImGui::MenuItem("Rules", nullptr, state.currentView == ReflectionState::ViewMode::Rules)) 
                state.currentView = ReflectionState::ViewMode::Rules;
            if (ImGui::MenuItem("Settings", nullptr, state.currentView == ReflectionState::ViewMode::Settings)) 
                state.currentView = ReflectionState::ViewMode::Settings;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Quick Actions Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_CLASS "##Types", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Manage Types");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_ACCOUNT_TREE "##Hierarchy", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("View Hierarchy");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_RULE "##Rules", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Reflection Rules");
    ImGui::PopStyleVar();
    
    ImGui::Separator();

    // Main Layout - Left Panel and Right Panel
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 200.0f;
    const float maxLeftPanelWidth = 400.0f;

    // Left Panel - Filters & Type List
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    
    // Search & Filters
    ImGui::Text("Search:");
    ImGui::InputText("##Search", &state.filter.searchQuery);
    
    if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Classes", &state.filter.showClasses);
        ImGui::SameLine();
        ImGui::Checkbox("Structs", &state.filter.showStructs);
        ImGui::Checkbox("Enums", &state.filter.showEnums);
        ImGui::SameLine();
        ImGui::Checkbox("Properties", &state.filter.showProperties);
        ImGui::Checkbox("Only Exposed", &state.filter.onlyExposed);
    }

    if (ImGui::CollapsingHeader("Types", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("TypesList", ImVec2(0, 0), true);
        for (const auto& type : state.types)
        {
            if (state.filter.onlyExposed && !type.isExposed) continue;
            
            const char* icon = type.category == "class" ? ICON_MS_CLASS :
                              type.category == "struct" ? ICON_MS_ARCHITECTURE :
                              type.category == "enum" ? ICON_MS_FORMAT_LIST_NUMBERED :
                              ICON_MS_SETTINGS;
            
            ImGui::TextColored(
                type.isExposed ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                "%s %s", icon, type.name.c_str()
            );
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
    ImGui::Text("Types: %zu", state.types.size());
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), 
        "(%zu exposed)", std::count_if(state.types.begin(), state.types.end(), 
        [](const ReflectionType& t) { return t.isExposed; }));
    ImGui::SameLine(ImGui::GetWindowWidth() - 150);
    ImGui::Checkbox("Auto-Refresh", &state.autoRefresh);
    ImGui::EndChild();

    // Main Content based on current view
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);
    switch (state.currentView)
    {
        case ReflectionState::ViewMode::Types:
            ImGui::Text("Type Management");
            ImGui::TextWrapped("Use NodeGraph Editor to create reflection workflows");
            ImGui::Checkbox("Show Inherited", &state.showInherited);
            break;
            
        case ReflectionState::ViewMode::Hierarchy:
            ImGui::Text("Type Hierarchy");
            ImGui::TextWrapped("Connect to NodeGraph Editor for inheritance visualization");
            break;
            
        case ReflectionState::ViewMode::Rules:
            ImGui::Text("Reflection Rules");
            ImGui::TextWrapped("Define reflection rules using NodeGraph Editor");
            break;
            
        case ReflectionState::ViewMode::Settings:
            ImGui::Text("Reflection Settings");
            ImGui::TextWrapped("Configure global reflection settings");
            break;
    }
    ImGui::EndChild();

    ImGui::EndChild(); // End RightPanel
    ImGui::End();
}

} // namespace hdImgui
