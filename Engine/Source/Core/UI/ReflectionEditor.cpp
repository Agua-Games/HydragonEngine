/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "ReflectionEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowReflectionEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Reflection", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Import Reflection Config");
        ImGui::Button("New Reflection Config");
        ImGui::Button("Save Reflection Config");
        ImGui::Button("Load Reflection Config");
        ImGui::Button("Export Reflection Config");
        ImGui::Button("Share Reflection Config");
        ImGui::Button("Reflection Settings");
    }  
    ImGui::End();
}
} // namespace hdImgui