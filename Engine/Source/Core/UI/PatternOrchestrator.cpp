/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "PatternOrchestrator.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowPatternOrchestrator(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Pattern Orchestrator", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Load Pattern");
    }  
    ImGui::End();
}
} // namespace hdImgui