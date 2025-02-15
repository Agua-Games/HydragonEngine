/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "MeshEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowMeshEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Mesh", p_open, ImGuiWindowFlags_MenuBar))
    {
        ImGui::Button("Edit Mesh");
        ImGui::Button("Import Mesh");
        ImGui::Button("Load Mesh");
        ImGui::Button("Save Mesh");
        ImGui::Button("Procedural Tools");      // overlay, context menus
        ImGui::Button("Generate Mesh");
        ImGui::Button("Decimate Mesh");
        ImGui::Button("Voxelize Mesh");
        ImGui::Button("Simplify Mesh");
        ImGui::Button("Optimize Mesh");
        ImGui::Button("Mesh From Points");
        ImGui::Button("Mesh From Point Cloud");
        ImGui::Button("Export Mesh");
        ImGui::Button("Mesh Settings");
        ImGui::Button("Load Mesh Settings");
    }      
    ImGui::End();
}
} // namespace hdImgui