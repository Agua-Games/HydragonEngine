/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "UIEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowUIEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("UI", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Import UI");
            ImGui::Button("New UI");
            ImGui::Button("Save UI");
            ImGui::Button("Load UI");
            ImGui::Button("Export UI");
            ImGui::Button("Share UI");
            ImGui::Button("Add UI");
            ImGui::Button("Remove UI");
            ImGui::Button("UI Settings");
        }    
        ImGui::End();
    }
} // namespace hdImgui