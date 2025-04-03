/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MeshEditor.h
 * @brief The Mesh Editor is a specialized view for mesh-based nodes.
 */
#pragma once
#include <imgui.h>
#include <string>
#include "MainEditorUIManager.h"

namespace hd {

struct MeshToolSettings {
    // Decimation settings
    float targetReduction = 0.5f;
    bool preserveUVs = true;
    bool preserveNormals = true;
    
    // Remeshing settings
    float voxelSize = 0.1f;
    int smoothIterations = 2;
    bool adaptiveSize = true;
    
    // Boolean operations
    enum class BooleanMode {
        Union,
        Difference,
        Intersection,
        Slice
    } booleanMode = BooleanMode::Union;
    
    // Procedural settings
    struct {
        float erosionStrength = 1.0f;
        float gravityInfluence = 1.0f;
        float decayRate = 0.5f;
        int fractureSeed = 42;
        int fracturePieces = 10;
    } proceduralParams;
    
    // UV tools
    struct {
        bool autoSeams = true;
        float angleThreshold = 45.0f;
        float packingMargin = 0.01f;
        bool preserveAspectRatio = true;
    } uvParams;
};

void showMeshEditor(bool* p_open, EditorWindowData* windowData);
void ShowMeshToolsPanel(MeshToolSettings& settings);
void ShowUVPanel(MeshToolSettings& settings);
void ShowProceduralPanel(MeshToolSettings& settings);
void ShowUSDPanel();

} // namespace hd
