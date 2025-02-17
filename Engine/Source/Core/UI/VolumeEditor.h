/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <imgui.h>
#include <string>
#include "hdImgui.h"

namespace hdImgui {

struct VolumeToolSettings {
    // Volume import/conversion settings
    float voxelSize = 0.1f;
    bool preserveDetails = true;
    float densityThreshold = 0.5f;
    
    // Mesh to Volume settings
    struct {
        float bandwidthSize = 3.0f;
        bool fillInterior = true;
        float surfaceThickness = 1.0f;
    } meshToVolume;
    
    // Point Cloud settings
    struct {
        float particleRadius = 0.1f;
        float smoothingRadius = 0.2f;
        int smoothingIterations = 2;
        bool adaptiveRadius = true;
    } pointCloud;
    
    // Procedural settings
    struct {
        float noiseScale = 1.0f;
        float noiseStrength = 1.0f;
        int noiseSeed = 42;
        float erosionRate = 0.5f;
        int flowIterations = 5;
    } proceduralParams;
    
    // Volume operations
    enum class OperationType {
        Union,
        Difference,
        Intersection,
        Smooth,
        Sharpen,
        Erode
    } operation = OperationType::Union;
};

void ShowVolumeEditor(bool* p_open, HdEditorWindowData* windowData);
void ShowVolumeToolsPanel(VolumeToolSettings& settings);
void ShowPointCloudPanel(VolumeToolSettings& settings);
void ShowProceduralPanel(VolumeToolSettings& settings);
void ShowVDBPanel();

} // namespace hdImgui
