/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "PropertiesMatrixEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowPropertiesMatrixEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Properties Matrix", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("New Selected Objects");
        ImGui::Button("Save Selected Objects");
        ImGui::Button("Load Selected Objects");
        ImGui::Button("Diff Selected");
    } 
    ImGui::End();
}
} // namespace hdImgui