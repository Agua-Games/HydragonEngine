/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "LeftToolbar.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowLeftToolbar(bool* p_open, HdEditorWindowData* windowData) 
{
    if (ImGui::Begin("Left Toolbar", p_open))
    {
        ImGui::Button("Test1");
    }
    ImGui::End();
}
} // namespace hdImgui