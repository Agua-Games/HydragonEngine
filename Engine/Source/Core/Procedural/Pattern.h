/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Pattern.h
 * @brief Pattern represents a procedural pattern node in the engine's node graph.
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
        
        inputs = {
            "Seed",
            "Scale",
            "Complexity",
            "Evolution",
            "OrchestratorIntent"  // Receives intent from orchestrator
        };
        
        outputs = {
            "PatternData",
            "PatternMetrics"
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class Pattern : public Node {
public:
    explicit Pattern(const ProceduralPatternInfo& info = ProceduralPatternInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        patternId = orchestrator.registerPattern(createDefaultPattern());
    }

    ~Pattern() {
        if (!patternId.empty()) {
            auto& orchestrator = ProceduralOrchestrator::getInstance();
            orchestrator.unregisterPattern(patternId);
        }
    }

    void processNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        uint32_t seed = getInputValue<uint32_t>("Seed");
        float scale = getInputValue<float>("Scale");
        float complexity = getInputValue<float>("Complexity");
        float evolution = getInputValue<float>("Evolution");
        auto intent = getInputValue<OctaveParams>("OrchestratorIntent");

        ProceduralStructureParams params;
        params.seed = seed;
        params.scale = scale;
        params.complexity = complexity;
        params.evolutionRate = evolution;

        patternId = orchestrator.createMaterialPattern(params);
        auto patternData = orchestrator.getProceduralPattern(patternId);
        
        setOutputValue("PatternData", patternData);
        setOutputValue("PatternMetrics", computePatternMetrics(patternData));
    }

private:
    std::string patternId;
};

} // namespace hd