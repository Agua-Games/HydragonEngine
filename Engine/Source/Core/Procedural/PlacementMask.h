/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PlacementMask.h
 * @brief Header file for the PlacementMask class.
 * 
 * ARCHITECTURAL NOTES:
 * - PlacementMask is a class that represents a placement mask in Hydragon.
 * - It is used to define valid placement areas for objects in the game world.
 * - It supports various types of masks, such as 2D and 3D masks.
 * - It supports different procedural mask types, such as density, height, and slope masks.
 * - It can be used to create various effects, such as blocking placement in certain areas, or creating placement patterns.
 */
#pragma once
#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"

namespace hd {

struct PlacementMaskInfo : public NodeInfo {
    PlacementMaskInfo() {
        NodeType = "Procedural/PlacementMask";
        
        inputs = {
            "MaskType",         // 2D, 3D, etc.
            "ProceduralMask",   // Density, height, slope, etc.
            "Bounds",           // Mask volume/area
            "RandomSeed",       // For deterministic results
            "CustomAttributes"   // Additional mask data
        };
        
        outputs = {
            "MaskData",         // Generated mask data
            "MaskMetrics"       // Mask performance metrics
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class PlacementMask : public Node {
public:
    explicit PlacementMask(const PlacementMaskInfo& info = PlacementMaskInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        maskPatternId = orchestrator.registerPattern(createDefaultMaskPattern());
    }

    ~PlacementMask() {
        if (!maskPatternId.empty()) {
            auto& orchestrator = ProceduralOrchestrator::getInstance();
            orchestrator.unregisterPattern(maskPatternId);
        }
    }

    void processNode() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        auto maskType = getInputValue<MaskType>("MaskType");
        auto proceduralMask = getInputValue<ProceduralMask>("ProceduralMask");
        auto bounds = getInputValue<BoundingVolume>("Bounds");
        auto seed = getInputValue<uint32_t>("RandomSeed");
        
        // Create mask parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Mask;
        params.maskType = maskType;
        params.proceduralMask = proceduralMask;
        params.bounds = bounds;
        params.seed = seed;
        
        // Generate mask pattern
        maskPatternId = orchestrator.createMaskPattern(params);
        auto maskData = orchestrator.getProceduralPattern(maskPatternId);
        
        // Set outputs
        setOutputValue("MaskData", maskData);
        setOutputValue("MaskMetrics", computeMaskMetrics(maskData));
    }

private:
    std::string maskPatternId;
};

} // namespace hd
