/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ParticleSystem.h
 * @brief ParticleSystem represents a traditional particle system node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - ParticleSystem nodes are used to simulate and render traditional particle systems.
 * - We try to support traditional particle systems by hooking them with WavePhysics, through proper conversions to waves, EnergyTransfer (=spawn) events, etc.
 * - It supports traditional particle systems, like fire, smoke, snow, etc.
 * - It supports procedural particle systems, like flocking, swarming, etc.
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
        NodeType = "Physics/Particles/ParticleSystem";
        
        inputs = {
            "Emitter",
            "ParticleProps",
            "Forces",
            "Collisions",
            "SimSpace",
            "ParticleLife",
            "ColorOverLife",
            "SizeOverLife",
            "Gravity",
            "LifeParams",
            "MaterialParams",
            "ProceduralIntent"
        };
        
        outputs = {
            "ParticleData",
            "SimulationState",
            "CollisionEvents",   // Collision Event Callbacks with data
            "EmitterState",
            "VisualData",
            "PerformanceMetrics"
        };

        isSerializable = true;
        isEditableInEditor = true;
        isProcedural = true;
    }
};

/**
 * @class ParticleSystem
 * @brief Represents a traditional particle system node in the engine's node graph.
 */
class ParticleSystem : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ParticleSystem(const ParticleInfo& info = ParticleInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        particlePatternId = orchestrator.registerPattern(createDefaultParticlePattern());
    }
    initialize() override {}
    load() override {}

    // === Processing ===
    void () override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Process inputs
        auto emitter = getInputValue<EmitterData>("Emitter");
        auto particleProps = getInputValue<ParticleProperties>("ParticleProps");
        auto forces = getInputValue<std::vector<ForceField>>("Forces");
        auto collisions = getInputValue<CollisionParams>("Collisions");
        auto simSpace = getInputValue<SimulationSpace>("SimSpace");
        auto particleLife = getInputValue<float>("ParticleLife");
        auto colorOverLife = getInputValue<ColorOverLifetime>("ColorOverLifetime");
        auto sizeOverLife = getInputValue<SizeOverLifetime>("SizeOverLifetime");
        auto gravity = getInputValue<glm::vec3>("Gravity");
        auto intent = getInputValue<OctaveParams>("ProceduralIntent");
        auto lifeParams = getInputValue<LifeParams>("LifeParams");
        auto materialParams = getInputValue<MaterialParams>("MaterialParams");
        
        // Create particle pattern parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::ParticleSystem;
        params.emitterData = emitter;
        params.particleProps = particleProps;
        params.forceFields = forces;
        params.collisionParams = collisions;
        params.simulationSpace = simSpace;
        params.particleLife = particleLife;
        params.colorOverLife = colorOverLife;
        params.sizeOverLife = sizeOverLife;
        params.gravity = gravity;
        params.lifeParams = lifeParams;
        params.materialParams = materialParams;
        
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

        orchestrator.setIntent(intent);
        orchestrator.();

        // === Cleanup ===
        void unload() override {}
        void cleanup() override {}
        ~ParticleSystem() = default;     // Default destructor
    }

private:
    std::string particlePatternId;
};

} // namespace hd