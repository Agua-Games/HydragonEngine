/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_ProcVegetationGrowthNode.h
 * @brief ProcVegetationGrowthNode represents a procedural vegetation growth node in the engine's node graph.
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "Node.h"

namespace hd {

struct HD_VegetationGrowthInfo : public NodeInfo {
    HD_VegetationGrowthInfo() {
        NodeType = "Vegetation/GrowthSimulator";
        
        Inputs = {
            "PlacementPoints",   // From PlacementGenerator
            "Climate",           // Climate influence
            "Nutrients",         // Soil quality
            "Competition",       // Resource competition
            "TimeParams",        // Growth timeline
            "SeasonalCycle"      // Seasonal influence
        };
        
        Outputs = {
            "GrowthStates",      // Current growth state
            "ResourceMap",       // Resource distribution
            "HealthMetrics",     // Plant health data
            "AgeData"           // Age distribution
        };
    }
};

} // namespace hd