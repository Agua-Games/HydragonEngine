/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "SceneGraphEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowSceneGraphEditor(bool* p_open, HdEditorWindowData* windowData)
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("SceneGraph ", p_open, ImGuiWindowFlags_MenuBar))
    {
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New SceneGraph")) {}
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
            if (ImGui::BeginMenu("Compose"))
            {
                if (ImGui::MenuItem("Add Layer", "CTRL+A")) {}
                if (ImGui::MenuItem("Duplicate Layer", "CTRL+D")) {} 
                ImGui::Separator();
                if (ImGui::MenuItem("Replace Layer", "CTRL+R")) {}
                if (ImGui::MenuItem("Remove Layer", "CTRL+R")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Move Layer Up", "CTRL+UP")) {}
                if (ImGui::MenuItem("Move Layer Down", "CTRL+DOWN")) {}
                ImGui::Separator();
                
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Instance"))
            {
                if (ImGui::MenuItem("Add Instance", "CTRL+A")) {}
                if (ImGui::MenuItem("Duplicate Instance", "CTRL+D")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Replace Instance", "CTRL+R")) {}
                if (ImGui::MenuItem("Remove Instance", "CTRL+R")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Open in NodeGraph", "CTRL+M")) {}
                ImGui::EndMenu();
            }   
            ImGui::EndMenuBar();
        }
        // Example: TreeNode with flags
        ImGuiTreeNodeFlags SceneGraphTreeNodeFlags = 
            ImGuiTreeNodeFlags_DefaultOpen | 
            ImGuiTreeNodeFlags_Framed | 
            ImGuiTreeNodeFlags_OpenOnArrow |
            ImGuiTreeNodeFlags_OpenOnDoubleClick;
        if (ImGui::TreeNodeEx("Castle", SceneGraphTreeNodeFlags)) {
            // Child nodes
            if (ImGui::TreeNode("Tower 1")) {
                if (ImGui::TreeNode("Door 1")){
                    ImGui::Text("Handle 1");
                    ImGui::TreePop();
                }
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Tower 2")) {
                ImGui::Text("Bullwark 1");
                ImGui::TreePop();
            }
            ImGui::TreePop();
        }
        if (ImGui::TreeNodeEx("Bridge", SceneGraphTreeNodeFlags)) {
            // Child nodes
            if (ImGui::TreeNode("Ending Node 1")) {
                ImGui::Text("Tower 1");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Shaft 1")) {
                ImGui::Text("This is the second child node.");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Ending Node 2")) {
                ImGui::Text("Tower 1");
                ImGui::TreePop();
            }
            // End the root node
            ImGui::TreePop();
        }
        ImGui::Separator();
        if (ImGui::CollapsingHeader("SceneGraph Layers"))
        {
            ImGui::Button("Add Layer");
            ImGui::Button("Duplicate Layer");
            ImGui::Button("Replace Layer");
            ImGui::Button("Remove Layer");
        }
        if (ImGui::CollapsingHeader("Streaming"))
        {
            static int s_streamingMethod = 1;
            static int s_streamingPriority = 0;
            ImGui::Combo("Method", &s_streamingMethod, "Lazy\0Standard\0Priority\0", 3);
            ImGui::Combo("Priority", &s_streamingPriority, "Low\0Medium\0High\0", 3);
            ImGui::Separator();
            ImGui::Button("Open in NodeGraph");
        }
    }
    ImGui::End();   
}
} // namespace hdImgui