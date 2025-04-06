/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ShortLivedParticleSystem.cpp
 * @brief Implementation file for the ShortLivedParticleSystem class.
 * 
 * ARCHITECTURAL NOTES:
 * - ShortLivedParticleSystem is a class that represents a short-lived particle in Hydragon. It's used to simulate short-lived energy transfers, like impacts, explosions, thermal
 * transfer, kinetic energy transfers to the vicinity, etc.
 */
#include "ShortLivedParticleSystem.h"
#include "PhysicsTypes.h"

// === Allocation, Initialization, Loading ===
ShortLivedParticleSystem::ShortLivedParticleSystem(const ShortLivedParticleSystemInfo& info) : Node(info) {}
void ShortLivedParticleSystem::initialize() {}
void ShortLivedParticleSystem::load() {}

// === Processing ===
void ShortLivedParticleSystem::processNode() {
    // Process particle updates
    // Handle collisions and energy transfer
    // Remove expired particles
}

ShortLivedParticle& ShortLivedParticleSystem::createParticle(const vec3& position, float initialEnergy) {
    ShortLivedParticle particle;
    particle.origin = position;
    particle.currentPos = position;
    particle.energy = initialEnergy;
    particle.lifetime = 0.0f;
    particle.radius = 0.0f;
    particle.flags.hasCollided = false;
    particle.flags.isArched = false;
    particle.flags.energyTier = 0;
    particle.flags.octaveGen = 0;
    particle.pathType = Path::LINEAR;
    
    m_activeParticles.push_back(particle);
    return m_activeParticles.back();
}

void ShortLivedParticleSystem::spawnFromImpact(const ImpactEvent& impact) {
    float totalEnergy = impact.energy;
    vec3 normal = impact.normal;
    
    // Spawn primary particle
    auto& particle = createParticle(impact.position, totalEnergy);
    
    // Calculate initial trajectory
    setupTrajectory(particle, normal, impact.material);
    
    // Optionally spawn octaves
    if (m_config.optimization.enableMultisampling) {
        spawnOctaves(particle);
    }
}

void ShortLivedParticleSystem::update(float deltaTime) {
    for (auto& particle : m_activeParticles) {
        // Update position and energy
        updateParticle(particle, deltaTime);
        
        // Calculate influence sphere
        float sphereRadius = calculateInfluenceSphere(particle);
        
        // Transfer energy to surrounding space
        if (particle.energy > m_config.minEnergyThreshold) {
            propagateEnergy(particle, sphereRadius);
        }
        
        // Handle collisions and energy transfer
        handleCollisions(particle);
    }
    
    // Remove dead particles
    removeExpiredParticles();
}

float ShortLivedParticleSystem::calculateEnergyAtPoint(const ShortLivedParticle& particle, const vec3& point) {
    float distFromOrigin = glm::length(point - particle.origin);
    float particleDistFromOrigin = glm::length(particle.currentPos - particle.origin);
    float normalizedLifetime = 1.0f - particle.lifetime;
    
    return particle.energy * 
           (particleDistFromOrigin - distFromOrigin) * 
           normalizedLifetime * 
           calculateFalloffCurve(distFromOrigin, particle.radius);
}

float ShortLivedParticleSystem::calculateFalloffCurve(float distance, float radius) {
    // Customizable falloff function (e.g., inverse square, exponential)
    float normalizedDist = glm::clamp(distance / radius, 0.0f, 1.0f);
    return 1.0f - (normalizedDist * normalizedDist);
}

void ShortLivedParticleSystem::propagateEnergy(const ShortLivedParticle& particle, float radius) {
    // Create energy field contribution
    EnergyField::WaveProperty wave;
    wave.amplitude = particle.energy;
    wave.frequency = 1.0f / particle.lifetime;
    wave.phase = calculatePhaseFromTrajectory(particle);
    
    m_energyField->addWaveContribution(particle.currentPos, radius, wave);
}

void ShortLivedParticleSystem::spawnOctaves(const ShortLivedParticle& parent) {
    if (parent.flags.octaveGen >= m_config.maxOctaves) return;
    
    // Spawn child particles with varied trajectories
    for (int i = 0; i < 3; ++i) {
        auto& child = createParticle(parent.currentPos, 
            parent.energy * m_config.optimization.octaveEnergyScale);
        child.flags.octaveGen = parent.flags.octaveGen + 1;
        
        // Vary trajectory from parent
        vec3 variation = generateVariation(parent.velocity);
        setupTrajectory(child, variation, parent.pathType);
    }
}

// === Cleanup ===
void ShortLivedParticleSystem::unload() {}
void ShortLivedParticleSystem::cleanup() {}
ShortLivedParticleSystem::~ShortLivedParticleSystem() = default;