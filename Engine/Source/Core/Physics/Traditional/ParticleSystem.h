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
        nodeType = "Physics/Particles/ParticleSystem";
        
        inputs = {
            "emitter",
            "particleProps",
            "forces",
            "collisions",
            "simSpace",
            "particleLife",
            "colorOverLife",
            "sizeOverLife",
            "gravity",
            "intent",
            "lifeParams",
            "materialParams",
            "proceduralIntent"
        };
        
        outputs = {
            "particleData",
            "simulationState",
            "collisionEvents",   // Collision Event Callbacks with data
            "emitterState",
            "visualData",
            "performanceMetrics"
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

    // Optimization Flags
    bool m_useSimpleSimulation = true;
    bool m_useCollisionEvents = false;
    bool m_useEmitterState = false;
    bool m_useVisualData = false;
    bool m_usePerformanceMetrics = false;

    // State
    EmitterData emitter;
    ParticleProperties particleProps;
    std::vector<ForceField> forces;
    CollisionParams collisions;
    SimulationSpace simSpace;
    float particleLife = 0.0f;
    ColorOverLifetime colorOverLife;
    SizeOverLifetime sizeOverLife;
    glm::vec3 gravity;
    OctaveParams intent;
    LifeParams lifeParams;
    MaterialParams materialParams;

    // === Processing ===
    void processNode() override {
        // Process inputs
        emitter = getInputValue<EmitterData>("emitter");
        particleProps = getInputValue<ParticleProperties>("particleProps");
        forces = getInputValue<std::vector<ForceField>>("forces");
        collisions = getInputValue<CollisionParams>("collisions");
        simSpace = getInputValue<SimulationSpace>("simSpace");
        particleLife = getInputValue<float>("particleLife");
        colorOverLife = getInputValue<ColorOverLifetime>("colorOverLifetime");
        sizeOverLife = getInputValue<SizeOverLifetime>("sizeOverLifetime");
        gravity = getInputValue<glm::vec3>("gravity");
        intent = getInputValue<OctaveParams>("intent");
        lifeParams = getInputValue<LifeParams>("lifeParams");
        materialParams = getInputValue<MaterialParams>("materialParams");
        
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
        params.intent = intent;
        params.lifeParams = lifeParams;
        params.materialParams = materialParams;
        
        // Update particle pattern
        particlePatternId = orchestrator.createParticlePattern(params);
        auto particleData = orchestrator.getProceduralPattern(particlePatternId);
        
        // Update outputs
        setOutputValue("particleData", particleData);
        setOutputValue("simulationState", computeSimulationState(particleData));
        setOutputValue("collisionEvents", processCollisions(particleData));
        setOutputValue("emitterState", updateEmitterState(particleData));
        setOutputValue("visualData", generateVisualData(particleData));
        setOutputValue("performanceMetrics", computePerformanceMetrics());

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