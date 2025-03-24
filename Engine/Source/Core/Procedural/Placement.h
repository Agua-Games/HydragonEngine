/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Placement.h
 * @brief Placement represents a procedural placement node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Placement nodes are used to place objects procedurally based on density fields and rules.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once
#include <string>
#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"

namespace hd {

struct PlacementInfo : public NodeInfo {
    PlacementInfo() {
        NodeType = "Procedural/Placement";
        
        inputs = {
            "DensityMap",        // Density field (2D/3D)
            "PlacementMask",     // Valid placement areas
            "Bounds",            // Placement volume/area
            "SpacingRules",      // Min/max spacing
            "Orientation",       // Orientation rules
            "Scale",             // Scale rules
            "RandomSeed",        // For deterministic results
            "CustomAttributes"   // Additional per-instance data
        };
        
        outputs = {
            "Points",            // Generated positions
            "Rotations",         // Orientation per point
            "Scales",            // Scale per point
            "InstanceData",      // Generic instance data
            "ClusterInfo",       // Grouping information
            "Metrics"            // Placement statistics
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class Placement : public Node {
public:
    explicit Placement(const PlacementInfo& info = PlacementInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        placementPatternId = orchestrator.registerPattern(createDefaultPlacementPattern());
    }

    void processNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Get inputs
        auto densityMap = getInputValue<DensityField>("DensityMap");
        auto mask = getInputValue<PlacementMask>("PlacementMask");
        auto bounds = getInputValue<BoundingVolume>("Bounds");
        auto spacing = getInputValue<SpacingRules>("SpacingRules");
        auto orientRules = getInputValue<OrientationRules>("Orientation");
        auto scaleRules = getInputValue<ScaleRules>("Scale");
        auto seed = getInputValue<uint32_t>("RandomSeed");
        
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
        placementPatternId = orchestrator.createPlacementPattern(params);
        auto placementData = orchestrator.getProceduralPattern(placementPatternId);
        
        // Set outputs
        setOutputValue("Points", extractPoints(placementData));
        setOutputValue("Rotations", extractRotations(placementData));
        setOutputValue("Scales", extractScales(placementData));
        setOutputValue("InstanceData", extractInstanceData(placementData));
        setOutputValue("ClusterInfo", extractClusterInfo(placementData));
        setOutputValue("Metrics", computeMetrics(placementData));
    }

private:
    std::string placementPatternId;
};

} // namespace hd