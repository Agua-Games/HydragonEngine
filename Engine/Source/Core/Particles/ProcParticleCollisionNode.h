/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcParticleCollisionNode.h
 * @brief ProcParticleCollisionNode represents a procedural particle collision node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - ProcParticleCollision nodes are used to simulate particle collisions procedurally.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */

namespace hd {

struct ParticleCollisionInfo : public NodeInfo {
    ParticleCollisionInfo() {
        NodeType = "Particles/Collision";
        
        Inputs = {
            "Particles",        // From ForcesNode
            "CollisionMesh",    // Scene geometry
            "Response",         // Collision response
            "Friction",         // Surface friction
            "Damping",         // Energy loss
            "SubSteps"         // Simulation quality
        };
        
        Outputs = {
            "FinalParticles",   // Post-collision state
            "Impacts",          // Collision events
            "Fragments",        // Break-up particles
            "SurfaceEffects"    // Surface interaction
        };
    }
};

} // namespace hd