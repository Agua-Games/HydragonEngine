/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>  // Include ImGui's header
#include <string>

#include "AgentsEditor.h"
#include "hdImgui.h"

namespace hdImgui {
void ShowAgentsEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    if (ImGui::Begin("Agents", p_open, ImGuiWindowFlags_MenuBar))
    {
        // Main Menu Bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New Agent")) {}
                if (ImGui::MenuItem("Load Agent")) {}
                if (ImGui::MenuItem("Save Agent")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Import Agent")) {}
                if (ImGui::MenuItem("Export Agent")) {}
                ImGui::Separator();
                if (ImGui::MenuItem("Exit")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Server"))
            {
                if (ImGui::MenuItem("Start Server")) {}
                if (ImGui::MenuItem("Stop Server")) {}
                if (ImGui::MenuItem("Server Settings...")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Tools"))
            {
                if (ImGui::MenuItem("Model Manager")) {}
                if (ImGui::MenuItem("Task Manager")) {}
                if (ImGui::MenuItem("Batch Processing")) {}
                if (ImGui::MenuItem("Performance Monitor")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Help"))
            {
                if (ImGui::MenuItem("Documentation")) {}
                if (ImGui::MenuItem("About")) {}
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // Status Bar
        {
            ImGui::TextColored(ImVec4(0.0f, 0.8f, 0.0f, 1.0f), "●"); ImGui::SameLine();
            ImGui::Text("Server Status: Running"); ImGui::SameLine(200);
            ImGui::Text("Model: GPT-4"); ImGui::SameLine(400);
            ImGui::Text("Tasks: 3/10"); ImGui::SameLine(600);
            ImGui::Text("Memory Usage: 1.2GB");
        }
        ImGui::Separator();

        // Main Content Area with Tabs
        static int selected_tab = 0;
        ImGuiTabBarFlags tab_bar_flags = ImGuiTabBarFlags_None;
        if (ImGui::BeginTabBar("AgentTabs", tab_bar_flags))
        {
            // Agent Task Tab
            if (ImGui::BeginTabItem("Agent Task"))
            {
                // Task Menu Bar
                if (ImGui::BeginMenuBar())
                {
                    if (ImGui::MenuItem("New Task")) {}
                    if (ImGui::MenuItem("Task History")) {}
                    if (ImGui::MenuItem("Export Task")) {}
                    ImGui::EndMenuBar();
                }

                // Agent Output Area
                static bool output_auto_scroll = true;
                ImGui::BeginChild("AgentOutput", ImVec2(0, ImGui::GetWindowHeight() * 0.5f), true);
                ImGui::TextWrapped("Assistant: I am ready to help you with your tasks. How can I assist you today?");
                if (output_auto_scroll)
                    ImGui::SetScrollHereY(1.0f);
                ImGui::EndChild();

                // Context Menu Bar
                {
                    if (ImGui::Button("Context")) ImGui::OpenPopup("ContextMenu");
                    ImGui::SameLine();
                    if (ImGui::Button("Presets")) ImGui::OpenPopup("PresetsMenu");
                    ImGui::SameLine();
                    if (ImGui::Button("Copy")) {}

                    // Context Menu Popup
                    if (ImGui::BeginPopup("ContextMenu"))
                    {
                        if (ImGui::MenuItem("Load Context")) {}
                        if (ImGui::MenuItem("Clear Context")) {}
                        if (ImGui::MenuItem("Save Context")) {}
                        ImGui::EndPopup();
                    }

                    // Presets Menu Popup
                    if (ImGui::BeginPopup("PresetsMenu"))
                    {
                        if (ImGui::MenuItem("Code Review")) {}
                        if (ImGui::MenuItem("Bug Fix")) {}
                        if (ImGui::MenuItem("Documentation")) {}
                        ImGui::EndPopup();
                    }
                }

                // User Input Area
                static char input_buffer[4096] = "";
                ImGui::InputTextMultiline("##UserInput", input_buffer, IM_ARRAYSIZE(input_buffer),
                    ImVec2(-1, ImGui::GetWindowHeight() * 0.2f), ImGuiInputTextFlags_AllowTabInput);

                // Input Controls
                {
                    if (ImGui::Button("Model")) ImGui::OpenPopup("ModelMenu");
                    ImGui::SameLine();
                    if (ImGui::Button("Settings")) ImGui::OpenPopup("InputSettings");
                    ImGui::SameLine();
                    if (ImGui::Button("Send")) {}

                    // Model Menu Popup
                    if (ImGui::BeginPopup("ModelMenu"))
                    {
                        if (ImGui::MenuItem("GPT-4")) {}
                        if (ImGui::MenuItem("GPT-3.5")) {}
                        if (ImGui::MenuItem("Claude")) {}
                        ImGui::EndPopup();
                    }
                }

                ImGui::EndTabItem();
            }

            // Agent Settings Tab
            if (ImGui::BeginTabItem("Agent Settings"))
            {
                ImGui::BeginChild("SettingsScroll");

                // Model Settings Section
                if (ImGui::CollapsingHeader("Model Settings", ImGuiTreeNodeFlags_DefaultOpen))
                {
                    static int model_selection = 0;
                    ImGui::Combo("Active Model", &model_selection, "GPT-4\0GPT-3.5\0Claude\0\0");

                    static float temperature = 0.7f;
                    ImGui::SliderFloat("Temperature", &temperature, 0.0f, 1.0f, "%.2f");

                    static int max_tokens = 2048;
                    ImGui::SliderInt("Max Tokens", &max_tokens, 256, 4096);

                    static float presence_penalty = 0.0f;
                    ImGui::SliderFloat("Presence Penalty", &presence_penalty, -2.0f, 2.0f, "%.2f");

                    static float frequency_penalty = 0.0f;
                    ImGui::SliderFloat("Frequency Penalty", &frequency_penalty, -2.0f, 2.0f, "%.2f");
                }

                // Task Management Section
                if (ImGui::CollapsingHeader("Task Management"))
                {
                    static int max_concurrent_tasks = 3;
                    ImGui::InputInt("Max Concurrent Tasks", &max_concurrent_tasks);

                    static bool auto_save_tasks = true;
                    ImGui::Checkbox("Auto-save Tasks", &auto_save_tasks);

                    static char task_save_path[256] = "C:/Tasks/";
                    ImGui::InputText("Task Save Path", task_save_path, IM_ARRAYSIZE(task_save_path));
                }

                // Permissions Section
                if (ImGui::CollapsingHeader("Permissions"))
                {
                    static bool allow_file_access = false;
                    ImGui::Checkbox("Allow File System Access", &allow_file_access);

                    static bool allow_network = true;
                    ImGui::Checkbox("Allow Network Access", &allow_network);

                    static bool allow_system_commands = false;
                    ImGui::Checkbox("Allow System Commands", &allow_system_commands);

                    static int security_level = 1;
                    ImGui::Combo("Security Level", &security_level, "Low\0Medium\0High\0Very High\0\0");
                }

                // Memory Management Section
                if (ImGui::CollapsingHeader("Memory Management"))
                {
                    static float max_memory_gb = 2.0f;
                    ImGui::SliderFloat("Max Memory (GB)", &max_memory_gb, 0.5f, 8.0f, "%.1f");

                    static bool auto_clear_context = true;
                    ImGui::Checkbox("Auto-clear Context", &auto_clear_context);

                    static int context_limit_mb = 512;
                    ImGui::InputInt("Context Memory Limit (MB)", &context_limit_mb);
                }

                // API Settings Section
                if (ImGui::CollapsingHeader("API Settings"))
                {
                    static char api_key[64] = "";
                    ImGui::InputText("API Key", api_key, IM_ARRAYSIZE(api_key), ImGuiInputTextFlags_Password);

                    static char api_endpoint[256] = "https://api.openai.com/v1/";
                    ImGui::InputText("API Endpoint", api_endpoint, IM_ARRAYSIZE(api_endpoint));

                    static float timeout_seconds = 30.0f;
                    ImGui::SliderFloat("Timeout (seconds)", &timeout_seconds, 5.0f, 120.0f, "%.0f");
                }

                ImGui::EndChild();
                ImGui::EndTabItem();
            }

            ImGui::EndTabBar();
        }
    }
    ImGui::End();
}
} // namespace hdImgui
