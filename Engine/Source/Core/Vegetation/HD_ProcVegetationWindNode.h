struct HD_VegetationWindInfo : public HD_NodeInfo {
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