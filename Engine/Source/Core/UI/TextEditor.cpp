/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "TextEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowTextEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Text", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("New Text");
            ImGui::Button("Save Text");
            ImGui::Button("Load Text");
            ImGui::Button("Add Text");
        }
        ImGui::End();
    }
} // namespace hdImgui