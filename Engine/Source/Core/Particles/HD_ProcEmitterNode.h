/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_ProcEmitterNode.h
 * @brief ProcEmitterNode represents a procedural emitter node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - ProcEmitter nodes are used to emit particles procedurally based on density fields and rules.
 * 
 * TODO:
 * - Update the whole content to match the latest HD_Object and HD_Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */
#pragma once

namespace hd {

struct HD_EmitterInfo : public HD_NodeInfo {
    HD_EmitterInfo() {
        NodeType = "Particles/Emitter";
        
        Inputs = {
            "Shape",            // Emission shape
            "Rate",            // Emission rate
            "InitialState",    // Initial conditions
            "Variation",       // Property variation
            "Lifetime",        // Particle lifetime
            "Seed"            // Random seed
        };
        
        Outputs = {
            "ParticleStream",   // Emitted particles
            "EmissionEvents",   // Emission timing
            "SystemState",      // Emitter state
            "Statistics"        // Performance stats
        };
    }
};

} // namespace hd