/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "FontEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowFontEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Font", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Import Font");
        ImGui::Button("New Font");
        ImGui::Button("Save Font");
        ImGui::Button("Load Font");
        ImGui::Button("Export Font");
        ImGui::Button("Share Font");
        ImGui::Button("Add Font");
        ImGui::Button("Remove Font");
        ImGui::Button("Font Settings");
        ImGui::Button("Compile Font");
    }    
    ImGui::End();
}
} // namespace hdImgui