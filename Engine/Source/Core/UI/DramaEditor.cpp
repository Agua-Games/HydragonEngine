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
};

static DramaEditorState* state = nullptr;

void ShowDramaEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!state) {
        state = new DramaEditorState();
    }

    // Basic window setup
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
            
            if (ImGui::MenuItem(ICON_MS_FOLDER_OPEN " Open...", "Ctrl+O")) {
                // TODO: Implement file open dialog
            }
            
            bool canSave = !state->currentDramaName.empty();
            if (ImGui::MenuItem(ICON_MS_SAVE " Save", "Ctrl+S", false, canSave)) {
                state->isModified = false;
            }
            
            ImGui::EndMenu();
        }

        if (ImGui::BeginMenu("Edit"))
        {
            if (ImGui::MenuItem(ICON_MS_SETTINGS " Properties")) {
                // TODO: Show drama properties
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    // Main content area
    ImGui::Text("Drama Sequence: %s", 
        state->currentDramaName.empty() ? "None" : state->currentDramaName.c_str());

    // Basic properties
    if (!state->currentDramaName.empty())
    {
        ImGui::Separator();
        ImGui::Text("Basic Properties");
        
        // Add simple controls that would actually modify the drama
        if (ImGui::Button(ICON_MS_EDIT " Edit Name"))
        {
            // TODO: Implement name editing
            state->isModified = true;
        }
        
        ImGui::SameLine();
        if (ImGui::Button(ICON_MS_DELETE " Delete"))
        {
            state->currentDramaName.clear();
            state->isModified = false;
        }
    }

    ImGui::End();

    // Cleanup if window is closed
    if (!*p_open && state) {
        delete state;
        state = nullptr;
    }
}

} // namespace hdImgui
