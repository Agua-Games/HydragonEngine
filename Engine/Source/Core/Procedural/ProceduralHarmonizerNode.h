/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralHarmonizerNode.h
 * @brief HarmonizerNode represents a harmonizer node in the engine's node graph.
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
        
        Inputs = {
            "PatternA",
            "PatternB",
            "BlendFactor",
            "HarmonyRules"
        };
        
        Outputs = {
            "HarmonizedPattern",
            "HarmonyMetrics"
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class ProceduralHarmonizerNode : public Node {
public:
    explicit ProceduralHarmonizerNode(const HarmonizerInfo& info = HarmonizerInfo())
        : Node(info) {}

    void ProcessNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        
        auto patternA = GetInputValue<ProceduralPatternData>("PatternA");
        auto patternB = GetInputValue<ProceduralPatternData>("PatternB");
        float blend = GetInputValue<float>("BlendFactor");
        auto rules = GetInputValue<HarmonyParams>("HarmonyRules");

        ProceduralStructureParams harmonizedParams;
        harmonizedParams.type = ProceduralStructureType::Harmonized;
        harmonizedParams.sourcePatterns = {patternA, patternB};
        harmonizedParams.blendFactor = blend;
        harmonizedParams.harmonyRules = rules;

        std::string harmonizedId = orchestrator.CreateMaterialPattern(harmonizedParams);
        auto harmonizedPattern = orchestrator.GetProceduralPattern(harmonizedId);
        
        SetOutputValue("HarmonizedPattern", harmonizedPattern);
        SetOutputValue("HarmonyMetrics", CalculateHarmonyMetrics(harmonizedPattern));
    }
};

} // namespace hd