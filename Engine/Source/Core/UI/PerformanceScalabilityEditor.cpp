/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "PerformanceScalabilityEditor.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowPerformanceScalabilityEditor(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Performance Scalability", p_open, ImGuiWindowFlags_MenuBar))
        {
            ImGui::Button("Import Performance Scalability Config");
            ImGui::Button("New Performance Scalability Config");
            ImGui::Button("Save Performance Scalability Config");
            ImGui::Button("Load Performance Scalability Config");
            ImGui::Button("Export Performance Scalability Config");
            ImGui::Button("Share Performance Scalability Config");
            ImGui::Button("Performance Scalability Settings");
        }    
        ImGui::End();
    }
} // namespace hdImgui