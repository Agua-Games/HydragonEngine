/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcVegetationPlacementNode.h
 * @brief ProcVegetationPlacementNode represents a procedural vegetation placement node in the engine's node graph.
 */

struct VegetationPlacementInfo : public NodeInfo {
    VegetationPlacementInfo() {
        NodeType = "Vegetation/PlacementGenerator";
        
        Inputs = {
            "DensityMap",        // From DensityGenerator
            "PlacementMask",     // Valid placement areas
            "SpacingRules",      // Min/max spacing
            "RandomSeed",        // For deterministic results
            "VariationParams"    // Species variation
        };
        
        Outputs = {
            "PlacementPoints",   // Generated positions
            "InstanceData",      // Per-instance data
            "ClusterInfo"        // Grouping information
        };
    }
};

} // namespace hd