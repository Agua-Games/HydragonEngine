/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "InputEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowInputEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Input", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Import Input Config");
        ImGui::Button("New Input Config");
        ImGui::Button("Load Input Config");
        ImGui::Button("Save Input Config");
        ImGui::Button("Export Input Config");
        ImGui::Button("Share Input Config");
    }   
    ImGui::End();
    }
} // namespace hdImgui