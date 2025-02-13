/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "Viewport2DTools.h"
#include "hdImgui.h"

namespace hdImgui {
    void ShowViewport2DTools(bool* p_open, HdEditorWindowData* windowData) 
    {
        ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
        if (ImGui::Begin("Viewport 2D Tools", p_open))
        {
            if (ImGui::CollapsingHeader("Brush")) {
                ImGui::Button("Load Brush");
                ImGui::Button("Save Brush");
                ImGui::SliderFloat("Brush Size", &windowData->viewport2D_BrushSize, windowData->viewport2D_BrushMinSize, windowData->viewport2D_BrushMaxSize, nullptr, ImGuiSliderFlags_AlwaysClamp);
                ImGui::SliderFloat("Brush Opacity", &windowData->viewport2D_BrushOpacity, windowData->viewport2D_BrushMinOpacity, windowData->viewport2D_BrushMaxOpacity, nullptr, ImGuiSliderFlags_AlwaysClamp);
                ImGui::SliderFloat("Brush Hardness", &windowData->viewport2D_BrushHardness, windowData->viewport2D_BrushMinHardness, windowData->viewport2D_BrushMaxHardness, nullptr, ImGuiSliderFlags_AlwaysClamp);
                ImGui::Button("Load Brush Alpha");
                ImGui::ColorEdit3("Brush Color", (float*)&windowData->viewport2D_BrushColor, ImGuiColorEditFlags_Float);
                ImGui::Button("Brush Mode");
                ImGui::Button("Brush Tools");
            }
            if (ImGui::CollapsingHeader("UVs")) {
                ImGui::Checkbox("Show UVs", &windowData->showViewport2DUVs);
                ImGui::Button("UV Visibility Mode");
                ImGui::Button("UV Tools");
                ImGui::Button("UV Unwrap");
                ImGui::Button("Reproject Texture Keeping UVs");
                ImGui::Button("Add UV Set");
                ImGui::Button("Remove UV Set");
                ImGui::Button("Rename UV Set");
            }
            if (ImGui::CollapsingHeader("Grid")) {
                ImGui::Checkbox("Show Grid", &windowData->showViewport2DGrid);
                ImGui::Checkbox("Snap to Grid", &windowData->snapToViewport2DGrid);
            }
            if (ImGui::CollapsingHeader("Axes")) {
                ImGui::Checkbox("Show Axes", &windowData->showViewport2DAxes);
            }
            if (ImGui::CollapsingHeader("Helpers")) {
                ImGui::Button("Show Helpers");
            }
        }
        ImGui::End();
    }
} // namespace hdImgui