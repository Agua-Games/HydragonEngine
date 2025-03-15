/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ParticleNode.h
 * @brief ParticleNode represents a particle system node in the engine's node graph.
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

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class ParticleNode : public Node {
public:
    explicit ParticleNode(const ParticleInfo& info = ParticleInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        particlePatternId = orchestrator.RegisterPattern(CreateDefaultParticlePattern());
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        
        // Process inputs
        auto emitter = GetInputValue<EmitterData>("Emitter");
        auto particleProps = GetInputValue<ParticleProperties>("ParticleProps");
        auto forces = GetInputValue<std::vector<ForceField>>("Forces");
        auto collisions = GetInputValue<CollisionParams>("Collisions");
        auto intent = GetInputValue<OctaveParams>("ProceduralIntent");
        
        // Create particle pattern parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Particle;
        params.emitterData = emitter;
        params.particleProps = particleProps;
        params.forceFields = forces;
        params.collisionParams = collisions;
        
        // Update particle pattern
        particlePatternId = orchestrator.CreateParticlePattern(params);
        auto particleData = orchestrator.GetProceduralPattern(particlePatternId);
        
        // Update outputs
        SetOutputValue("ParticleData", particleData);
        SetOutputValue("SimulationState", ComputeSimulationState(particleData));
        SetOutputValue("CollisionEvents", ProcessCollisions(particleData));
        SetOutputValue("EmitterState", UpdateEmitterState(particleData));
        SetOutputValue("VisualData", GenerateVisualData(particleData));
        SetOutputValue("PerformanceMetrics", ComputePerformanceMetrics());
    }

private:
    std::string particlePatternId;
};

} // namespace hd