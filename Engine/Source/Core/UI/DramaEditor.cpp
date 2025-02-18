/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include "DramaEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

struct DramaEditorState {
    std::string currentDramaName;
    bool isModified = false;
    
    // UI States
    bool showSequenceList = true;
    bool showEventList = true;
    int selectedSequence = -1;
    int selectedEvent = -1;
    char searchBuffer[256] = "";
    float tensionCurve = 0.5f;
    int activeTab = 0;
};

static DramaEditorState* state = nullptr;

void ShowDramaEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!state) {
        state = new DramaEditorState();
    }

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    std::string windowTitle = "Drama Editor";
    if (!state->currentDramaName.empty()) {
        windowTitle += " - " + state->currentDramaName;
        if (state->isModified) windowTitle += "*";
    }

    if (!ImGui::Begin(windowTitle.c_str(), p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
        if (ImGui::BeginMenu("File"))
        {
            if (ImGui::MenuItem(ICON_MS_ADD " New Drama", "Ctrl+N")) {
                state->currentDramaName = "New Drama";
                state->isModified = false;
            }
            if (ImGui::MenuItem(ICON_MS_FOLDER_OPEN " Open...", "Ctrl+O")) {}
            
            bool canSave = !state->currentDramaName.empty();
            if (ImGui::MenuItem(ICON_MS_SAVE " Save", "Ctrl+S", false, canSave)) {
                state->isModified = false;
            }
            if (ImGui::MenuItem(ICON_MS_SAVE_AS " Save As...", "Ctrl+Shift+S", false, canSave)) {}
            
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_MS_IMPORT_EXPORT " Import Drama...")) {}
            if (ImGui::MenuItem(ICON_MS_UPLOAD " Export Drama...")) {}
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem(ICON_MS_UNDO " Undo", "Ctrl+Z")) {}
            if (ImGui::MenuItem(ICON_MS_REDO " Redo", "Ctrl+Y")) {}
            ImGui::Separator();
            if (ImGui::MenuItem(ICON_MS_SETTINGS " Preferences...")) {}
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("View"))
        {
            ImGui::MenuItem(ICON_MS_LIST " Sequence List", nullptr, &state->showSequenceList);
            ImGui::MenuItem(ICON_MS_EVENT " Event List", nullptr, &state->showEventList);
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // Toolbar
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4));
    if (ImGui::Button(ICON_MS_ADD_BOX " New Sequence")) { state->isModified = true; }
    ImGui::SameLine();
    if (ImGui::Button(ICON_MS_ADD_CIRCLE " New Event")) { state->isModified = true; }
    ImGui::SameLine();
    ImGui::Separator();
    ImGui::SameLine();
    if (ImGui::Button(ICON_MS_PREVIEW " Preview")) {}
    ImGui::SameLine();
    if (ImGui::Button(ICON_MS_PLAY_ARROW " Test")) {}
    ImGui::PopStyleVar();

    // Search bar
    ImGui::SetNextItemWidth(-1);
    if (ImGui::InputTextWithHint("##Search", ICON_MS_SEARCH " Search sequences and events...", 
        state->searchBuffer, IM_ARRAYSIZE(state->searchBuffer))) {}

    // Main content area with tabs
    if (ImGui::BeginTabBar("DramaEditorTabs"))
    {
        if (ImGui::BeginTabItem(ICON_MS_MOVIE " Sequences"))
        {
            state->activeTab = 0;
            
            // Split view
            ImGui::Columns(2, "DramaColumns", true);
            
            // Left column: Sequence List
            if (state->showSequenceList)
            {
                ImGui::BeginChild("SequenceList", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
                ImGui::Text("Sequences");
                ImGui::Separator();
                
                // Example sequences
                const char* sequences[] = { "Introduction", "Rising Action", "Climax", "Resolution" };
                for (int i = 0; i < IM_ARRAYSIZE(sequences); i++)
                {
                    bool isSelected = state->selectedSequence == i;
                    if (ImGui::Selectable(sequences[i], isSelected))
                    {
                        state->selectedSequence = i;
                    }
                }
                ImGui::EndChild();
            }
            
            ImGui::NextColumn();
            
            // Right column: Event List
            if (state->showEventList)
            {
                ImGui::BeginChild("EventList");
                ImGui::Text("Events");
                ImGui::Separator();
                
                // Example events
                if (state->selectedSequence >= 0)
                {
                    const char* events[] = { "Character Enter", "Dialogue", "Action", "Exit" };
                    for (int i = 0; i < IM_ARRAYSIZE(events); i++)
                    {
                        bool isSelected = state->selectedEvent == i;
                        if (ImGui::Selectable(events[i], isSelected))
                        {
                            state->selectedEvent = i;
                        }
                    }
                }
                ImGui::EndChild();
            }
            
            ImGui::Columns(1);
            ImGui::EndTabItem();
        }

        if (ImGui::BeginTabItem(ICON_MS_ANALYTICS " Drama Curve"))
        {
            state->activeTab = 1;
            
            ImGui::Text("Tension/Interest Curve");
            ImGui::SliderFloat("##TensionCurve", &state->tensionCurve, 0.0f, 1.0f, "%.2f");
            
            // Simple curve visualization
            ImGui::PlotLines("##CurvePlot", &state->tensionCurve, 1, 0, nullptr, 
                0.0f, 1.0f, ImVec2(-1, 80));
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    // Status bar
    ImGui::Separator();
    ImGui::Text("Status: %s", state->isModified ? "Modified" : "Saved");
    ImGui::SameLine(ImGui::GetWindowWidth() - 120);
    ImGui::Text("Events: %d", 4); // Example count

    ImGui::End();

    // Cleanup if window is closed
    if (!*p_open && state) {
        delete state;
        state = nullptr;
    }
}

} // namespace hdImgui
