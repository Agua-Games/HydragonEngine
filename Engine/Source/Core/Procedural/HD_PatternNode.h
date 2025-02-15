/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "Core/NodeGraph/HD_Node.h"

namespace hd {

struct HD_ProceduralPatternInfo : public HD_NodeInfo {
    HD_ProceduralPatternInfo() {
        NodeType = "Procedural/Pattern";
        
        Inputs = {
            "Seed",
            "Scale",
            "Complexity",
            "Evolution",
            "OrchestratorIntent"  // Receives intent from orchestrator
        };
        
        Outputs = {
            "PatternData",
            "PatternMetrics"
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class HD_PatternNode : public HD_Node {
public:
    explicit HD_PatternNode(const HD_ProceduralPatternInfo& info = HD_ProceduralPatternInfo())
        : HD_Node(info) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        patternId = orchestrator.RegisterPattern(CreateDefaultPattern());
    }

    ~HD_PatternNode() {
        if (!patternId.empty()) {
            auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
            orchestrator.UnregisterPattern(patternId);
        }
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
        uint32_t seed = GetInputValue<uint32_t>("Seed");
        float scale = GetInputValue<float>("Scale");
        float complexity = GetInputValue<float>("Complexity");
        float evolution = GetInputValue<float>("Evolution");
        auto intent = GetInputValue<OctaveParams>("OrchestratorIntent");

        ProceduralStructureParams params;
        params.seed = seed;
        params.scale = scale;
        params.complexity = complexity;
        params.evolutionRate = evolution;

        patternId = orchestrator.CreateMaterialPattern(params);
        auto patternData = orchestrator.GetProceduralPattern(patternId);
        
        SetOutputValue("PatternData", patternData);
        SetOutputValue("PatternMetrics", ComputePatternMetrics(patternData));
    }

private:
    std::string patternId;
};

} // namespace hd