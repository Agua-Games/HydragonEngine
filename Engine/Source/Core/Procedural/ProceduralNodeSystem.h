/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralNodeSystem.h
 * @brief ProceduralNodeSystem is a class that manages the procedural node system.
 * 
 * ARCHITECTURAL NOTES:
 * - The end goal of this structure is to allow, among other things, the engine to self-replicate,
 *   self-modify, procedurally generate and modify nodes themselves.
 * 
 * TODO:
 */
#pragma once

#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "Node.h"

namespace hd {

class ProceduralNodeSystem {
public:
    // Create a self-modifying procedural pattern
    std::unique_ptr<Node> CreateProceduralPattern() {
        auto pattern = std::make_unique<HD_ProceduralEvolutionNode>();
        
        // Connect to orchestrator
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
        // Setup self-modification capabilities
        pattern->EnableSelfModification(true);
        
        // Add procedural modification nodes
        auto modifier = pattern->AddChild<HD_SelfModifyingNode>();
        modifier->ConnectToOrchestrator(&orchestrator);
        
        // Setup validation and safety
        auto validator = pattern->AddChild<HD_ValidationNode>();
        validator->SetValidationRules(GetSafetyRules());
        
        return pattern;
    }

    // Example of safe self-modification
    void EvolveProcedualSystem() {
        // Create evolution parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Evolving;
        
        // Generate new pattern with self-modification
        auto pattern = CreateProceduralPattern();
        pattern->ProcessNodeGraph();
        
        // Validate and apply changes
        if (pattern->ValidateEvolution()) {
            pattern->ApplyEvolution();
        }
    }
};

} // namespace hd