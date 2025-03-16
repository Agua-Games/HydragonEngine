/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NodeGraph_UsageExample_03.cpp
 * @brief This file contains examples of how to setup a node graph in code.
 */
#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"
#include "Core/Particles/ParticleSystem.h"
#include "Core/Materials/Material.h"
#include "Core/Procedural/ProceduralOrchestrator.h"

namespace hd {

class SystemIntegrationExample {
public:
    void setupScene() {
        auto& engine = Engine::GetInstance();
        auto& nodeGraph = engine.GetNodeGraph();
        auto& orchestrator = ProceduralOrchestrator::GetInstance();

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
        auto& engine = Engine::GetInstance();
        
        // Process the node graph
        engine.GetNodeGraph().process();

        // Update procedural systems
        engine.GetProceduralOrchestrator().ProcessNodeGraph();

        // Example of system harmony maintenance
        HarmonyParams harmonyParams;
        harmonyParams.particleComplexity = 0.7f;
        harmonyParams.visualFidelity = 0.8f;
        
        engine.ConfigureHarmonyParameters(harmonyParams);
        engine.ProcessSystems();
    }

    void handleSystemIntent() {
        auto& engine = Engine::GetInstance();
        
        // Set system-wide intent
        SystemIntent intent;
        intent.evolutionTarget = EvolutionTarget::Performance;
        intent.complexityPreference = 0.6f;
        
        engine.SetSystemIntent(intent);

        // Process the changes
        engine.ProcessSystems();
    }
};

// Example of runtime usage
void demonstrateUsage() {
    SystemIntegrationExample example;
    
    // Initial setup
    example.setupScene();

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