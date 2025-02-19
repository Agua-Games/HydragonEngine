/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include "imgui.h"
#include <string>

#include "IconsMaterialSymbols.h"

#include "ScriptEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowScriptEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    if (!p_open || !*p_open)
        return;

    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    if (ImGui::Begin("Script", nullptr, ImGuiWindowFlags_MenuBar))
    {
        // Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Script", "Ctrl+N")) {}
                if (ImGui::MenuItem("Import Script", "Ctrl+O")) {}
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {}
                ImGui::Separator();
                if (ImGui::BeginMenu("Recent Files"))
                {
                    ImGui::MenuItem("player_controller.cpp");
                    ImGui::MenuItem("ai_behavior.py");
                    ImGui::MenuItem("game_rules.cpp");
                    ImGui::EndMenu();
                }
                ImGui::Separator();
                if (ImGui::MenuItem("Send to External IDE", "Ctrl+E")) {}
                if (ImGui::MenuItem("Update Files from Ext. IDE", "Ctrl+R")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "Ctrl+Z")) {}
                if (ImGui::MenuItem("Redo", "Ctrl+Y")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "Ctrl+X")) {}
                if (ImGui::MenuItem("Copy", "Ctrl+C")) {}
                if (ImGui::MenuItem("Paste", "Ctrl+V")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Copy from Node", "Ctrl+Shift+C")) {}
                if (ImGui::MenuItem("Paste to Node", "Ctrl+Shift+V")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("View"))
            {
                if (ImGui::MenuItem("Toggle Line Numbers", "Ctrl+L")) {}
                if (ImGui::MenuItem("Toggle Minimap", "Ctrl+M")) {}
                if (ImGui::MenuItem("Toggle Error List", "Ctrl+E")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Increase Font Size", "Ctrl++")) {}
                if (ImGui::MenuItem("Decrease Font Size", "Ctrl+-")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Script"))
            {
                if (ImGui::MenuItem("Compile", "F7")) {}
                if (ImGui::MenuItem("Run", "F5")) {}
                if (ImGui::MenuItem("Debug", "F9")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Record Macro")) {}
                if (ImGui::MenuItem("Play Macro")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Ask AI Assistant", "Ctrl+Space")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Calculate dimensions
        float availableWidth = ImGui::GetContentRegionAvail().x;
        float leftSidebarWidth = 45.0f;  // Fixed width for left sidebar
        static float scriptTreeWidth = 200.0f;  // Now resizable
        const float minTreeWidth = 150.0f;
        const float maxTreeWidth = 400.0f;
        float mainContentWidth = availableWidth - leftSidebarWidth - scriptTreeWidth - 4.0f; // 4.0f for resizer

        // Remove borders for child windows
        ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0.0f);

        // Main horizontal layout
        ImGui::BeginChild("ScriptEditorContent", ImVec2(0, 0), false, 
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        {
            // Left Toolbar Sidebar
            ImGui::BeginChild("ScriptLeftSidebar", ImVec2(leftSidebarWidth, -1), false);
            {
                // Style settings for a compact vertical toolbar
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(4, 4));
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 2));
                
                ImVec2 buttonSize = windowData->iconDefaultSize;

                // Code Navigation Tools
                if (ImGui::Button(ICON_MS_SEARCH "##FindInFiles", buttonSize)) { /* Handle Find in Files */ }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Find in Files (Ctrl+Shift+F)");

                if (ImGui::Button(ICON_MS_ACCOUNT_TREE "##CodeOutline", buttonSize)) { /* Handle Code Outline */ }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Code Outline");

                ImGui::Separator();

                // Code Analysis Tools
                if (ImGui::Button(ICON_MS_BUG_REPORT "##Debug", buttonSize)) { /* Handle Debug */ }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Debug (F5)");

                if (ImGui::Button(ICON_MS_PSYCHOLOGY "##AIAssist", buttonSize)) { /* Handle AI Assistant */ }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("AI Assistant (Ctrl+Space)");

                ImGui::Separator();

                // Code Generation Tools
                if (ImGui::Button(ICON_MS_AUTO_FIX "##CodeGen", buttonSize)) { /* Handle Code Generation */ }
                if (ImGui::IsItemHovered()) ImGui::SetTooltip("Generate Code");

                ImGui::PopStyleVar(3);
            }
            ImGui::EndChild();
            
            ImGui::SameLine();

            // Script Navigation Tree
            ImGui::BeginChild("ScriptTree", ImVec2(scriptTreeWidth, -1), false);
            {
                // Search bar for scripts
                static char searchScripts[64] = "";
                ImGui::PushItemWidth(-1);
                ImGui::InputTextWithHint("##SearchScripts", ICON_MS_SEARCH " Search Scripts...", searchScripts, IM_ARRAYSIZE(searchScripts));
                ImGui::PopItemWidth();
                ImGui::Separator();

                // Script Tree
                if (ImGui::TreeNodeEx("Scripts", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    // Game Logic folder
                    if (ImGui::TreeNode("Game Logic"))
                    {
                        if (ImGui::TreeNodeEx("player_controller.cpp", ImGuiTreeNodeFlags_Leaf))
                            ImGui::TreePop();
                        if (ImGui::TreeNodeEx("game_manager.cpp", ImGuiTreeNodeFlags_Leaf))
                            ImGui::TreePop();
                        if (ImGui::TreeNode("AI"))
                        {
                            if (ImGui::TreeNodeEx("ai_controller.cpp", ImGuiTreeNodeFlags_Leaf))
                                ImGui::TreePop();
                            if (ImGui::TreeNodeEx("behavior_tree.py", ImGuiTreeNodeFlags_Leaf))
                                ImGui::TreePop();
                            ImGui::TreePop();
                        }
                        ImGui::TreePop();
                    }

                    // Utils folder
                    if (ImGui::TreeNode("Utils"))
                    {
                        if (ImGui::TreeNodeEx("math_helpers.py", ImGuiTreeNodeFlags_Leaf))
                            ImGui::TreePop();
                        if (ImGui::TreeNodeEx("debug_tools.py", ImGuiTreeNodeFlags_Leaf))
                            ImGui::TreePop();
                        ImGui::TreePop();
                    }

                    // Systems folder
                    if (ImGui::TreeNode("Systems"))
                    {
                        if (ImGui::TreeNodeEx("inventory_system.cpp", ImGuiTreeNodeFlags_Leaf))
                            ImGui::TreePop();
                        if (ImGui::TreeNodeEx("quest_system.py", ImGuiTreeNodeFlags_Leaf))
                            ImGui::TreePop();
                        ImGui::TreePop();
                    }

                    ImGui::TreePop();
                }
            }
            ImGui::EndChild();
            
            ImGui::SameLine();

            // Resizer
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.5f, 0.5f, 0.5f, 0.3f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.7f, 0.7f, 0.7f, 0.4f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.9f, 0.9f, 0.9f, 0.5f));
            ImGui::Button("##Resizer", ImVec2(4.0f, -1));
            if (ImGui::IsItemActive())
            {
                scriptTreeWidth += ImGui::GetIO().MouseDelta.x;
                if (scriptTreeWidth < minTreeWidth) scriptTreeWidth = minTreeWidth;
                if (scriptTreeWidth > maxTreeWidth) scriptTreeWidth = maxTreeWidth;
            }
            if (ImGui::IsItemHovered())
                ImGui::SetMouseCursor(ImGuiMouseCursor_ResizeEW);
            ImGui::PopStyleColor(3);
            
            ImGui::SameLine();

            // Main Editor Area
            ImGui::BeginChild("ScriptEditorMain", ImVec2(mainContentWidth, -1), false);
            {
                // Top Toolbar
                ImGui::BeginChild("EditorToolbar", ImVec2(-1, 30), false);
                {
                    ImVec2 buttonSize = windowData->iconDefaultSize;

                    if (ImGui::Button(ICON_MS_SAVE "##Save", buttonSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Save (Ctrl+S)");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_UNDO "##Undo", buttonSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Undo (Ctrl+Z)");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_REDO "##Redo", buttonSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Redo (Ctrl+Y)");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_SEARCH "##Find", buttonSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Find (Ctrl+F)");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_FIND_REPLACE "##Replace", buttonSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("Replace (Ctrl+H)");
                    ImGui::SameLine();

                    if (ImGui::Button(ICON_MS_PSYCHOLOGY "##AIAssistant", buttonSize)) {}
                    if (ImGui::IsItemHovered()) ImGui::SetTooltip("AI Assistant (Ctrl+Space)");
                }
                ImGui::EndChild();

                // Editor Content Area (Tab bar and text editor)
                ImGui::BeginChild("EditorContent", ImVec2(-1, -20)); // -20 for status bar
                {
                    if (ImGui::BeginTabBar("ScriptTabs", ImGuiTabBarFlags_Reorderable))
                    {
                        // Python script tab
                        if (ImGui::BeginTabItem("math_helpers.py"))
                        {
                            const float lineNumbersWidth = 30.0f; // Width of line numbers column
                            ImGui::BeginChild("LineNumbers", ImVec2(lineNumbersWidth, -1), false);
                            {
                                // Match the style of the main text area
                                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
                                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.5f, 0.5f, 0.5f, 1.0f)); // Grayed out color

                                // Calculate visible lines based on current scroll position
                                float lineHeight = ImGui::GetTextLineHeight();
                                int visibleLines = (int)(ImGui::GetWindowHeight() / lineHeight);
                                float scrollY = ImGui::GetScrollY();
                                int startLine = (int)(scrollY / lineHeight);

                                // Draw line numbers
                                for (int i = 0; i <= visibleLines; i++)
                                {
                                    int lineNo = startLine + i + 1;
                                    ImGui::Text("%3d", lineNo);
                                }

                                ImGui::PopStyleColor();
                                ImGui::PopStyleVar();
                            }
                            ImGui::EndChild();
                            ImGui::SameLine(0, 0); // No spacing between line numbers and text

                            // Main text area
                            ImGui::BeginChild("TextArea", ImVec2(-1, -1), false);
                            {
                                static char pythonText[4096] = 
                                    "def calculate_trajectory(velocity, angle):\n"
                                    "    # Calculate projectile trajectory\n"
                                    "    pass\n\n"
                                    "def interpolate_bezier(p0, p1, p2, t):\n"
                                    "    # Bezier curve interpolation\n"
                                    "    pass";
                                
                                // Sync scroll position between line numbers and text
                                if (ImGui::IsWindowFocused())
                                {
                                    float scroll = ImGui::GetScrollY();
                                    ImGui::SetScrollY(scroll);
                                }

                                ImGui::InputTextMultiline("##source_python", pythonText, IM_ARRAYSIZE(pythonText), 
                                    ImVec2(-1, -1), ImGuiInputTextFlags_AllowTabInput);
                            }
                            ImGui::EndChild();
                            ImGui::EndTabItem();
                        }

                        // C++ script tabs
                        if (ImGui::BeginTabItem("player_controller.cpp"))
                        {
                            static char cppText1[4096] = 
                                "#include \"PlayerController.h\"\n\n"
                                "void PlayerController::Update(float deltaTime)\n"
                                "{\n"
                                "    // Handle player input\n"
                                "    HandleMovement(deltaTime);\n"
                                "    HandleActions();\n"
                                "}";
                            ImGui::InputTextMultiline("##source_cpp1", cppText1, IM_ARRAYSIZE(cppText1), 
                                ImVec2(-1, -1), ImGuiInputTextFlags_AllowTabInput);
                            ImGui::EndTabItem();
                        }

                        if (ImGui::BeginTabItem("inventory_system.cpp"))
                        {
                            static char cppText2[4096] = 
                                "#include \"InventorySystem.h\"\n\n"
                                "bool InventorySystem::AddItem(const Item& item)\n"
                                "{\n"
                                "    // Check inventory space\n"
                                "    // Add item logic\n"
                                "    return true;\n"
                                "}";
                            ImGui::InputTextMultiline("##source_cpp2", cppText2, IM_ARRAYSIZE(cppText2), 
                                ImVec2(-1, -1), ImGuiInputTextFlags_AllowTabInput);
                            ImGui::EndTabItem();
                        }

                        ImGui::EndTabBar();
                    }
                }
                ImGui::EndChild();

                // Status Bar at bottom
                ImGui::BeginChild("StatusBar", ImVec2(-1, 20), false);
                {
                    ImGui::Text("Ln 1, Col 1");
                    ImGui::SameLine();
                    ImGui::Text("UTF-8");
                    ImGui::SameLine();
                    ImGui::Text("Python"); // Will change based on active tab
                }
                ImGui::EndChild();
            }
            ImGui::EndChild();
        }
        ImGui::EndChild();

        ImGui::PopStyleVar(); // Pop the border size style
    }
    ImGui::End();
}
} // namespace hdImgui
