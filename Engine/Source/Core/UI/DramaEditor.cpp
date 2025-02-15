/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "DramaEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowDramaEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Drama", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Import Drama");
        ImGui::Button("New Drama");
        ImGui::Button("Save Drama");
        ImGui::Button("Load Drama");
        ImGui::Button("Play Drama");
        ImGui::Button("Stop Drama");
        ImGui::Button("Export Drama");
        ImGui::Button("Share Drama");
        ImGui::Button("Add Drama");
        ImGui::Button("Remove Drama");
        ImGui::Button("Drama Settings");
    }      
    ImGui::End();
}
} // namespace hdImgui