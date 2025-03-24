/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */
#if 0
#include "Core/Engine.h"
#include "ParticleSystemNode.h"
#include "ProceduralOrchestrator.h"
#include "ProceduralTypes.h"
#include "ProceduralEvolutionNode.h"
#include "ProceduralNode.h"
#include "Pattern.h"
#include "ProceduralStructureNode.h"
#include "Noise.h"

namespace hd {

class ParticleEvolutionExample_01 {
public:
    void evolveParticleSystem() {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Create base particle system
        auto particleSystem = std::make_shared<ParticleSystem>();
        
        // Setup evolution parameters
        OctaveParams evolutionParams;
        evolutionParams.complexity = 0.7f;
        evolutionParams.coherence = 0.8f;
        evolutionParams.variation = 0.5f;

        // Register for procedural evolution
        auto pattern = orchestrator.registerPattern(particleSystem);
        
        // evolve the system
        pattern->setIntent("VisualComplexity", evolutionParams);
        
        // Process evolution
        orchestrator.processNodeGraph();
        
        // The particle system will now evolve based on:
        // 1. System harmony requirements
        // 2. Performance constraints
        // 3. Visual complexity targets
        // 4. Global evolution parameters
    }

    void handleEvolutionFeedback() {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Get evolution metrics
        auto metrics = orchestrator.GetOutputValue<EvolutionMetrics>("EvolutionMetrics");
        
        // Adjust based on feedback
        if (metrics.complexityScore > 0.8f) {
            // Reduce complexity
            OctaveParams newParams;
            newParams.complexity = 0.6f;
            orchestrator.modulateSystem(SystemDomain::Particles, newParams);
        }
    }
};

} // namespace hd

#endif