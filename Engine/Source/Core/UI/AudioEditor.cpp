/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#include <imgui.h>
#include <string>
#include "AudioEditor.h"
#include "hdImgui.h"
#include "IconsMaterialSymbols.h"

namespace hdImgui {

struct AudioEditorState {
    // Current audio asset/pattern being edited
    std::string currentAudioName;
    bool isModified = false;
    
    // UI States
    bool showWaveform = true;
    bool showSpectrum = true;
    bool showPatternControls = true;
    float volume = 1.0f;
    float pitch = 1.0f;
    
    // Maestro Integration
    float harmonyInfluence = 0.5f;
    float patternIntensity = 0.5f;
    bool autoHarmonize = true;
    
    // Procedural Settings
    struct {
        float density = 0.5f;
        float variation = 0.3f;
        float coherence = 0.7f;
        bool usePhysicsInput = true;
        bool useParticleVectors = true;
        float velocityThreshold = 5.0f;
        float collisionThreshold = 10.0f;
    } proceduralParams;
};

static AudioEditorState s_audioState;

void ShowAudioEditor(bool* p_open, HdEditorWindowData* windowData) 
{
    ImGui::SetNextWindowBgAlpha(windowData->globalWindowBgAlpha);
    
    // Remove window padding
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    
    if (ImGui::Begin("Audio", p_open, ImGuiWindowFlags_MenuBar))
    {
        // Style for borderless menu bar
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(10, 4));
        ImGui::PushStyleColor(ImGuiCol_MenuBarBg, ImVec4(0, 0, 0, 0));
        
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem(ICON_MS_ADD " Import Audio")) {}
                if (ImGui::MenuItem(ICON_MS_AUDIOTRACK " New Audio Pattern")) {}
                ImGui::Separator();
                if (ImGui::MenuItem(ICON_MS_SAVE " Save")) {}
                if (ImGui::MenuItem(ICON_MS_UPLOAD " Export")) {}
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("View"))
            {
                ImGui::MenuItem("Waveform", nullptr, &s_audioState.showWaveform);
                ImGui::MenuItem("Spectrum", nullptr, &s_audioState.showSpectrum);
                ImGui::MenuItem("Pattern Controls", nullptr, &s_audioState.showPatternControls);
                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }
        
        ImGui::PopStyleColor();  // MenuBarBg
        ImGui::PopStyleVar();    // FramePadding

        // Add padding only for the content area below toolbar
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(8, 8));
        
        // Toolbar styling - Match TopToolbar's approach
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
        
        const float toolbarHeight = 30.0f;
        ImVec2 buttonSize = windowData->iconDefaultSize;
        
        // Create toolbar child window
        ImGui::BeginChild("AudioEditorToolbar", ImVec2(-1, toolbarHeight), true, 
            ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);
        
        // Set cursor position to window origin
        ImGui::SetCursorScreenPos(ImGui::GetWindowPos());
        
        // Style for buttons
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);
        ImVec4 buttonColor = ImGui::GetStyleColorVec4(ImGuiCol_Button);
        ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(buttonColor.x, buttonColor.y, buttonColor.z, 0.3f));
        
        // Transport Controls
        if (ImGui::Button(ICON_MS_PLAY_ARROW "##Play", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Play/Preview");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_STOP "##Stop", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Stop");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_LOOP "##Loop", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Loop Selection");
        ImGui::SameLine();
        
        ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();
        
        // Edit Tools
        if (ImGui::Button(ICON_MS_CONTENT_CUT "##Split", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Split Clip");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_MERGE_TYPE "##Merge", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Merge Clips");
        ImGui::SameLine();
        
        ImGui::Dummy(ImVec2(5, 0)); ImGui::SameLine();
        
        // Quick Access
        if (ImGui::Button(ICON_MS_TIMELINE "##Montage", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Open in Montage Editor");
        ImGui::SameLine();
        
        if (ImGui::Button(ICON_MS_ACCOUNT_TREE "##NodeGraph", buttonSize)) {}
        if (ImGui::IsItemHovered()) ImGui::SetTooltip("Open in Node Graph");
        
        ImGui::PopStyleColor();
        ImGui::PopStyleVar(4); // Pop all style vars
        
        ImGui::EndChild();
        
        // Main content split view
        ImGui::Columns(2, "AudioMainSplit", true);
        
        // Left panel: Asset/Pattern List only
        {
            ImGui::BeginChild("AssetList", ImVec2(0, -1), true);
            ImGui::Text("Audio Assets & Patterns");
            ImGui::Separator();
            
            // Asset Browser/Pattern List
            if (ImGui::TreeNode("Sound Effects"))
            {
                ImGui::Selectable("footsteps_wood.wav");
                ImGui::Selectable("impact_metal_01.wav");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Music"))
            {
                ImGui::Selectable("battle_theme_01.ogg");
                ImGui::Selectable("ambient_forest.ogg");
                ImGui::TreePop();
            }
            if (ImGui::TreeNode("Patterns"))
            {
                ImGui::Selectable("wind_variation_01");
                ImGui::Selectable("water_ambient");
                ImGui::TreePop();
            }
            ImGui::EndChild();
        }
        
        ImGui::NextColumn();
        
        // Right panel: Tabbed interface for Properties and Audio Views
        {
            if (ImGui::BeginTabBar("AudioEditorTabs"))
            {
                // Properties Tab
                if (ImGui::BeginTabItem("Properties"))
                {
                    ImGui::BeginChild("PropertiesView", ImVec2(0, -1), true);
                    
                    // Basic Properties
                    ImGui::Text("Basic Properties");
                    ImGui::Separator();
                    ImGui::SliderFloat("Volume", &s_audioState.volume, 0.0f, 1.0f);
                    ImGui::SliderFloat("Pitch", &s_audioState.pitch, 0.5f, 2.0f);
                    
                    ImGui::Spacing();
                    ImGui::Spacing();
                    
                    // Maestro Integration
                    if (ImGui::CollapsingHeader("Maestro Integration", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::Checkbox("Auto-Harmonize", &s_audioState.autoHarmonize);
                        ImGui::SliderFloat("Harmony Influence", &s_audioState.harmonyInfluence, 0.0f, 1.0f);
                        ImGui::SliderFloat("Pattern Intensity", &s_audioState.patternIntensity, 0.0f, 1.0f);
                    }
                    
                    ImGui::Spacing();
                    
                    // Procedural Settings
                    if (ImGui::CollapsingHeader("Procedural Generation", ImGuiTreeNodeFlags_DefaultOpen))
                    {
                        ImGui::SliderFloat("Density", &s_audioState.proceduralParams.density, 0.0f, 1.0f);
                        ImGui::SliderFloat("Variation", &s_audioState.proceduralParams.variation, 0.0f, 1.0f);
                        ImGui::SliderFloat("Coherence", &s_audioState.proceduralParams.coherence, 0.0f, 1.0f);
                        ImGui::Separator();
                        ImGui::Checkbox("Use Physics Input", &s_audioState.proceduralParams.usePhysicsInput);
                        ImGui::Checkbox("Use Particle Vectors", &s_audioState.proceduralParams.useParticleVectors);
                        ImGui::SliderFloat("Velocity Threshold", &s_audioState.proceduralParams.velocityThreshold, 0.0f, 20.0f);
                        ImGui::SliderFloat("Collision Threshold", &s_audioState.proceduralParams.collisionThreshold, 0.0f, 20.0f);
                    }
                    
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
                
                // Audio View Tab
                if (ImGui::BeginTabItem("Audio View"))
                {
                    ImGui::BeginChild("AudioView", ImVec2(0, -1), true);
                    
                    // Waveform Section
                    if (s_audioState.showWaveform)
                    {
                        ImGui::Text("Waveform");
                        ImGui::Separator();
                        
                        // Waveform controls - Fixed mini toolbar layout
                        ImGui::BeginGroup();
                        ImVec2 miniButtonSize(24, 24);
                        float spacing = 4.0f;
                        
                        ImGui::Button(ICON_MS_ZOOM_IN "##ZoomIn", miniButtonSize);
                        ImGui::SameLine(0.0f, spacing);
                        ImGui::Button(ICON_MS_ZOOM_OUT "##ZoomOut", miniButtonSize);
                        ImGui::SameLine(0.0f, spacing);
                        ImGui::Button(ICON_MS_FIT_SCREEN "##FitView", miniButtonSize);
                        ImGui::EndGroup();
                        
                        // Waveform visualization
                        ImGui::GetWindowDrawList()->AddRect(
                            ImGui::GetCursorScreenPos(),
                            ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                                   ImGui::GetCursorScreenPos().y + 150),
                            ImGui::GetColorU32(ImGuiCol_Text)
                        );
                        ImGui::Dummy(ImVec2(0, 150));
                    }
                    
                    ImGui::Spacing();
                    ImGui::Spacing();
                    
                    // Spectrum Section
                    if (s_audioState.showSpectrum)
                    {
                        ImGui::Text("Spectrum Analysis");
                        ImGui::Separator();
                        
                        // Spectrum controls
                        ImGui::BeginGroup();
                        ImVec2 miniButtonSize(24, 24);
                        float spacing = 4.0f;
                        
                        ImGui::Button(ICON_MS_EQUALIZER "##EQView", miniButtonSize);
                        ImGui::SameLine(0.0f, spacing);
                        ImGui::Button(ICON_MS_ANALYTICS "##FreqView", miniButtonSize);
                        ImGui::EndGroup();
                        
                        // Spectrum visualization
                        ImGui::GetWindowDrawList()->AddRect(
                            ImGui::GetCursorScreenPos(),
                            ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                                   ImGui::GetCursorScreenPos().y + 150),
                            ImGui::GetColorU32(ImGuiCol_Text)
                        );
                        ImGui::Dummy(ImVec2(0, 150));
                    }
                    
                    ImGui::EndChild();
                    ImGui::EndTabItem();
                }
                
                ImGui::EndTabBar();
            }
        }
        
        ImGui::Columns(1);
    }
    ImGui::End();
    
    ImGui::PopStyleVar(); // Window padding
}

} // namespace hdImgui
