struct HD_VegetationPlacementInfo : public HD_NodeInfo {
    HD_VegetationPlacementInfo() {
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