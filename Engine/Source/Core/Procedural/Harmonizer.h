/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Harmonizer.h
 * @brief Harmonizer represents a harmonizer node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Harmonizer nodes are used to harmonize patterns with each other.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once

#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"
#include "Core/NodeGraph/Node.h"

namespace hd {

struct HarmonizerInfo : public NodeInfo {
    HarmonizerInfo() {
        NodeType = "Procedural/Harmonizer";
        
        inputs = {
            "PatternA",
            "PatternB",
            "BlendFactor",
            "HarmonyRules"
        };
        
        outputs = {
            "HarmonizedPattern",
            "HarmonyMetrics"
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class Harmonizer : public Node {
public:
    explicit Harmonizer(const HarmonizerInfo& info = HarmonizerInfo())
        : Node(info) {}

    void () override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        auto patternA = getInputValue<ProceduralPatternData>("PatternA");
        auto patternB = getInputValue<ProceduralPatternData>("PatternB");
        float blend = getInputValue<float>("BlendFactor");
        auto rules = getInputValue<HarmonyParams>("HarmonyRules");

        ProceduralStructureParams harmonizedParams;
        harmonizedParams.type = ProceduralStructureType::harmonized;
        harmonizedParams.sourcePatterns = {patternA, patternB};
        harmonizedParams.blendFactor = blend;
        harmonizedParams.harmonyRules = rules;

        std::string harmonizedId = orchestrator.createMaterialPattern(harmonizedParams);
        auto harmonizedPattern = orchestrator.getProceduralPattern(harmonizedId);
        
        setOutputValue("harmonizedPattern", harmonizedPattern);
        setOutputValue("HarmonyMetrics", calculateHarmonyMetrics(harmonizedPattern));
    }
};

} // namespace hd