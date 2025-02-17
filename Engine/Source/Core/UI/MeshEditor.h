/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>
#include <string>
#include "hdImgui.h"

namespace hdImgui {

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

void ShowMeshEditor(bool* p_open, HdEditorWindowData* windowData);
void ShowMeshToolsPanel(MeshToolSettings& settings);
void ShowUVPanel(MeshToolSettings& settings);
void ShowProceduralPanel(MeshToolSettings& settings);
void ShowUSDPanel();

} // namespace hdImgui
