/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PatternNode.h
 * @brief PatternNode represents a procedural pattern node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Pattern nodes are used to generate procedural patterns for various purposes.
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

struct ProceduralPatternInfo : public NodeInfo {
    ProceduralPatternInfo() {
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

class PatternNode : public Node {
public:
    explicit PatternNode(const ProceduralPatternInfo& info = ProceduralPatternInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        patternId = orchestrator.RegisterPattern(CreateDefaultPattern());
    }

    ~PatternNode() {
        if (!patternId.empty()) {
            auto& orchestrator = ProceduralOrchestrator::GetInstance();
            orchestrator.UnregisterPattern(patternId);
        }
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        
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