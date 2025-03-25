/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ParticleForces.h
 * @brief ParticleForces represents a traditional particle forces node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - ParticleForces nodes are used to apply forces to particles traditionally.
 * - It has strong built-in procedural support, and can be used to create complex traditional particle systems.
 * - We try to support traditional particle forces by hooking them with WavePhysics, through proper conversions to waves, EnergyTransfer (=spawn) events, etc.
 * 
 * TODO:
 * - Create .cpp file and move the implementation there.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct ParticleForcesInfo : public NodeInfo {
    ParticleForcesInfo() {
        NodeType = "Physics/Particles/ParticleForces";
        
        inputs = {
            "Particles",        // From ParticleSystem
            "Gravity",         // Gravity force
            "Wind",            // Wind force
            "Turbulence",      // Turbulent forces
            "Attractors",      // Point attractors
            "Fields"           // Force fields
        };
        
        outputs = {
            "ModifiedParticles", // Updated particles
            "ForceMetrics",      // Force magnitudes
            "EnergyState",       // System energy
            "Trajectories"       // Motion paths
        };
    }
};

class ParticleForces : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ParticleForces(const ParticleForcesInfo& info = ParticleForcesInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNodeGraph() override {
        update();
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ParticleForces() = default;     // Default destructor
};

} // namespace hd