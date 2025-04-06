/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ShortLivedParticleSystem.h
 * @brief Header file for the ShortLivedParticleSystem class.
 * 
 * ARCHITECTURAL NOTES:
 * - ShortLivedParticleSystem is a class that represents a short-lived particle in Hydragon. It's used to simulate short-lived energy transfers, like impacts, explosions, thermal
 * transfer, kinetic energy transfers to the vicinity, etc.
 * 
 * @todo Update the whole content to match the latest WavePhysics design.
 * @todo Move the structs which should be moved to PhysicsTypes.h.
 */
#pragma once
#include "Node.h"
#include "PhysicsTypes.h"

namespace hd {

struct ShortLivedParticle {
    vec3 origin;           // Spawn/impact point
    vec3 currentPos;       // Current position
    vec3 velocity;         // Direction and speed
    float energy;          // Current energy level
    float lifetime;        // 0.0 to 1.0
    float radius;          // Current influence sphere radius
    
    // Packed into 32 bits for optimization
    struct Flags {
        uint8_t hasCollided : 1;    // Hit another surface
        uint8_t isArched : 1;       // Non-linear trajectory
        uint8_t energyTier : 2;     // Energy magnitude category
        uint8_t octaveGen : 2;      // Generation number for multi-sampling
        uint8_t reserved : 2;
    } flags;

    // Trajectory types
    enum class Path : uint8_t {
        LINEAR,
        BALLISTIC,
        SPIRAL,
        FIELD_GUIDED
    } pathType;
};

struct ImpactEvent {
    vec3 position;
    vec3 normal;
    float energy;
    PhysicsMaterial material;
};

struct ShortLivedParticleSystemInfo : public NodeInfo {
    ShortLivedParticleSystemInfo() {
        NodeType = "Physics/ShortLivedParticleSystem";

        inputs = {
            "ImpactEvents",  // Impact events
            "EnergyField",   // Energy field
            "Config"         // System configuration
        };
        
        outputs = {
            "ParticleData",  // Particle data
            "EnergyField",   // Updated energy field
            "CollisionEvents"// Collision events
        };
    }
};

class ShortLivedParticleSystem : public Node {
public:
    // === Structure Definitions ===
    struct Config {
        float baseLifetime = 0.5f;          // Base lifetime in seconds
        float energyTransferEfficiency = 0.8f;
        float minEnergyThreshold = 0.01f;   // Minimum energy to stay alive
        uint32_t maxParticles = 1024;       // Maximum concurrent particles
        uint32_t maxOctaves = 3;            // Maximum generation splits
        
        struct Optimization {
            bool useSparseSampling = true;
            bool enableMultisampling = false;
            float octaveEnergyScale = 0.5f;  // Energy scale per generation
            float cullingRadius = 50.0f;     // Distance-based culling
        } optimization;
    };

    // === Allocation, Initialization, Loading ===
    explicit ShortLivedParticleSystem(const ShortLivedParticleSystemInfo& info = ShortLivedParticleSystemInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override;
    ShortLivedParticle& createParticle(const vec3& position, float initialEnergy);
    void spawnFromImpact(const ImpactEvent& impact);

    void update(float deltaTime);

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ShortLivedParticleSystem() = default;     // Default destructor

private:
    // === Structure Definitions ===
    std::vector<ShortLivedParticle> m_activeParticles;
    std::shared_ptr<EnergyField> m_energyField;
    Config m_config;

    // === Processing ===
    float calculateEnergyAtPoint(const ShortLivedParticle& particle, const vec3& point);
    float calculateFalloffCurve(float distance, float radius);                          // Maybe rename to calculateDissipationCurve() instead
    void propagateEnergy(const ShortLivedParticle& particle, float radius);
    void spawnOctaves(const ShortLivedParticle& parent);
};

} // namespace hd