struct HD_PlacementInfo : public HD_NodeInfo {
    HD_PlacementInfo() {
        NodeType = "Procedural/Placement";
        
        Inputs = {
            "DensityMap",        // Density field (2D/3D)
            "PlacementMask",     // Valid placement areas
            "Bounds",            // Placement volume/area
            "SpacingRules",      // Min/max spacing
            "Orientation",       // Orientation rules
            "Scale",             // Scale rules
            "RandomSeed",        // For deterministic results
            "CustomAttributes"   // Additional per-instance data
        };
        
        Outputs = {
            "Points",            // Generated positions
            "Rotations",         // Orientation per point
            "Scales",            // Scale per point
            "InstanceData",      // Generic instance data
            "ClusterInfo",       // Grouping information
            "Metrics"            // Placement statistics
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class HD_ProcPlacementNode : public HD_Node {
public:
    explicit HD_ProcPlacementNode(const HD_PlacementInfo& info = HD_PlacementInfo())
        : HD_Node(info) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        placementPatternId = orchestrator.RegisterPattern(CreateDefaultPlacementPattern());
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
        // Get inputs
        auto densityMap = GetInputValue<DensityField>("DensityMap");
        auto mask = GetInputValue<PlacementMask>("PlacementMask");
        auto bounds = GetInputValue<BoundingVolume>("Bounds");
        auto spacing = GetInputValue<SpacingRules>("SpacingRules");
        auto orientRules = GetInputValue<OrientationRules>("Orientation");
        auto scaleRules = GetInputValue<ScaleRules>("Scale");
        auto seed = GetInputValue<uint32_t>("RandomSeed");
        
        // Create placement parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Placement;
        params.densityField = densityMap;
        params.mask = mask;
        params.bounds = bounds;
        params.spacing = spacing;
        params.orientation = orientRules;
        params.scale = scaleRules;
        params.seed = seed;
        
        // Generate placement pattern
        placementPatternId = orchestrator.CreatePlacementPattern(params);
        auto placementData = orchestrator.GetProceduralPattern(placementPatternId);
        
        // Set outputs
        SetOutputValue("Points", ExtractPoints(placementData));
        SetOutputValue("Rotations", ExtractRotations(placementData));
        SetOutputValue("Scales", ExtractScales(placementData));
        SetOutputValue("InstanceData", ExtractInstanceData(placementData));
        SetOutputValue("ClusterInfo", ExtractClusterInfo(placementData));
        SetOutputValue("Metrics", ComputeMetrics(placementData));
    }

private:
    std::string placementPatternId;
};