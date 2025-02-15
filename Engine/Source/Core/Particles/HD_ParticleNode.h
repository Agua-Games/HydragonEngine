/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Procedural/HD_ProceduralTypes.h"
#include "Core/Procedural/HD_ProceduralOrchestrator.h"

namespace hd {

struct HD_ParticleInfo : public HD_NodeInfo {
    HD_ParticleInfo() {
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

class HD_ParticleNode : public HD_Node {
public:
    explicit HD_ParticleNode(const HD_ParticleInfo& info = HD_ParticleInfo())
        : HD_Node(info) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        particlePatternId = orchestrator.RegisterPattern(CreateDefaultParticlePattern());
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
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