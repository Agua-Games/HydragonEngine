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
    if (ImGui::Begin("Audio", p_open, ImGuiWindowFlags_MenuBar))
    {
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

        // Toolbar with proper spacing
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(6, 6));
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(6, 3));
        
        ImVec2 buttonSize(32, 32);
        float toolbarHeight = 40;
        
        ImGui::BeginChild("##Toolbar", ImVec2(-1, toolbarHeight), false);
        {
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
            ImGui::Dummy(ImVec2(10, 0));
            ImGui::SameLine();
            
            // Edit Tools
            if (ImGui::Button(ICON_MS_CONTENT_CUT "##Split", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Split Clip");
            ImGui::SameLine();
            
            if (ImGui::Button(ICON_MS_MERGE_TYPE "##Merge", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Merge Clips");
            
            ImGui::SameLine();
            ImGui::Dummy(ImVec2(10, 0));
            ImGui::SameLine();
            
            // Quick Access
            if (ImGui::Button(ICON_MS_TIMELINE "##Montage", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Open in Montage Editor");
            ImGui::SameLine();
            
            if (ImGui::Button(ICON_MS_ACCOUNT_TREE "##NodeGraph", buttonSize)) {}
            if (ImGui::IsItemHovered()) ImGui::SetTooltip("Open in Node Graph");
        }
        ImGui::EndChild();
        
        ImGui::PopStyleVar(2);

        // Main content split view
        ImGui::Columns(2, "AudioMainSplit", true);
        
        // Left panel: Asset/Pattern List and Properties
        {
            // Asset Browser/Pattern List - reduced from 0.4f to 0.35f to give more space to Properties
            ImGui::BeginChild("AssetList", ImVec2(0, ImGui::GetWindowHeight() * 0.35f), true);
            ImGui::Text("Audio Assets & Patterns");
            ImGui::Separator();
            
            // Example asset list
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
            
            // Properties Panel - now gets more vertical space
            ImGui::BeginChild("Properties", ImVec2(0, 0), true);
            ImGui::Text("Properties");
            ImGui::Separator();
            
            // Basic Properties
            ImGui::SliderFloat("Volume", &s_audioState.volume, 0.0f, 1.0f);
            ImGui::SliderFloat("Pitch", &s_audioState.pitch, 0.5f, 2.0f);
            
            // Maestro Integration
            if (ImGui::CollapsingHeader("Maestro Integration", ImGuiTreeNodeFlags_DefaultOpen))
            {
                ImGui::Checkbox("Auto-Harmonize", &s_audioState.autoHarmonize);
                ImGui::SliderFloat("Harmony Influence", &s_audioState.harmonyInfluence, 0.0f, 1.0f);
                ImGui::SliderFloat("Pattern Intensity", &s_audioState.patternIntensity, 0.0f, 1.0f);
            }
            
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
        }
        
        ImGui::NextColumn();
        
        // Right panel: Waveform/Spectrum View and Pattern Controls
        {
            float viewHeight = s_audioState.showPatternControls ? 
                ImGui::GetWindowHeight() * 0.6f : ImGui::GetWindowHeight();
            
            ImGui::BeginChild("WaveformView", ImVec2(0, viewHeight), true);
            if (s_audioState.showWaveform)
            {
                // Waveform controls - Fixed mini toolbar layout
                ImGui::BeginGroup();
                ImVec2 miniButtonSize(24, 24);  // Smaller, consistent button size
                float spacing = 4.0f;           // Consistent spacing
                
                ImGui::Button(ICON_MS_ZOOM_IN "##ZoomIn", miniButtonSize);
                ImGui::SameLine(0.0f, spacing);
                ImGui::Button(ICON_MS_ZOOM_OUT "##ZoomOut", miniButtonSize);
                ImGui::SameLine(0.0f, spacing);
                ImGui::Button(ICON_MS_FIT_SCREEN "##FitView", miniButtonSize);
                ImGui::EndGroup();
                
                ImGui::Separator();
                
                // Placeholder for waveform
                ImGui::GetWindowDrawList()->AddRect(
                    ImGui::GetCursorScreenPos(),
                    ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                           ImGui::GetCursorScreenPos().y + 100),
                    ImGui::GetColorU32(ImGuiCol_Text)
                );
                ImGui::Dummy(ImVec2(0, 100));
            }
            
            if (s_audioState.showSpectrum)
            {
                ImGui::Text("Spectrum Analysis");
                // Spectrum controls - Fixed mini toolbar layout
                ImGui::BeginGroup();
                ImVec2 miniButtonSize(24, 24);  // Smaller, consistent button size
                float spacing = 4.0f;           // Consistent spacing
                
                ImGui::Button(ICON_MS_EQUALIZER "##EQView", miniButtonSize);
                ImGui::SameLine(0.0f, spacing);
                ImGui::Button(ICON_MS_ANALYTICS "##FreqView", miniButtonSize);
                ImGui::EndGroup();
                
                // Placeholder for spectrum
                ImGui::GetWindowDrawList()->AddRect(
                    ImGui::GetCursorScreenPos(),
                    ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                           ImGui::GetCursorScreenPos().y + 100),
                    ImGui::GetColorU32(ImGuiCol_Text)
                );
                ImGui::Dummy(ImVec2(0, 100));
            }
            ImGui::EndChild();
            
            // Pattern Controls
            if (s_audioState.showPatternControls)
            {
                ImGui::BeginChild("PatternControls", ImVec2(0, 0), true);
                ImGui::Text("Pattern Controls");
                ImGui::Separator();
                
                // Pattern presets with more compact layout
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x * 0.7f);
                if (ImGui::BeginCombo("Pattern Preset", "Default"))
                {
                    ImGui::Selectable("Ambient Nature");
                    ImGui::Selectable("Urban Environment");
                    ImGui::Selectable("Combat Intensity");
                    ImGui::EndCombo();
                }
                
                // Pattern visualization placeholder
                ImGui::GetWindowDrawList()->AddRect(
                    ImGui::GetCursorScreenPos(),
                    ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetContentRegionAvail().x,
                           ImGui::GetCursorScreenPos().y + 80),
                    ImGui::GetColorU32(ImGuiCol_Text)
                );
                ImGui::Dummy(ImVec2(0, 80));
                
                ImGui::EndChild();
            }
        }
        
        ImGui::Columns(1);
    }
    ImGui::End();
}

} // namespace hdImgui
