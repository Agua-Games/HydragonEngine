/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "Core/NodeGraph/HD_Node.h"

namespace hd {

struct HD_EvolutionInfo : public HD_NodeInfo {
    HD_EvolutionInfo() {
        NodeType = "Procedural/Evolution";
        
        Inputs = {
            "SourcePattern",
            "TimeScale",
            "EvolutionRules",
            "Constraints"
        };
        
        Outputs = {
            "EvolvedPattern",
            "EvolutionState",
            "TransitionMetrics"
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class HD_ProceduralEvolutionNode : public HD_Node {
public:
    explicit HD_ProceduralEvolutionNode(const HD_EvolutionInfo& info = HD_EvolutionInfo())
        : HD_Node(info) {}

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
        auto sourcePattern = GetInputValue<ProceduralPatternData>("SourcePattern");
        float timeScale = GetInputValue<float>("TimeScale");
        auto rules = GetInputValue<OctaveParams>("EvolutionRules");
        auto constraints = GetInputValue<HarmonyParams>("Constraints");

        ProceduralStructureParams evolutionParams;
        evolutionParams.type = ProceduralStructureType::Evolving;
        evolutionParams.sourcePattern = sourcePattern;
        evolutionParams.timeScale = timeScale;
        evolutionParams.evolutionRules = rules;
        evolutionParams.constraints = constraints;

        std::string evolvedId = orchestrator.CreateMaterialPattern(evolutionParams);
        auto evolvedPattern = orchestrator.GetProceduralPattern(evolvedId);
        
        SetOutputValue("EvolvedPattern", evolvedPattern);
        SetOutputValue("EvolutionState", GetEvolutionState());
        SetOutputValue("TransitionMetrics", CalculateTransitionMetrics());
    }
};

} // namespace hd