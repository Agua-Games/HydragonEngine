/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcParticleForces.h
 * @brief ProcParticleForces represents a procedural particle forces node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - ProcParticleForces nodes are used to apply forces to particles procedurally.
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

namespace hd {

struct ParticleForcesInfo : public NodeInfo {
    ParticleForcesInfo() {
        NodeType = "Particles/Forces";
        
        inputs = {
            "Particles",        // From EmitterNode
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

} // namespace hd