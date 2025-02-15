/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "DebugEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowDebugEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Debug", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Clear Terminal");
        ImGui::Button("Save Debug Log");
        ImGui::Separator();
        ImGui::Button("Debug Settings");
    }      
    ImGui::End();
}
} // namespace hdImgui