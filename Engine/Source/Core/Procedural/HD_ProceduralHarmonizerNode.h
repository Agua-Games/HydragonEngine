/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_ProceduralHarmonizerNode.h
 * @brief HarmonizerNode represents a harmonizer node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Harmonizer nodes are used to harmonize patterns with each other.
 * 
 * TODO:
 * - Update the whole content to match the latest HD_Object and HD_Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "Core/NodeGraph/HD_Node.h"

namespace hd {

struct HD_HarmonizerInfo : public HD_NodeInfo {
    HD_HarmonizerInfo() {
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

class HD_ProceduralHarmonizerNode : public HD_Node {
public:
    explicit HD_ProceduralHarmonizerNode(const HD_HarmonizerInfo& info = HD_HarmonizerInfo())
        : HD_Node(info) {}

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
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