/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include <vector>
#include "InputEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

struct InputAction {
    std::string name;
    std::string category;
    std::string keyboardPrimary;
    std::string keyboardSecondary;
    std::string mouseBinding;
    std::string gamepadBinding;
    bool isEnabled;
    bool isCustomizable;
};

static struct {
    std::vector<InputAction> actions = {
        {"Jump", "Movement", "Space", "", "Mouse4", "A", true, true},
        {"Sprint", "Movement", "Left Shift", "", "", "RT", true, true},
        {"Interact", "Actions", "E", "", "", "X", true, true},
        {"OpenInventory", "UI", "Tab", "I", "", "Y", true, true},
        {"QuickSave", "System", "F5", "", "", "", true, false}
    };
    char searchBuffer[256] = "";
    bool showDisabledActions = true;
    bool showSystemActions = true;
    std::string selectedCategory = "All";
    bool isModified = false;
    ImGuiTableFlags tableFlags = ImGuiTableFlags_Resizable | ImGuiTableFlags_Reorderable | 
                                ImGuiTableFlags_Hideable | ImGuiTableFlags_Sortable | 
                                ImGuiTableFlags_SortMulti | ImGuiTableFlags_RowBg | 
                                ImGuiTableFlags_BordersOuter | ImGuiTableFlags_BordersV | 
                                ImGuiTableFlags_ScrollY;
} state;

static void ShowInputMappingTable()
{
    if (ImGui::BeginTable("InputMappingTable", 7, state.tableFlags))
    {
        // Table headers
        ImGui::TableSetupColumn("Action", ImGuiTableColumnFlags_DefaultSort);
        ImGui::TableSetupColumn("Category");
        ImGui::TableSetupColumn("Primary Key");
        ImGui::TableSetupColumn("Secondary Key");
        ImGui::TableSetupColumn("Mouse");
        ImGui::TableSetupColumn("Gamepad");
        ImGui::TableSetupColumn("##Options", ImGuiTableColumnFlags_NoSort | ImGuiTableColumnFlags_WidthFixed);
        ImGui::TableHeadersRow();

        // Table content
        for (auto& action : state.actions)
        {
            // Filter by search
            if (state.searchBuffer[0] != '\0' && 
                action.name.find(state.searchBuffer) == std::string::npos)
                continue;

            // Filter by category
            if (state.selectedCategory != "All" && action.category != state.selectedCategory)
                continue;

            ImGui::TableNextRow();
            
            // Action Name
            ImGui::TableNextColumn();
            ImGui::PushID(&action);
            if (!action.isEnabled)
                ImGui::PushStyleColor(ImGuiCol_Text, ImGui::GetStyle().Colors[ImGuiCol_TextDisabled]);
            ImGui::Text("%s", action.name.c_str());
            if (!action.isEnabled)
                ImGui::PopStyleColor();

            // Category
            ImGui::TableNextColumn();
            ImGui::Text("%s", action.category.c_str());

            // Primary Key
            ImGui::TableNextColumn();
            if (ImGui::Button(action.keyboardPrimary.empty() ? "None" : action.keyboardPrimary.c_str()))
            {
                // TODO: Open key binding popup
            }

            // Secondary Key
            ImGui::TableNextColumn();
            if (ImGui::Button(action.keyboardSecondary.empty() ? "None" : action.keyboardSecondary.c_str()))
            {
                // TODO: Open key binding popup
            }

            // Mouse
            ImGui::TableNextColumn();
            if (ImGui::Button(action.mouseBinding.empty() ? "None" : action.mouseBinding.c_str()))
            {
                // TODO: Open mouse binding popup
            }

            // Gamepad
            ImGui::TableNextColumn();
            if (ImGui::Button(action.gamepadBinding.empty() ? "None" : action.gamepadBinding.c_str()))
            {
                // TODO: Open gamepad binding popup
            }

            // Options
            ImGui::TableNextColumn();
            if (ImGui::Button(ICON_MS_MORE_VERT "##options"))
                ImGui::OpenPopup(("ActionOptions_" + action.name).c_str());

            if (ImGui::BeginPopup(("ActionOptions_" + action.name).c_str()))
            {
                if (ImGui::MenuItem("Reset to Default"))
                {
                    // TODO: Reset bindings
                }
                if (ImGui::MenuItem(action.isEnabled ? "Disable" : "Enable"))
                {
                    action.isEnabled = !action.isEnabled;
                }
                if (action.isCustomizable)
                {
                    if (ImGui::MenuItem("Delete"))
                    {
                        // TODO: Delete action
                    }
                }
                ImGui::EndPopup();
            }
            ImGui::PopID();
        }
        ImGui::EndTable();
    }
}

void ShowInputEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (!ImGui::Begin("Input", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem("New Input Config", "Ctrl+N")) {}
            if (ImGui::MenuItem("Open...", "Ctrl+O")) {}
            if (ImGui::MenuItem("Save", "Ctrl+S", nullptr, state.isModified)) {}
            if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
            ImGui::Separator();
            if (ImGui::BeginMenu("Import"))
            {
                if (ImGui::MenuItem("From JSON...")) {}
                if (ImGui::MenuItem("From XML...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Export"))
            {
                if (ImGui::MenuItem("To JSON...")) {}
                if (ImGui::MenuItem("To XML...")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Add New Action", "Ctrl+N")) {}
            if (ImGui::MenuItem("Add New Category")) {}
            ImGui::Separator();
            if (ImGui::MenuItem("Reset All to Defaults")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem("Show Disabled Actions", nullptr, &state.showDisabledActions);
            ImGui::MenuItem("Show System Actions", nullptr, &state.showSystemActions);
            ImGui::Separator();
            if (ImGui::MenuItem("Collapse All Categories")) {}
            if (ImGui::MenuItem("Expand All Categories")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Tools"))
        {
            if (ImGui::MenuItem("Input Debugger")) {}
            if (ImGui::MenuItem("Conflict Checker")) {}
            if (ImGui::MenuItem("Generate Documentation")) {}
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // Toolbar with proper spacing and organization
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 4));  // Increased spacing between items
        
        // Left-aligned buttons group
        ImGui::BeginGroup();
        {
            ImVec2 buttonSize(32, 32);
            
            if (ImGui::Button(ICON_MS_ADD_BOX "##NewAction", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Action");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_FOLDER_OPEN "##Load", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Load Configuration");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_SAVE "##Save", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save Configuration");
            
            ImGui::SameLine(0, 20); // Extra spacing before search
            
            // Search bar
            ImGui::SetNextItemWidth(200);
            ImGui::InputTextWithHint("##search", ICON_MS_SEARCH " Search actions...", 
                state.searchBuffer, IM_ARRAYSIZE(state.searchBuffer));
        }
        ImGui::EndGroup();
        
        // Right-aligned buttons group
        float windowWidth = ImGui::GetContentRegionAvail().x;
        float rightAlignPosition = windowWidth - 120; // Reserve space for right-aligned buttons
        ImGui::SameLine(rightAlignPosition);
        
        ImGui::BeginGroup();
        {
            ImVec2 buttonSize(32, 32);
            
            if (ImGui::Button(ICON_MS_SETTINGS "##Settings", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Input Settings");
            
            ImGui::SameLine();
            if (ImGui::Button(ICON_MS_HELP "##Help", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Help");
        }
        ImGui::EndGroup();
        
        ImGui::PopStyleVar(2);
        ImGui::Separator();
    }

    // Main content area
    ImGui::BeginChild("InputContent", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
    {
        // Category filter combo
        ImGui::SetNextItemWidth(200);
        if (ImGui::BeginCombo("Category", state.selectedCategory.c_str()))
        {
            if (ImGui::Selectable("All", state.selectedCategory == "All"))
                state.selectedCategory = "All";
            // TODO: Add actual categories
            ImGui::EndCombo();
        }

        ShowInputMappingTable();
    }
    ImGui::EndChild();

    ImGui::End();
}

} // namespace hdImgui
