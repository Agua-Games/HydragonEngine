/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralEditor.cpp
 * @brief The Procedural Editor is a specialized view for procedural pattern orchestration.
 */
#pragma once
#include <string>
#include <imgui.h>

#include "ProceduralManagerEditor.h"
#include "UIManager.h"

namespace hd {
  
void showProceduralManager(bool* p_open, EditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Procedural Manager", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Load Pattern");
    }  
    ImGui::End();
}

} // namespace hd