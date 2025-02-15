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