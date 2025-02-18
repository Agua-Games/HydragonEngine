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
    
    // Drama Tensor States
    float tensionCurve = 0.5f;
    float characterBias = 0.0f;
    float destinyForce = 0.0f;
    float conflictIntensity = 0.5f;
    float harmonicBalance = 0.5f;
    float chaosField = 0.0f;
    float trajectoryMomentum = 0.0f;
    
    int activeTab = 0;
    
    // Trajectory Integration
    float trajectoryPoints[100] = {0};
    int trajectoryPointCount = 0;
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

    // Toolbar with proper spacing
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));
    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 3));
    
    ImVec2 buttonSize(32, 32);
    float toolbarHeight = 40;
    
    ImGui::BeginChild("##Toolbar", ImVec2(-1, toolbarHeight), false);
    {
        if (ImGui::Button(ICON_MS_ADD_BOX "##NewSequence", buttonSize)) { state->isModified = true; }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Sequence");
        
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_ADD_CIRCLE "##NewEvent", buttonSize)) { state->isModified = true; }
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("New Event");
        
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(10, 0));
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_PREVIEW "##Preview", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Preview Drama");
        
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_PLAY_ARROW "##Test", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Test Drama Flow");
        
        ImGui::SameLine();
        ImGui::Dummy(ImVec2(10, 0));
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_ANALYTICS "##DramaTensor", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Drama Tensor Analysis");
        
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_TIMELINE "##Trajectory", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Character Trajectory");
    }
    ImGui::EndChild();
    
    ImGui::PopStyleVar(2);

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

        if (ImGui::BeginTabItem(ICON_MS_ANALYTICS " Drama Tensors"))
        {
            state->activeTab = 1;
            
            ImGui::BeginChild("DramaTensors", ImVec2(0, 0), true);
            
            // Drama Tensor Analysis
            ImGui::Text("Drama Tensor Analysis");
            ImGui::Separator();
            
            // Character Trajectory
            ImGui::Text("Character Trajectory Integration");
            ImGui::PlotLines("##TrajectoryPlot", state->trajectoryPoints, 
                state->trajectoryPointCount, 0, "Character Arc", -1.0f, 1.0f, ImVec2(-1, 80));
            
            // Tension Forces
            ImGui::Text("Dramatic Forces");
            ImGui::SliderFloat("Tension", &state->tensionCurve, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat("Character Bias", &state->characterBias, -1.0f, 1.0f, "%.2f");
            ImGui::SliderFloat("Destiny Force", &state->destinyForce, -1.0f, 1.0f, "%.2f");
            
            ImGui::Separator();
            
            // Conflict and Harmony
            ImGui::Text("Conflict Dynamics");
            ImGui::SliderFloat("Conflict Intensity", &state->conflictIntensity, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat("Harmonic Balance", &state->harmonicBalance, 0.0f, 1.0f, "%.2f");
            ImGui::SliderFloat("Chaos Field", &state->chaosField, 0.0f, 1.0f, "%.2f");
            
            // Momentum and Flow
            ImGui::Text("Narrative Momentum");
            ImGui::SliderFloat("Trajectory Momentum", &state->trajectoryMomentum, -1.0f, 1.0f, "%.2f");
            
            ImGui::EndChild();
            
            ImGui::EndTabItem();
        }

        ImGui::EndTabBar();
    }

    // Status bar
    ImGui::Separator();
    ImGui::Text("Status: %s", state->isModified ? "Modified" : "Saved");
    ImGui::SameLine();
    ImGui::Text("| Tension: %.2f", state->tensionCurve);
    ImGui::SameLine();
    ImGui::Text("| Momentum: %.2f", state->trajectoryMomentum);
    ImGui::SameLine(ImGui::GetWindowWidth() - 120);
    ImGui::Text("Events: %d", 4);

    ImGui::End();

    // Cleanup if window is closed
    if (!*p_open && state) {
        delete state;
        state = nullptr;
    }
}

} // namespace hdImgui
