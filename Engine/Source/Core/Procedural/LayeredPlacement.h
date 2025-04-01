/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file LayeredPlacement.h
 * @brief Header file for the LayeredPlacement class.
 * 
 * ARCHITECTURAL NOTES:
 * - LayeredPlacement is a class that represents a layered placement in Hydragon.
 * - It is used to place objects in layers, with each layer having its own density field and rules.
 * - It supports various placement types, such as random, grid, and pattern.
 * - It also supports different object types, such as particles, meshes, and sprites.
 * - It can be used to create various effects, such as layers of snow, leaves, and water.
 */
#pragma once
#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"

namespace hd {

struct LayeredPlacementInfo : public NodeInfo {
    LayeredPlacementInfo() {
        NodeType = "Procedural/LayeredPlacement";
        
        inputs = {
            "Layers",           // Array of layer data
            "PlacementType",    // Random, grid, pattern, etc.
            "ObjectType",       // Particle, mesh, sprite, etc.
            "Bounds",           // Placement volume/area
            "RandomSeed",       // For deterministic results
            "CustomAttributes"   // Additional per-instance data
        };
        
        outputs = {
            "Points",           // Generated positions
            "Rotations",        // Orientation per point
            "Scales",           // Scale per point
            "InstanceData",     // Generic instance data
            "ClusterInfo",      // Grouping information
            "Metrics"           // Placement statistics
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class LayeredPlacement : public Node {
public:
    explicit LayeredPlacement(const LayeredPlacementInfo& info = LayeredPlacementInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        placementPatternId = orchestrator.registerPattern(createDefaultPlacementPattern());
    }

    ~LayeredPlacement() {
        if (!placementPatternId.empty()) {
            auto& orchestrator = ProceduralOrchestrator::getInstance();
            orchestrator.unregisterPattern(placementPatternId);
        }
    }

    void processNode() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        auto layers = getInputValue<std::vector<LayerData>>("Layers");
        auto placementType = getInputValue<PlacementType>("PlacementType");
        auto objectType = getInputValue<ObjectType>("ObjectType");
        auto bounds = getInputValue<BoundingVolume>("Bounds");
        auto seed = getInputValue<uint32_t>("RandomSeed");
        
        // Create placement parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Placement;
        params.layers = layers;
        params.placementType = placementType;
        params.objectType = objectType;
        params.bounds = bounds;
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
