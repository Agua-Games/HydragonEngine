/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ParticleEmitter.h
 * @brief ParticleEmitter represents a procedural emitter node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - ParticleEmitter nodes are used to emit traditional particles procedurally based on density fields and rules.
 * - It has strong built-in procedural support, and can be used to create complex traditional particle systems.
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

struct EmitterInfo : public NodeInfo {
    EmitterInfo() {
        NodeType = "Particles/Emitter";
        
        inputs = {
            "Shape",            // Emission shape
            "Rate",            // Emission rate
            "InitialState",    // Initial conditions
            "Variation",       // Property variation
            "Lifetime",        // Particle lifetime
            "Seed"            // Random seed
        };
        
        outputs = {
            "ParticleStream",   // Emitted particles
            "EmissionEvents",   // Emission timing
            "SystemState",      // Emitter state
            "Statistics"        // Performance stats
        };
    }
};

} // namespace hd