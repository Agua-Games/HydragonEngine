/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include "MeshEditor.h"
#include "hdImgui.h"

namespace hdImgui {

static MeshToolSettings g_meshToolSettings;

void ShowMeshEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Mesh Editor", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Mesh", "Ctrl+N")) {}
                if (ImGui::MenuItem("Import Mesh...", "Ctrl+O")) {}
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("Recent Meshes"))
                {
                    ImGui::MenuItem("character.usd");
                    ImGui::MenuItem("prop_table.obj");
                    ImGui::MenuItem("environment.fbx");
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Export Selected...", "Ctrl+E")) {}
                if (ImGui::MenuItem("Export All...")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Delete", "Del")) {}
                if (ImGui::MenuItem("Duplicate", "Ctrl+D")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Select All", "Ctrl+A")) {}
                if (ImGui::MenuItem("Select None", "Ctrl+Shift+A")) {}
                if (ImGui::MenuItem("Invert Selection", "Ctrl+I")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Mesh"))
            {
                if (ImGui::MenuItem("Triangulate")) {}
                if (ImGui::MenuItem("Quadrangulate")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Smooth")) {}
                if (ImGui::MenuItem("Subdivide")) {}
                if (ImGui::MenuItem("Decimate")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("UV"))
                {
                    if (ImGui::MenuItem("Generate UVs")) {}
                    if (ImGui::MenuItem("Pack UVs")) {}
                    if (ImGui::MenuItem("Straighten UVs")) {}
                    if (ImGui::MenuItem("Auto Seams")) {}
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("USD"))
            {
                if (ImGui::MenuItem("Import USD Layer")) {}
                if (ImGui::MenuItem("Export as USD")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Compose Layers")) {}
                if (ImGui::MenuItem("Edit Variants")) {}
                if (ImGui::MenuItem("Manage References")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Boolean Operations")) {}
                if (ImGui::MenuItem("Procedural Effects")) {}
                if (ImGui::MenuItem("Batch Process")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Generate LODs")) {}
                if (ImGui::MenuItem("Bake Maps")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Main content area with toolbar and viewport
        ImGui::BeginChild("MeshContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
        {
            // Toolbar
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
            ImGui::BeginChild("Toolbar", ImVec2(ImGui::GetWindowWidth(), 40), true);
            {
                if (ImGui::Button("Select")) {}
                ImGui::SameLine();
                if (ImGui::Button("Move")) {}
                ImGui::SameLine();
                if (ImGui::Button("Rotate")) {}
                ImGui::SameLine();
                if (ImGui::Button("Scale")) {}
                ImGui::SameLine();
                ImGui::Separator();
                ImGui::SameLine();
                
                // Snap settings
                ImGui::TextUnformatted("Snap:"); ImGui::SameLine();
                static bool snapToGrid = false;
                ImGui::Checkbox("Grid", &snapToGrid); ImGui::SameLine();
                static bool snapToVertex = false;
                ImGui::Checkbox("Vertex", &snapToVertex);
            }
            ImGui::EndChild();
            ImGui::PopStyleVar();

            // Store the current sidebar width in a static variable
            static float sidebarWidth = 300.0f;
            const float minSidebarWidth = 200.0f;  // Minimum width
            const float maxSidebarWidth = 500.0f;  // Maximum width

            // Main Viewport
            ImGui::BeginChild("ViewportArea", ImVec2(ImGui::GetContentRegionAvail().x - sidebarWidth - 8.0f, 0), true);
            {
                // Viewport content here
                ImGui::Text("Mesh Viewport");
            }
            ImGui::EndChild();

            ImGui::SameLine();

            // Splitter
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
            ImGui::Button("##Splitter", ImVec2(8.0f, -1));
            ImGui::PopStyleColor(3);

            if (ImGui::IsItemActive())
            {
                float deltaX = ImGui::GetIO().MouseDelta.x;
                sidebarWidth -= deltaX;
                if (sidebarWidth < minSidebarWidth) sidebarWidth = minSidebarWidth;
                if (sidebarWidth > maxSidebarWidth) sidebarWidth = maxSidebarWidth;
            }
            else if (ImGui::IsItemHovered())
            {
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            }

            ImGui::SameLine();

            // Right panel with tools
            ImGui::BeginChild("RightPanel", ImVec2(sidebarWidth, 0), true);
            {
                if (ImGui::BeginTabBar("MeshTabs"))
                {
                    if (ImGui::BeginTabItem("Tools"))
                    {
                        ShowMeshToolsPanel(g_meshToolSettings);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("UVs"))
                    {
                        ShowUVPanel(g_meshToolSettings);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Procedural"))
                    {
                        ShowProceduralPanel(g_meshToolSettings);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("USD"))
                    {
                        ShowUSDPanel();
                        ImGui::EndTabItem();
                    }
                    ImGui::EndTabBar();
                }
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();
    }
    ImGui::End();
}

void ShowMeshToolsPanel(MeshToolSettings& settings)
{
    if (ImGui::CollapsingHeader("Decimation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Target Reduction", &settings.targetReduction, 0.0f, 1.0f);
        ImGui::Checkbox("Preserve UVs", &settings.preserveUVs);
        ImGui::Checkbox("Preserve Normals", &settings.preserveNormals);
        if (ImGui::Button("Apply Decimation")) {}
    }

    if (ImGui::CollapsingHeader("Remeshing"))
    {
        ImGui::SliderFloat("Voxel Size", &settings.voxelSize, 0.01f, 1.0f);
        ImGui::SliderInt("Smooth Iterations", &settings.smoothIterations, 0, 10);
        ImGui::Checkbox("Adaptive Size", &settings.adaptiveSize);
        if (ImGui::Button("Remesh")) {}
    }

    if (ImGui::CollapsingHeader("Boolean Operations"))
    {
        const char* boolModes[] = { "Union", "Difference", "Intersection", "Slice" };
        int currentMode = (int)settings.booleanMode;
        if (ImGui::Combo("Operation", &currentMode, boolModes, IM_ARRAYSIZE(boolModes)))
        {
            settings.booleanMode = (MeshToolSettings::BooleanMode)currentMode;
        }
        if (ImGui::Button("Apply Boolean")) {}
    }
}

void ShowUVPanel(MeshToolSettings& settings)
{
    if (ImGui::CollapsingHeader("UV Generation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::Checkbox("Auto Seams", &settings.uvParams.autoSeams);
        ImGui::SliderFloat("Angle Threshold", &settings.uvParams.angleThreshold, 0.0f, 180.0f);
        if (ImGui::Button("Generate UVs")) {}
    }

    if (ImGui::CollapsingHeader("UV Packing"))
    {
        ImGui::SliderFloat("Margin", &settings.uvParams.packingMargin, 0.0f, 0.1f);
        ImGui::Checkbox("Preserve Aspect", &settings.uvParams.preserveAspectRatio);
        if (ImGui::Button("Pack UVs")) {}
    }
}

void ShowProceduralPanel(MeshToolSettings& settings)
{
    if (ImGui::CollapsingHeader("Time-Based Effects", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Erosion", &settings.proceduralParams.erosionStrength, 0.0f, 5.0f);
        ImGui::SliderFloat("Gravity", &settings.proceduralParams.gravityInfluence, 0.0f, 5.0f);
        ImGui::SliderFloat("Decay", &settings.proceduralParams.decayRate, 0.0f, 1.0f);
        if (ImGui::Button("Apply Effect")) {}
    }

    if (ImGui::CollapsingHeader("Fracturing"))
    {
        ImGui::InputInt("Seed", &settings.proceduralParams.fractureSeed);
        ImGui::SliderInt("Pieces", &settings.proceduralParams.fracturePieces, 2, 50);
        if (ImGui::Button("Generate Fracture")) {}
    }
}

void ShowUSDPanel()
{
    if (ImGui::CollapsingHeader("USD Layers", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Add Layer")) {}
        if (ImGui::Button("Compose Layers")) {}
    }

    if (ImGui::CollapsingHeader("Variants"))
    {
        if (ImGui::Button("Add Variant Set")) {}
        if (ImGui::Button("Edit Variants")) {}
    }

    if (ImGui::CollapsingHeader("References"))
    {
        if (ImGui::Button("Add Reference")) {}
        if (ImGui::Button("Manage References")) {}
    }
}

} // namespace hdImgui
