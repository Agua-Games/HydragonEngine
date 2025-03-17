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

#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"
#include "Node.h"

namespace hd {

class ProceduralNodeSystem {
public:
    // Create a self-modifying procedural pattern
    std::unique_ptr<Node> createProceduralPattern() {
        auto pattern = std::make_unique<ProceduralEvolutionNode>();
        
        // Connect to orchestrator
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Setup self-modification capabilities
        pattern->enableSelfModification(true);
        
        // Add procedural modification nodes
        auto modifier = pattern->addChild<SelfModifyingNode>();
        modifier->connectToOrchestrator(&orchestrator);
        
        // Setup validation and safety
        auto validator = pattern->addChild<ValidationNode>();
        validator->setValidationRules(getSafetyRules());
        
        return pattern;
    }

    // Example of safe self-modification
    void evolveProcedualSystem() {
        // Create evolution parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Evolving;
        
        // Generate new pattern with self-modification
        auto pattern = createProceduralPattern();
        pattern->processNodeGraph();
        
        // Validate and apply changes
        if (pattern->validateEvolution()) {
            pattern->applyEvolution();
        }
    }
};

} // namespace hd