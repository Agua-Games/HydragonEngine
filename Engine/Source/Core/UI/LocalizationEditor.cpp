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

#include "LocalizationEditor.h"
#include "hdImgui.h"

namespace hdImgui {

struct LocalizationEntry {
    std::string key;
    std::string defaultText;
    std::map<std::string, std::string> translations;  // language code -> translated text
    bool isModified = false;
    bool needsReview = false;
};

struct LocalizationFilter {
    bool showMissing = true;
    bool showModified = true;
    bool showNeedsReview = true;
    std::string searchQuery;
    std::string selectedLanguage;
};

struct LocalizationState {
    std::vector<LocalizationEntry> entries;
    std::vector<std::string> availableLanguages;
    LocalizationFilter filter;
    bool autoSave = false;
    bool showSourceContext = true;
    
    enum class ViewMode {
        Translations,
        Languages,
        Packages,
        Settings
    } currentView = ViewMode::Translations;
};

void ShowLocalizationEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    static LocalizationState state{};
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Localization Manager", p_open, ImGuiWindowFlags_MenuBar))
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
            if (ImGui::MenuItem(ICON_MS_UPLOAD " Export")) {}
            if (ImGui::MenuItem(ICON_MS_DOWNLOAD " Import")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_MS_REFRESH " Refresh")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Translations", nullptr, state.currentView == LocalizationState::ViewMode::Translations)) 
                state.currentView = LocalizationState::ViewMode::Translations;
            if (ImGui::MenuItem("Languages", nullptr, state.currentView == LocalizationState::ViewMode::Languages)) 
                state.currentView = LocalizationState::ViewMode::Languages;
            if (ImGui::MenuItem("Packages", nullptr, state.currentView == LocalizationState::ViewMode::Packages)) 
                state.currentView = LocalizationState::ViewMode::Packages;
            if (ImGui::MenuItem("Settings", nullptr, state.currentView == LocalizationState::ViewMode::Settings)) 
                state.currentView = LocalizationState::ViewMode::Settings;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Quick Actions Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_TRANSLATE "##Translate", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Quick Translate");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_LANGUAGE "##Languages", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Manage Languages");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_CLOUD_DOWNLOAD "##Packages", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Language Packages");
    ImGui::PopStyleVar();
    
    ImGui::Separator();

    // Main Layout - Left Panel and Right Panel
    static float leftPanelWidth = 250.0f;
    const float minLeftPanelWidth = 200.0f;
    const float maxLeftPanelWidth = 400.0f;

    // Left Panel - Filters & Language Selection
    ImGui::BeginChild("LeftPanel", ImVec2(leftPanelWidth, 0), true);
    
    // Search & Filters
    ImGui::Text("Search:");
    ImGui::InputText("##Search", &state.filter.searchQuery);
    
    if (ImGui::CollapsingHeader("Filters", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Show Missing", &state.filter.showMissing);
        ImGui::SameLine();
        ImGui::Checkbox("Modified", &state.filter.showModified);
        ImGui::Checkbox("Needs Review", &state.filter.showNeedsReview);
        
        ImGui::Text("Language:");
        if (ImGui::BeginCombo("##Language", state.filter.selectedLanguage.c_str()))
        {
            for (const auto& lang : state.availableLanguages)
            {
                if (ImGui::Selectable(lang.c_str(), lang == state.filter.selectedLanguage))
                    state.filter.selectedLanguage = lang;
            }
            ImGui::EndCombo();
        }
    }

    if (ImGui::CollapsingHeader("Entries", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::BeginChild("EntriesList", ImVec2(0, 0), true);
        for (const auto& entry : state.entries)
        {
            ImGui::TextColored(
                entry.isModified ? ImVec4(0.2f, 0.7f, 0.2f, 1.0f) : 
                entry.needsReview ? ImVec4(0.7f, 0.7f, 0.2f, 1.0f) : 
                ImVec4(0.7f, 0.7f, 0.7f, 1.0f),
                "%s %s", ICON_MS_TEXT_FIELDS, entry.key.c_str()
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
    ImGui::Text("Entries: %zu", state.entries.size());
    ImGui::SameLine();
    ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), 
        "(%zu modified)", std::count_if(state.entries.begin(), state.entries.end(), 
        [](const LocalizationEntry& e) { return e.isModified; }));
    ImGui::SameLine(ImGui::GetWindowWidth() - 150);
    ImGui::Checkbox("Auto-Save", &state.autoSave);
    ImGui::EndChild();

    // Main Content based on current view
    ImGui::BeginChild("MainContent", ImVec2(0, 0), true);
    switch (state.currentView)
    {
        case LocalizationState::ViewMode::Translations:
            ImGui::Text("Translation Management");
            ImGui::TextWrapped("Use NodeGraph Editor to create translation workflows");
            ImGui::Checkbox("Show Source Context", &state.showSourceContext);
            break;
            
        case LocalizationState::ViewMode::Languages:
            ImGui::Text("Language Management");
            ImGui::TextWrapped("Configure available languages and regional settings");
            break;
            
        case LocalizationState::ViewMode::Packages:
            ImGui::Text("Language Packages");
            ImGui::TextWrapped("Install and manage language packs using NodeGraph Editor");
            break;
            
        case LocalizationState::ViewMode::Settings:
            ImGui::Text("Localization Settings");
            ImGui::TextWrapped("Configure global localization settings");
            break;
    }
    ImGui::EndChild();

    ImGui::EndChild(); // End RightPanel
    ImGui::End();
}

} // namespace hdImgui
