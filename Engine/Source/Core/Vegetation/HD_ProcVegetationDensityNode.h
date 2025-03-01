/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_ProcVegetationDensityNode.h
 * @brief ProcVegetationDensityNode represents a procedural vegetation density node in the engine's node graph.
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "HD_Node.h"

namespace hd {

struct HD_VegetationDensityInfo : public HD_NodeInfo {
    HD_VegetationDensityInfo() {
        NodeType = "Vegetation/DensityGenerator";
        
        Inputs = {
            "Terrain",           // Height/normal data
            "BiomeType",         // Biome classification
            "SlopeParams",       // Slope constraints
            "HeightRange",       // Valid height range
            "DensityMask"        // Optional density mask
        };
        
        Outputs = {
            "DensityMap",        // Generated density
            "ValidPlacementMap"  // Binary placement mask
        };
    }
};

} // namespace hd