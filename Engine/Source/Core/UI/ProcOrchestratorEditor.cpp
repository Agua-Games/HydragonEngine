/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcOrchestratorEditor.cpp
 * @brief The Procedural Orchestrator Editor is a specialized view for procedural pattern orchestration.
 */
#pragma once
#include <string>
#include <imgui.h>

#include "ProcOrchestratorEditor.h"
#include "UIManager.h"

namespace hd {
  
void showProcOrchestrator(bool* p_open, EditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Pattern Orchestrator", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Load Pattern");
    }  
    ImGui::End();
}

} // namespace hd