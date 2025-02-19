/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <string>
#include <algorithm>
#include <vector>
#include <map>

#include <imgui.h>
#include "misc/cpp/imgui_stdlib.h"
#include "IconsMaterialSymbols.h"

#include "PresetEditor.h"
#include "hdImgui.h"

namespace hdImgui {

struct PresetEntry {
    std::string name;
    std::string category;
    std::string description;
    bool isModified = false;
    bool isDefault = false;
};

struct PresetFilter {
    bool showModified = true;
    bool showDefault = true;
    std::string searchQuery;
    std::string selectedCategory;
};

struct PresetState {
    std::vector<PresetEntry> entries;
    std::vector<std::string> categories;
    PresetFilter filter;
    bool autoSave = false;
    
    enum class ViewMode {
        Library,
        Categories,
        Import,
        Settings
    } currentView = ViewMode::Library;
};

void ShowPresetEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    static PresetState state{};
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Presets", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem(ICON_MS_SAVE " Save Changes")) {}
            if (ImGui::MenuItem(ICON_MS_UPLOAD " Export Preset")) {}
            if (ImGui::MenuItem(ICON_MS_DOWNLOAD " Import Preset")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_MS_REFRESH " Refresh")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Library", nullptr, state.currentView == PresetState::ViewMode::Library)) 
                state.currentView = PresetState::ViewMode::Library;
            if (ImGui::MenuItem("Categories", nullptr, state.currentView == PresetState::ViewMode::Categories)) 
                state.currentView = PresetState::ViewMode::Categories;
            if (ImGui::MenuItem("Import", nullptr, state.currentView == PresetState::ViewMode::Import)) 
                state.currentView = PresetState::ViewMode::Import;
            if (ImGui::MenuItem("Settings", nullptr, state.currentView == PresetState::ViewMode::Settings)) 
                state.currentView = PresetState::ViewMode::Settings;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Quick Actions Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_ADD "##New", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Preset");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_FOLDER "##Browse", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Browse Presets");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_SETTINGS "##Configure", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Configure Presets");
    ImGui::PopStyleVar();
    
    ImGui::Separator();

    // Main Layout - Left Panel and Right Panel
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 200.0f;
    const float maxLeftPanelWidth = 400.0f;

    // Left Panel - Filters & Preset Selection
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    
    // Search & Filters
    ImGui::Text("Search:");
    ImGui::InputText("##Search", &state.filter.searchQuery);
    
    if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Show Modified", &state.filter.showModified);
        ImGui::SameLine();
        ImGui::Checkbox("Show Default", &state.filter.showDefault);
        
        ImGui::Text("Category:");
        if (ImGui::BeginCombo("##Category", state.filter.selectedCategory.c_str()))
        {
            for (const auto& category : state.categories)
            {
                if (ImGui::Selectable(category.c_str(), category == state.filter.selectedCategory))
                    state.filter.selectedCategory = category;
            }
            ImGui::EndCombo();
        }
    }

    if (ImGui::CollapsingHeader("Presets", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("PresetsList", ImVec2(0, 0), true);
        for (const auto& preset : state.entries)
        {
            ImGui::TextColored(
                preset.isModified ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : 
                preset.isDefault ? ImVec4(0.7f, 0.7f, 0.2f, 1.0f) : 
                ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                "%s %s", ICON_MS_SETTINGS_APPLICATIONS, preset.name.c_str()
            );
            
            if (ImGui::IsItemHovered() && !preset.description.empty())
            {
                ImGui::BeginTooltip();
                ImGui::Text("%s", preset.description.c_str());
                ImGui::EndTooltip();
            }
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
    ImGui::Text("Presets: %zu", state.entries.size());
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), 
        "(%zu modified)", std::count_if(state.entries.begin(), state.entries.end(), 
        [](const PresetEntry& e) { return e.isModified; }));
    ImGui::SameLine(ImGui::GetWindowWidth() - 150);
    ImGui::Checkbox("Auto-Save", &state.autoSave);
    ImGui::EndChild();

    // Main Content based on current view
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);
    switch (state.currentView)
    {
        case PresetState::ViewMode::Library:
            ImGui::Text("Preset Library");
            ImGui::TextWrapped("Use NodeGraph Editor to create and manage preset workflows");
            break;
            
        case PresetState::ViewMode::Categories:
            ImGui::Text("Preset Categories");
            ImGui::TextWrapped("Manage and organize preset categories");
            break;
            
        case PresetState::ViewMode::Import:
            ImGui::Text("Import Presets");
            ImGui::TextWrapped("Import presets from files or create from current settings");
            break;
            
        case PresetState::ViewMode::Settings:
            ImGui::Text("Preset Settings");
            ImGui::TextWrapped("Configure global preset settings");
            break;
    }
    ImGui::EndChild();

    ImGui::EndChild(); // End RightPanel
    ImGui::End();
}

} // namespace hdImgui
