/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BottomToolbar.cpp
 * @brief The bottom toolbar is a specialized view for displaying status information.
 */
#pragma once
#include <imgui.h>
#include <string>

#include "BottomToolbar.h"
#include "UIManager.h"

namespace hd {
void ShowBottomToolbar(bool* p_open, EditorWindowData* windowData) 
{
    if (ImGui::Begin("Bottom Toolbar", p_open))
    {
        ImGui::Button("Test1");
    }
    ImGui::End();
}
} // namespace hd