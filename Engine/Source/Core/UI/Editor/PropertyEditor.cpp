/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PropertyEditor.cpp
 * @brief The Property Editor is a specialized view for property-based nodes.
 */
#pragma once
#include <imgui.h>
#include <string>

#include "PropertyEditor.h"
#include "UIManager.h"

namespace hd {

void showPropertyEditor(bool* p_open, EditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Properties", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Load Template");
    }
    ImGui::End();
}

} // namespace hd