/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */

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

class ShortLivedParticleSystem {
public:
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

    void spawnFromImpact(const ImpactEvent& impact) {
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

    void update(float deltaTime) {
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

private:
    float calculateEnergyAtPoint(const ShortLivedParticle& particle, const vec3& point) {
        float distFromOrigin = glm::length(point - particle.origin);
        float particleDistFromOrigin = glm::length(particle.currentPos - particle.origin);
        float normalizedLifetime = 1.0f - particle.lifetime;
        
        return particle.energy * 
               (particleDistFromOrigin - distFromOrigin) * 
               normalizedLifetime * 
               calculateFalloffCurve(distFromOrigin, particle.radius);
    }

    float calculateFalloffCurve(float distance, float radius) {
        // Customizable falloff function (e.g., inverse square, exponential)
        float normalizedDist = glm::clamp(distance / radius, 0.0f, 1.0f);
        return 1.0f - (normalizedDist * normalizedDist);
    }

    void propagateEnergy(const ShortLivedParticle& particle, float radius) {
        // Create energy field contribution
        EnergyField::WaveProperty wave;
        wave.amplitude = particle.energy;
        wave.frequency = 1.0f / particle.lifetime;
        wave.phase = calculatePhaseFromTrajectory(particle);
        
        m_energyField->addWaveContribution(particle.currentPos, radius, wave);
    }

    void spawnOctaves(const ShortLivedParticle& parent) {
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

    std::vector<ShortLivedParticle> m_activeParticles;
    std::shared_ptr<EnergyField> m_energyField;
    Config m_config;
};

} // namespace hd