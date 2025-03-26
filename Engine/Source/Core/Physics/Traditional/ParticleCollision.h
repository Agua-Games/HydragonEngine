/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ParticleCollision.h
 * @brief ParticleCollision represents a traditional particle collision node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - ParticleCollision nodes are used to simulate particle collisions traditionally.
 * - We try to support traditional particle collisions by hooking them with WavePhysics, through proper conversions to waves, EnergyTransfer (=spawn) events, etc.
 * 
 * TODO:
 * - Create .cpp file and move the implementation there.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 */

#include "Node.h"

namespace hd {

struct ParticleCollisionInfo : public NodeInfo {
    ParticleCollisionInfo() {
        NodeType = "Physics/Particles/ParticleCollision";
        
        inputs = {
            "Particles",        // From ParticleSystem
            "CollisionMesh",    // Scene geometry
            "Response",         // Collision response
            "Friction",         // Surface friction
            "Damping",          // Energy loss
            "SubSteps"          // Simulation quality
        };
        
        outputs = {
            "FinalParticles",   // Post-collision state
            "Impacts",          // Collision events
            "Fragments",        // Break-up particles
            "SurfaceEffects"    // Surface interaction
        };
    }
};

class ParticleCollision : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ParticleCollision(const ParticleCollisionInfo& info = ParticleCollisionInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ParticleCollision() = default;     // Default destructor
};

} // namespace hd