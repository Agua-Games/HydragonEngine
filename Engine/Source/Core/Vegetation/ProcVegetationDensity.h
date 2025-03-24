/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcVegetationDensity.h
 * @brief ProcVegetationDensity represents a procedural vegetation density node in the engine's node graph.
 */
#pragma once

#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"
#include "Node.h"

namespace hd {

struct VegetationDensityInfo : public NodeInfo {
    VegetationDensityInfo() {
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