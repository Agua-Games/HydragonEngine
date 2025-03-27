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
#include <string>
#include "Node.h"
#include "ProceduralTypes.h"

namespace hd {

struct ParticleEmitterInfo : public NodeInfo {
    ParticleEmitterInfo() {
        nodeType = "Physics/Particles/ParticleEmitter";
        
        inputs = {
            "shape",           // Emission shape
            "direction",       // Emission direction
            "position",        // Emission position
            "radius",          // Used for circular and spherical emitters
            "rate",            // Emission rate
            "maxParticles",    // Maximum concurrent particles
            "initialVelocity", // Initial velocity
            "initialState",    // Initial conditions
            "variation",       // Property variation
            "lifetime",        // Particle lifetime
            "seed",            // Random seed
            "emitterParams"    // Emitter parameters
        };
        
        outputs = {
            "particleStream",   // Emitted particles
            "emissionEvents",   // Emission timing
            "systemState",      // Emitter state
            "statistics"        // Performance stats
        };
    }
};

/**
 * @class ParticleEmitter
 * @brief Represents a traditional (though procedural) particle emitter node in the engine's node graph.
 */
class ParticleEmitter : public Node {
public:
    // === Struct Definitions ===
    struct EmitterParams {
        std::string id;             // Unique identifier
        //ProceduralShape shape;    // Emission shape               // Uncomment after defining ProceduralShape
        float radius;               // Emission radius
        float seed;                 // Random seed
        float rate;                 // Emission rate
        float maxParticles;         // Maximum concurrent particles
        vec3 initialVelocity;       // Initial velocity
        float lifetime;             // Particle lifetime
        float size;                 // Particle size
        float speed;                // Emission speed
        vec3 direction;             // Emission direction
        vec3 position;              // Emission position
        // ... other parameters
    };

    // === Allocation, Initialization, Loading ===
    explicit ParticleEmitter(const ParticleEmitterInfo& info = ParticleEmitterInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    std::unordered_map<std::string, EmitterParams> emitters;

    // === Processing ===
    void activate(const std::string& emitterId);
    void deactivate(const std::string& emitterId);
    void addEmitter(const EmitterParams& params);
    void removeEmitter(const std::string& emitterId);
    void updateEmitters();
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ParticleEmitter() = default;     // Default destructor
};

} // namespace hd