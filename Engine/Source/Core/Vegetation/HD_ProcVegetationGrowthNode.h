struct HD_VegetationGrowthInfo : public HD_NodeInfo {
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