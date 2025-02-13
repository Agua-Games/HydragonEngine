/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "CommandsPalette.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowCommandsPalette(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Commands Palette", nullptr))
        {
           ImGui::Button("Run Command"); 
        }
        ImGui::End();
    }
} // namespace hdImgui