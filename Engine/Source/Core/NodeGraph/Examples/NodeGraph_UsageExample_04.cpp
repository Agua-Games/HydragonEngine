/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - System integration example
 *  - Procedural particle system setup
 *  - Procedural material generation
 *  - Procedural system harmony maintenance
 *  - System intent handling
 */
#if 0

#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"
#include "Core/Particles/ParticleSystem.h"
#include "Core/Materials/Material.h"
#include "Core/Procedural/ProceduralOrchestrator.h"

namespace hd {

class SystemIntegrationExample_01 {
public:
    void setup() {
        auto& engine = Engine::getInstance();
        auto& nodeGraph = engine.getNodeGraph();
        auto& orchestrator = ProceduralOrchestrator::getInstance();

        // Create a procedural particle system
        auto particleSystem = std::make_shared<ParticleSystem>();
        particleSystem->setMaxParticles(1000)
            .addEmitter<SphereEmitter>()
            .addModifier<GravityModifier>();

        // Create a procedurally generated material
        auto material = std::make_shared<Material>();
        material->setProperty("baseColor", Color(1.0f, 0.5f, 0.2f))
                .setProperty("roughness", 0.5f)
                .setProperty("metallic", 0.0f);

        // Connect nodes in the graph
        nodeGraph.connect(particleSystem, "VisualData", material, "ParticleInput");

        // Setup procedural evolution
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Particle;
        params.evolutionRate = 0.5f;

        orchestrator.registerPattern(particleSystem, params);
    }

    void update(float deltaTime) {
        auto& engine = Engine::getInstance();
        
        // Process the node graph
        engine.getNodeGraph().processNode();

        // Update procedural systems
        engine.getProceduralOrchestrator().();

        // Example of system harmony maintenance
        HarmonyParams harmonyParams;
        harmonyParams.particleComplexity = 0.7f;
        harmonyParams.visualFidelity = 0.8f;
        
        engine.configureHarmonyParameters(harmonyParams);
        engine.processSystems();
    }

    void handleSystemIntent() {
        auto& engine = Engine::getInstance();
        
        // Set system-wide intent
        SystemIntent intent;
        intent.evolutionTarget = EvolutionTarget::Performance;
        intent.complexityPreference = 0.6f;
        
        engine.setSystemIntent(intent);

        // Process the changes
        engine.processSystems();
    }
};

// Example of runtime usage
void demonstrateUsage() {
    SystemIntegrationExample example;
    
    // Initial setup
    example.setup();

    // Main loop
    float deltaTime = 1.0f / 60.0f;
    while (true) {
        example.update(deltaTime);
        
        // Handle dynamic system changes
        if (needsEvolution()) {
            example.handleSystemIntent();
        }
    }
}

} // namespace hd

#endif