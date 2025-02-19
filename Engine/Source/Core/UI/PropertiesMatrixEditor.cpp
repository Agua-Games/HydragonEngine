/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include <map>
#include <algorithm> // Add this for std::count_if
#include "IconsMaterialSymbols.h"
#include "PropertiesMatrixEditor.h"
#include "hdImgui.h"
#include "misc/cpp/imgui_stdlib.h"

namespace hdImgui {

struct PropertyColumn {
    std::string name;
    std::string type;
    bool isVisible = true;
    bool isEditable = true;
    bool isSelected = false;
};

struct ObjectRow {
    std::string name;
    std::string type;
    bool isSelected = false;
    std::map<std::string, std::string> properties;
};

struct MatrixState {
    std::vector<PropertyColumn> columns;
    std::vector<ObjectRow> rows;
    
    // Filters
    std::string objectFilter;
    std::string propertyFilter;
    bool showInherited = true;
    bool showReadOnly = false;
    
    // View settings
    bool autoApply = false;
    bool showDiff = true;
    enum class SortMode {
        ByName,
        ByType,
        ByModified
    } sortMode = SortMode::ByName;
};

void ShowPropertiesMatrixEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    static MatrixState state{};
    
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Properties Matrix", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem(ICON_MS_SAVE " Save Selection")) {}
            if (ImGui::MenuItem(ICON_MS_UPLOAD " Export Matrix")) {}
            if (ImGui::MenuItem(ICON_MS_DOWNLOAD " Import Matrix")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_MS_REFRESH " Refresh")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Select All Objects")) {}
            if (ImGui::MenuItem("Select All Properties")) {}
            if (ImGui::MenuItem("Clear Selection")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Copy Values")) {}
            if (ImGui::MenuItem("Paste Values")) {}
            ImGui::EndMenu();
        }
        if (ImGui::BeginMenu("View"))
        {
            if (ImGui::MenuItem("Sort by Name", nullptr, state.sortMode == MatrixState::SortMode::ByName)) 
                state.sortMode = MatrixState::SortMode::ByName;
            if (ImGui::MenuItem("Sort by Type", nullptr, state.sortMode == MatrixState::SortMode::ByType)) 
                state.sortMode = MatrixState::SortMode::ByType;
            if (ImGui::MenuItem("Sort by Modified", nullptr, state.sortMode == MatrixState::SortMode::ByModified)) 
                state.sortMode = MatrixState::SortMode::ByModified;
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    // Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_ADD "##Add", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Add Selected Objects");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_REMOVE "##Remove", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Remove Selected Objects");
    ImGui::SameLine();
    
    if (ImGui::Button(ICON_MS_EDIT "##Edit", ImVec2(24, 24))) {}
    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Edit Selected Properties");
    ImGui::PopStyleVar();
    
    ImGui::SameLine();
    ImGui::Checkbox("Auto Apply", &state.autoApply);
    ImGui::SameLine();
    ImGui::Checkbox("Show Diff", &state.showDiff);
    
    ImGui::Separator();

    // Filters
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
    ImGui::Text("Object Filter:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputText("##ObjectFilter", &state.objectFilter);
    
    ImGui::SameLine();
    ImGui::Text("Property Filter:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(200);
    ImGui::InputText("##PropertyFilter", &state.propertyFilter);
    
    ImGui::SameLine();
    ImGui::Checkbox("Show Inherited", &state.showInherited);
    ImGui::SameLine();
    ImGui::Checkbox("Show Read-Only", &state.showReadOnly);
    ImGui::PopStyleVar();
    
    ImGui::Separator();

    // Main Table
    static ImGuiTableFlags flags = 
        ImGuiTableFlags_Resizable | 
        ImGuiTableFlags_Reorderable | 
        ImGuiTableFlags_Hideable | 
        ImGuiTableFlags_Sortable | 
        ImGuiTableFlags_RowBg | 
        ImGuiTableFlags_BordersOuter | 
        ImGuiTableFlags_BordersV |
        ImGuiTableFlags_ScrollY;

    if (ImGui::BeginTable("PropertiesMatrix", 
        static_cast<int>(state.columns.size() + 2), flags)) // Cast size_t to int
    {
        // Headers
        ImGui::TableSetupScrollFreeze(0, 1); // Make header row always visible
        ImGui::TableSetupColumn("Name", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableSetupColumn("Type", ImGuiTableColumnFlags_WidthFixed);
        
        for (const auto& col : state.columns)
        {
            ImGui::TableSetupColumn(col.name.c_str(), 
                ImGuiTableColumnFlags_None | 
                (col.isEditable ? 0 : ImGuiTableColumnFlags_NoReorder));
        }
        ImGui::TableHeadersRow();

        // Rows
        for (auto& row : state.rows)
        {
            ImGui::TableNextRow();
            
            // Name column
            ImGui::TableSetColumnIndex(0);
            ImGui::Selectable(row.name.c_str(), &row.isSelected);
            
            // Type column
            ImGui::TableSetColumnIndex(1);
            ImGui::TextUnformatted(row.type.c_str());
            
            // Property columns
            int colIdx = 2;
            for (const auto& col : state.columns)
            {
                ImGui::TableSetColumnIndex(colIdx++);
                auto it = row.properties.find(col.name);
                if (it != row.properties.end())
                {
                    if (col.isEditable)
                    {
                        ImGui::SetNextItemWidth(-1);
                        ImGui::InputText(("##" + row.name + col.name).c_str(), &it->second);
                    }
                    else
                    {
                        ImGui::TextUnformatted(it->second.c_str());
                    }
                }
            }
        }
        ImGui::EndTable();
    }

    // Status Bar
    ImGui::BeginChild("StatusBar", ImVec2(0, 30), true);
    ImGui::Text("Objects: %zu", state.rows.size());
    ImGui::SameLine();
    ImGui::Text("Selected: %zu", std::count_if(state.rows.begin(), state.rows.end(), 
        [](const ObjectRow& r) { return r.isSelected; }));
    ImGui::SameLine();
    ImGui::Text("Properties: %zu", state.columns.size());
    ImGui::SameLine();
    ImGui::Text("Modified: %zu", 0); // TODO: Track modifications
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui
