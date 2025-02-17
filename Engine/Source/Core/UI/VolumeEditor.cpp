/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include "VolumeEditor.h"
#include "hdImgui.h"

namespace hdImgui {

static VolumeToolSettings g_volumeToolSettings;

void ShowVolumeEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Volume Editor", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Volume", "Ctrl+N")) {}
                if (ImGui::MenuItem("Import VDB...", "Ctrl+O")) {}
                if (ImGui::MenuItem("Import Point Cloud...", "Ctrl+Shift+O")) {}
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("Recent Volumes"))
                {
                    ImGui::MenuItem("smoke_sim.vdb");
                    ImGui::MenuItem("fluid_cache.vdb");
                    ImGui::MenuItem("terrain_height.vdb");
                    ImGui::EndMenu();
                }
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Volume"))
            {
                if (ImGui::MenuItem("Convert to Mesh")) {}
                if (ImGui::MenuItem("Convert from Mesh")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Resample")) {}
                if (ImGui::MenuItem("Prune")) {}
                if (ImGui::MenuItem("Optimize")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Point Cloud"))
            {
                if (ImGui::MenuItem("Convert to Volume")) {}
                if (ImGui::MenuItem("Generate Surface")) {}
                if (ImGui::MenuItem("Filter Points")) {}
                if (ImGui::MenuItem("Resample Points")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Boolean Operations")) {}
                if (ImGui::MenuItem("Procedural Effects")) {}
                if (ImGui::MenuItem("Level Set Tools")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Generate LODs")) {}
                if (ImGui::MenuItem("Compress Volume")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Main content area with toolbar and viewport
        ImGui::BeginChild("VolumeContent", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
        {
            // Toolbar
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 3));
            if (ImGui::Button("Transform")) {}
            ImGui::SameLine();
            if (ImGui::Button("Crop")) {}
            ImGui::SameLine();
            if (ImGui::Button("Fill")) {}
            ImGui::PopStyleVar();

            // Store the current sidebar width in a static variable
            static float sidebarWidth = 300.0f;
            const float minSidebarWidth = 200.0f;  // Minimum width
            const float maxSidebarWidth = 500.0f;  // Maximum width

            // Main Viewport
            ImGui::BeginChild("ViewportArea", ImVec2(ImGui::GetContentRegionAvail().x - sidebarWidth - 8.0f, 0), true);
            {
                // Viewport content here
                ImGui::Text("Volume Viewport");
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
                if (ImGui::BeginTabBar("VolumeTabs"))
                {
                    if (ImGui::BeginTabItem("Tools"))
                    {
                        ShowVolumeToolsPanel(g_volumeToolSettings);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Point Cloud"))
                    {
                        ShowPointCloudPanel(g_volumeToolSettings);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("Procedural"))
                    {
                        ShowProceduralPanel(g_volumeToolSettings);
                        ImGui::EndTabItem();
                    }
                    if (ImGui::BeginTabItem("VDB"))
                    {
                        ShowVDBPanel();
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

void ShowVolumeToolsPanel(VolumeToolSettings& settings)
{
    if (ImGui::CollapsingHeader("Volume Operations", ImGuiTreeNodeFlags_DefaultOpen))
    {
        const char* opModes[] = { "Union", "Difference", "Intersection", "Smooth", "Sharpen", "Erode" };
        int currentMode = (int)settings.operation;
        if (ImGui::Combo("Operation", &currentMode, opModes, IM_ARRAYSIZE(opModes)))
        {
            settings.operation = (VolumeToolSettings::OperationType)currentMode;
        }
        ImGui::SliderFloat("Voxel Size", &settings.voxelSize, 0.01f, 1.0f);
        ImGui::SliderFloat("Density Threshold", &settings.densityThreshold, 0.0f, 1.0f);
        ImGui::Checkbox("Preserve Details", &settings.preserveDetails);
        if (ImGui::Button("Apply Operation")) {}
    }

    if (ImGui::CollapsingHeader("Mesh Conversion"))
    {
        ImGui::SliderFloat("Bandwidth", &settings.meshToVolume.bandwidthSize, 1.0f, 10.0f);
        ImGui::SliderFloat("Surface Thickness", &settings.meshToVolume.surfaceThickness, 0.1f, 5.0f);
        ImGui::Checkbox("Fill Interior", &settings.meshToVolume.fillInterior);
        if (ImGui::Button("Convert Mesh to Volume")) {}
    }
}

void ShowPointCloudPanel(VolumeToolSettings& settings)
{
    if (ImGui::CollapsingHeader("Point Cloud Processing", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Particle Radius", &settings.pointCloud.particleRadius, 0.01f, 1.0f);
        ImGui::SliderFloat("Smoothing Radius", &settings.pointCloud.smoothingRadius, 0.01f, 1.0f);
        ImGui::SliderInt("Smoothing Iterations", &settings.pointCloud.smoothingIterations, 1, 10);
        ImGui::Checkbox("Adaptive Radius", &settings.pointCloud.adaptiveRadius);
        if (ImGui::Button("Process Points")) {}
    }

    if (ImGui::CollapsingHeader("Surface Generation"))
    {
        if (ImGui::Button("Generate Mesh from Points")) {}
        if (ImGui::Button("Generate Volume from Points")) {}
    }
}

void ShowProceduralPanel(VolumeToolSettings& settings)
{
    if (ImGui::CollapsingHeader("Noise Generation", ImGuiTreeNodeFlags_DefaultOpen))
    {
        ImGui::SliderFloat("Scale", &settings.proceduralParams.noiseScale, 0.1f, 10.0f);
        ImGui::SliderFloat("Strength", &settings.proceduralParams.noiseStrength, 0.0f, 2.0f);
        ImGui::InputInt("Seed", &settings.proceduralParams.noiseSeed);
        if (ImGui::Button("Apply Noise")) {}
    }

    if (ImGui::CollapsingHeader("Flow Effects"))
    {
        ImGui::SliderFloat("Erosion Rate", &settings.proceduralParams.erosionRate, 0.0f, 1.0f);
        ImGui::SliderInt("Flow Iterations", &settings.proceduralParams.flowIterations, 1, 20);
        if (ImGui::Button("Simulate Flow")) {}
    }
}

void ShowVDBPanel()
{
    if (ImGui::CollapsingHeader("VDB Properties", ImGuiTreeNodeFlags_DefaultOpen))
    {
        if (ImGui::Button("Analyze Grid")) {}
        if (ImGui::Button("Optimize Storage")) {}
    }

    if (ImGui::CollapsingHeader("Grid Operations"))
    {
        if (ImGui::Button("Resample Grid")) {}
        if (ImGui::Button("Transform Grid")) {}
        if (ImGui::Button("Combine Grids")) {}
    }

    if (ImGui::CollapsingHeader("Compression"))
    {
        if (ImGui::Button("Compress VDB")) {}
        if (ImGui::Button("Convert to MiniVDB")) {}
    }
}

} // namespace hdImgui
