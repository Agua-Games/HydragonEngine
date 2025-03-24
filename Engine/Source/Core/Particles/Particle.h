/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Particle.h
 * @brief Particle represents a particle system node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Particle nodes are used to simulate and render particle systems.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */
#pragma once

#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"

namespace hd {

struct ParticleInfo : public NodeInfo {
    ParticleInfo() {
        NodeType = "Particles/ParticleSystem";
        
        Inputs = {
            "Emitter",           // Emitter properties
            "ParticleProps",     // Particle properties
            "Forces",            // External forces
            "Collisions",        // Collision parameters
            "LifetimeParams",    // Lifetime control
            "MaterialParams",    // Particle material
            "ProceduralIntent",  // For procedural variation
            "SimSpace"           // World/Local space
        };
        
        Outputs = {
            "ParticleData",      // Current particle states
            "SimulationState",   // Simulation data
            "CollisionEvents",   // Collision feedback
            "EmitterState",      // Emitter status
            "VisualData",        // Rendering data
            "PerformanceMetrics"
        };

        isSerializable = true;
        isEditableInEditor = true;
        isProcedural = true;
    }
};

class Particle : public Node {
public:
    explicit Particle(const ParticleInfo& info = ParticleInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        particlePatternId = orchestrator.registerPattern(createDefaultParticlePattern());
    }

    void processNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Process inputs
        auto emitter = getInputValue<EmitterData>("Emitter");
        auto particleProps = getInputValue<ParticleProperties>("ParticleProps");
        auto forces = getInputValue<std::vector<ForceField>>("Forces");
        auto collisions = getInputValue<CollisionParams>("Collisions");
        auto intent = getInputValue<OctaveParams>("ProceduralIntent");
        
        // Create particle pattern parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Particle;
        params.emitterData = emitter;
        params.particleProps = particleProps;
        params.forceFields = forces;
        params.collisionParams = collisions;
        
        // Update particle pattern
        particlePatternId = orchestrator.createParticlePattern(params);
        auto particleData = orchestrator.getProceduralPattern(particlePatternId);
        
        // Update outputs
        setOutputValue("ParticleData", particleData);
        setOutputValue("SimulationState", computeSimulationState(particleData));
        setOutputValue("CollisionEvents", processCollisions(particleData));
        setOutputValue("EmitterState", updateEmitterState(particleData));
        setOutputValue("VisualData", generateVisualData(particleData));
        setOutputValue("PerformanceMetrics", computePerformanceMetrics());
    }

private:
    std::string particlePatternId;
};

} // namespace hd