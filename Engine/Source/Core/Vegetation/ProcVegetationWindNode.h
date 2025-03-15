/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_ProcVegetationWindNode.h
 * @brief ProcVegetationWindNode represents a procedural vegetation wind node in the engine's node graph.
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "Node.h"

namespace hd {

struct HD_VegetationWindInfo : public NodeInfo {
    HD_VegetationWindInfo() {
        NodeType = "Vegetation/WindResponse";
        
        Inputs = {
            "InstanceData",      // From PlacementGenerator
            "GrowthStates",      // From GrowthSimulator
            "WindDirection",     // Wind vector
            "WindStrength",      // Wind force
            "Turbulence",        // Turbulence params
            "Stiffness"         // Material properties
        };
        
        Outputs = {
            "BendingData",       // Vertex deformation
            "AnimationState",    // Current animation
            "StressFactors",     // Physical stress
            "LODParams"         // Level of detail data
        };
    }
};

} // namespace hd