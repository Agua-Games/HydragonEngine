/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "Viewport3DTools.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowViewport3DTools(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Viewport 3D Tools", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New")) {}
                if (ImGui::MenuItem("Import SceneGraph", "Ctrl+O")) {}
                if (ImGui::MenuItem("Open SceneGraph", "Ctrl+A")) {}
                if (ImGui::BeginMenu("Open Recent SceneGraph"))
                {
                    ImGui::MenuItem("Museum.usd");
                    ImGui::MenuItem("Restaurant.husd");
                    ImGui::MenuItem("WarHorse.usd");
                    if (ImGui::BeginMenu("More.."))
                    {
                        ImGui::MenuItem("Submarine.usd");
                        ImGui::MenuItem("ArcticStation.usd"); 
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Close SceneGraph", "Ctrl+W")) {}
                if (ImGui::MenuItem("Save SceneGraph", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save SceneGraph As..")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Add SceneGraph As Layer", "Ctrl+Shift+A")) {}
                if (ImGui::MenuItem("Remove Layer", "Ctrl+R")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open in NodeGraph", "Ctrl+M")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Exit", "Alt+F4")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        if (ImGui::CollapsingHeader("SceneGraph")) {
            if (ImGui::CollapsingHeader("Main"))
            {
                ImGui::Button("Import SceneGraph");
            }
            if (ImGui::CollapsingHeader("Layers"))
            {
                ImGui::Button("Add Layer");
                ImGui::Button("Remove Layer");
            }
            if (ImGui::CollapsingHeader("Nodes")) 
            {
                ImGui::Button("Add Node");
                ImGui::Button("Remove Node");
                ImGui::Button("Replace Node");
                ImGui::Button("Duplicate Node");
                ImGui::Button("Rename Node");
                ImGui::Button("Edit Node");
                ImGui::Button("Create Node");
            }
            if (ImGui::CollapsingHeader("Instances")) {
                ImGui::Button("Add Instance");
                ImGui::Button("Remove Instance");
                ImGui::Button("Duplicate Instance");
                ImGui::Button("Override Instance Inherits");
                ImGui::Button("Rename Instance");
                ImGui::Button("Edit Instance");
                ImGui::Button("Create Instance");
            }
            if (ImGui::CollapsingHeader("Compositions")) {
                ImGui::Button("Create Composition");
                ImGui::Button("Duplicate Composition");
                ImGui::Button("Remove Composition");
                ImGui::Button("Rename Composition");
                ImGui::Button("Edit Composition");
            }
            if (ImGui::CollapsingHeader("Variants")) {
                ImGui::Button("Create Variants");
                ImGui::Button("Duplicate Variant");
            }
        }
        if (ImGui::CollapsingHeader("Camera")) {
            ImGui::Button("Perspective On");
            ImGui::Button("Camera Mode");
            ImGui::Button("Camera Orbit");
            ImGui::Button("Camera Pan");
            ImGui::Button("Camera Zoom");
            ImGui::Button("Camera Track");
            ImGui::Button("Camera Walk");
            ImGui::Button("Camera Fly");
            ImGui::Button("Camera Free");
            ImGui::Button("Camera Target");
            ImGui::Button("Camera Follow");
        }
        if (ImGui::CollapsingHeader("Transform")) {
            ImGui::Button("Transform Mode");
            ImGui::Button("Transform Translate");
            ImGui::Button("Transform Rotate");
            ImGui::Button("Transform Scale");
        }
        if (ImGui::CollapsingHeader("Grid")) {
            ImGui::Checkbox("Show Grid", &windowData->showViewport3DGrid);
            ImGui::Checkbox("Snap to Grid", &windowData->snapToViewport3DGrid);
        }
        if (ImGui::CollapsingHeader("Axes")) {
            ImGui::Checkbox("Show Axes", &windowData->showViewport3DAxes);
        }
        if (ImGui::CollapsingHeader("Helpers")) {
            ImGui::Button("Show Helpers");
        }
        if (ImGui::CollapsingHeader("Gizmos")) {
            ImGui::Button("Show Gizmos");
            ImGui::Button("Gizmo Mode");
            ImGui::Button("Gizmo Tools");
        }
        if (ImGui::CollapsingHeader("Selection")) {
            ImGui::Button("Selection Mode");
            ImGui::Button("Selection Tools");
        }
        if (ImGui::CollapsingHeader("Shading")) {
            ImGui::Button("Shading Mode");
            ImGui::Button("Shading Settings");
            ImGui::SliderInt("AntiAliasing", &windowData->viewport3D_AntiAliasing, 0, 8);
        }
        if (ImGui::CollapsingHeader("Lighting")) {
            ImGui::Button("Lighting Mode");
            ImGui::Button("Lighting Settings");
        }
        if (ImGui::CollapsingHeader("Fog")) {
            ImGui::Button("Fog Mode");
            ImGui::Button("Fog Settings");
        }
        if (ImGui::CollapsingHeader("Environment")) {
            ImGui::Button("Environment Mode");
            ImGui::Button("Environment Settings");
        }
        if (ImGui::CollapsingHeader("Skybox")) {
            ImGui::Button("Skybox Mode");
            ImGui::Button("Skybox Settings");
        }
        if (ImGui::CollapsingHeader("Rendering")) {
            ImGui::Button("Rendering Mode");
            ImGui::Button("Buffer Mode");
            ImGui::Button("Rendering Settings");
        }
        if (ImGui::CollapsingHeader("Post-Processing")) {
            ImGui::Button("Post-Processing Mode");
            ImGui::Button("Post-Processing Settings");
        }
        if (ImGui::CollapsingHeader("Effects")) {
            ImGui::Button("Effects Mode");
            ImGui::Button("Effects Settings");
        }
        if (ImGui::CollapsingHeader("Audio")) {
            ImGui::Button("Audio Mode");
            ImGui::Button("Audio Settings");
        }
        if (ImGui::CollapsingHeader("Physics")) {
            ImGui::Button("Physics Mode");
            ImGui::Button("Physics Settings");
        }
        if (ImGui::CollapsingHeader("Brush")) {
            ImGui::Button("Load Brush");
            ImGui::Button("Save Brush");
            ImGui::SliderFloat("Brush Size", &windowData->viewport3D_BrushSize, windowData->viewport3D_BrushMinSize, windowData->viewport3D_BrushMaxSize, nullptr, ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderFloat("Brush Opacity", &windowData->viewport3D_BrushOpacity, windowData->viewport3D_BrushMinOpacity, windowData->viewport3D_BrushMaxOpacity, nullptr, ImGuiSliderFlags_AlwaysClamp);
            ImGui::SliderFloat("Brush Hardness", &windowData->viewport3D_BrushHardness, windowData->viewport3D_BrushMinHardness, windowData->viewport3D_BrushMaxHardness, nullptr, ImGuiSliderFlags_AlwaysClamp);
            ImGui::Button("Load Brush Alpha");
            ImGui::ColorEdit3("Brush Color", windowData->viewport3D_BrushColor, ImGuiColorEditFlags_Float);
            ImGui::Button("Brush Mode");
            ImGui::Button("Brush Tools");
        }
        if (ImGui::CollapsingHeader("UVs")) {
            ImGui::Button("UV Visibility Mode");
            ImGui::Button("UV Tools");
            ImGui::Button("UV Unwrap");
            ImGui::Button("Reproject Texture Keeping UVs");
            ImGui::Button("Add UV Set");
            ImGui::Button("Remove UV Set");
            ImGui::Button("Rename UV Set");
        }
        if (ImGui::CollapsingHeader("Navigation")) {
            ImGui::Button("Navigation Mode");
            ImGui::Button("Navigation Tools");
        }
        if (ImGui::CollapsingHeader("Timeline")) {
            ImGui::Button("Timeline Mode");
            ImGui::Button("Timeline Tools");
        }
        if (ImGui::CollapsingHeader("Animation")) {
            ImGui::Button("Animation Mode");
            ImGui::Button("Animation Tools");
        }
        if (ImGui::CollapsingHeader("Visibility")) {
            ImGui::Button("Visible Only");
            ImGui::Button("Visible Selected");
            ImGui::Button("Visible All");
            ImGui::Button("Visible Layers");
        }
    }
    ImGui::End();
}
} // namespace hdImgui