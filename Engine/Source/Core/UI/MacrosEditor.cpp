/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "MacrosEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowMacrosEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Macros", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Save Macro");
        ImGui::Button("Load Macro");
    }  
    ImGui::End();
}
} // namespace hdImgui