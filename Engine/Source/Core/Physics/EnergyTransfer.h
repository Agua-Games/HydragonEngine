namespace hd {

// Energy manifestation types and thresholds
struct EnergyManifestationType {
    enum class Type : uint8_t {
        KINETIC,        // Motion, velocity
        THERMAL,        // Heat, temperature
        DEFORMATION,    // Shape changes
        PARTICLE,       // Break-up, debris
        WAVE,          // Shock waves, ripples
        FIELD          // Electromagnetic, force fields
    };

    // Energy bands based on frequency ranges
    struct FrequencyBand {
        static constexpr float kRadioWaves     = 1e3f;    // Low energy EM
        static constexpr float kInfrared       = 1e12f;   // Thermal radiation
        static constexpr float kVisible        = 1e15f;   // Visible light
        static constexpr float kUltraviolet    = 1e16f;   // Starting to be harmful
        static constexpr float kXRay          = 1e18f;   // Ionizing radiation
        static constexpr float kGammaRay      = 1e20f;   // Highly penetrating
        static constexpr float kCosmicRay     = 1e24f;   // Ultra-high energy
    };

    // Interaction thresholds
    struct InteractionThreshold {
        static constexpr float kMolecularBond  = 1e-19f;  // Can break molecular bonds
        static constexpr float kIonization     = 1e-17f;  // Can ionize atoms
        static constexpr float kNuclearEffect  = 1e-13f;  // Can affect nuclear structure
        static constexpr float kParticleDecay  = 1e-10f;  // Can induce particle decay
    };
};

struct EnergyTransferNode {
    // Progressive quantization levels
    struct Precision {
        uint8_t highEnergy : 8;    // Full precision for critical areas
        uint8_t mediumEnergy : 6;  // Reduced for medium intensity
        uint8_t lowEnergy : 4;     // Minimal for background effects
    };

    // Energy transfer rules
    struct TransferRule {
        EnergyManifestationType::Type sourceType;
        EnergyManifestationType::Type targetType;
        float threshold;
        float conversionEfficiency;
        
        // Automatic reaction spawning
        struct Reaction {
            std::string effectTemplate;  // "explosion", "debris", "spark"
            float intensityScale;        // Scale effect by energy magnitude
            uint32_t particleCount;      // Dynamic particle count
            float lifetime;              // Effect duration
        } reaction;
    };
};

class EnergyField {
public:
    struct WaveProperty {
        float amplitude;
        float frequency;
        float phase;
        vec3 propagationDirection;
        
        // Wave characteristics
        struct {
            float wavelength;
            float attenuation;
            float dispersion;
            float penetration;      // Material penetration depth
            float ionizationRate;   // Rate of ionization effects
        } medium;
        
        // Effect properties based on frequency
        struct Effects {
            bool canIonize;
            bool canInduceMutation;
            bool canTriggerDecay;
            float chainReactionProbability;
        } effects;

        // Compute interaction effects based on frequency
        Effects calculateEffects() const {
            Effects e;
            e.canIonize = frequency >= FrequencyBand::kUltraviolet;
            e.canInduceMutation = frequency >= FrequencyBand::kXRay;
            e.canTriggerDecay = frequency >= FrequencyBand::kGammaRay;
            e.chainReactionProbability = 
                frequency >= FrequencyBand::kCosmicRay ? 
                    (frequency / FrequencyBand::kCosmicRay) : 0.0f;
            return e;
        }

        // Get penetration depth based on frequency
        float calculatePenetrationDepth(float materialDensity) const {
            // Higher frequencies penetrate deeper
            float basePenetration = std::log10(frequency) * 0.1f;
            return basePenetration / materialDensity;
        }
        
        // Dynamic precision based on amplitude
        uint32_t getBitDepth() const {
            if (amplitude > EnergyManifestationType::kHighEnergyThreshold) return 32;
            if (amplitude > EnergyManifestationType::kMediumEnergyThreshold) return 16;
            return 8;
        }
    };

    // Energy transfer and transformation
    void transferEnergy(const vec3& position, float amount, 
                       EnergyManifestationType::Type sourceType,
                       EnergyManifestationType::Type targetType);
                       
    // Automatic reaction spawning
    void spawnReactions(const vec3& position, float energy);

    // Propagate wave from impact
    void propagateWave(const vec3& origin, const vec3& direction, float initialEnergy) {
        WaveProperty wave;
        wave.amplitude = initialEnergy;
        wave.propagationDirection = direction;
        wave.frequency = calculateInitialFrequency(initialEnergy);
        wave.effects = wave.calculateEffects();
        
        // Create wavefront tracker
        auto wavefront = m_particleSystem.createParticle(origin, initialEnergy);
        wavefront.pathType = ShortLivedParticle::Path::FIELD_GUIDED;
        
        // Consider frequency-dependent effects
        if (wave.effects.canTriggerDecay) {
            initializeDecayChain(wave);
        }
        
        // Register wave contribution with penetration effects
        float radius = calculateInfluenceRadius(initialEnergy);
        addWaveContribution(origin, radius, wave);
    }

    // Compute superposition at point
    float computeFieldStrength(const vec3& point) {
        float totalEnergy = 0.0f;
        
        for (const auto& wave : m_activeWaves) {
            // Consider phase and attenuation
            float distance = glm::length(point - wave.origin);
            float attenuation = calculateAttenuation(distance, wave.medium);
            float phaseShift = distance * wave.frequency;
            
            totalEnergy += wave.amplitude * attenuation * 
                          std::cos(phaseShift + wave.phase);
        }
        
        return totalEnergy;
    }

    // Handle wave interactions
    void processWaveInteractions() {
        for (const auto& wave : m_activeWaves) {
            // Check for constructive/destructive interference
            processInterference(wave);
            
            // Handle medium transitions
            processMediumBoundaries(wave);
            
            // Update wave properties based on field conditions
            updateWaveProperties(wave);
        }
    }

private:
    float calculateAttenuation(float distance, const WaveProperty::Medium& medium) {
        return std::exp(-distance * medium.attenuation);
    }

    void processInterference(const WaveProperty& wave) {
        // Handle wave superposition and interference patterns
        // This affects energy distribution in the field
    }

    void processMediumBoundaries(const WaveProperty& wave) {
        // Handle reflection, refraction, and transmission
        // at medium boundaries
    }

    std::vector<WaveProperty> m_activeWaves;
    ShortLivedParticleSystem m_particleSystem;  // Used for wavefront tracking

    void initializeDecayChain(const WaveProperty& wave) {
        if (Random::Float() < wave.effects.chainReactionProbability) {
            // Spawn secondary radiation waves
            spawnSecondaryWaves(wave);
        }
    }

    void spawnSecondaryWaves(const WaveProperty& primary) {
        // Simulate particle decay or nuclear reactions
        // producing secondary radiation
    }
};

// Integration with ShortLivedParticleSystem
class WaveTrackingSystem {
public:
    void updateWavePropagation(float deltaTime) {
        for (auto& particle : m_particleSystem.getActiveParticles()) {
            // Update wave properties
            auto& wave = m_energyField.getWaveAt(particle.currentPos);
            wave.amplitude *= calculateEnergyDissipation(particle, deltaTime);
            
            // Update propagation
            vec3 newDirection = calculateWaveDirection(particle, m_energyField);
            particle.velocity = newDirection * getWaveSpeed(wave);
            
            // Generate field effects
            if (particle.energy > m_config.minEnergyThreshold) {
                m_energyField.propagateWave(
                    particle.currentPos,
                    particle.velocity,
                    particle.energy
                );
            }
        }
    }

private:
    float getWaveSpeed(const WaveProperty& wave) {
        return wave.frequency * wave.medium.wavelength;
    }

    vec3 calculateWaveDirection(const ShortLivedParticle& wavefront, 
                              const EnergyField& field) {
        // Consider field gradients and medium properties
        vec3 baseDirection = wavefront.velocity;
        vec3 fieldInfluence = field.getGradientAt(wavefront.currentPos);
        
        return glm::normalize(baseDirection + fieldInfluence * m_config.fieldInfluence);
    }

    ShortLivedParticleSystem m_particleSystem;
    EnergyField m_energyField;
    Config m_config;
};

// Energy as a dimensional field
class EnergyDimension {
public:
    struct ExcitationField {
        // 4D noise field (3D space + energy dimension)
        float sampleField(const vec3& position, float energyLevel) const {
            // Warp the sampling based on energy level
            vec3 warpedPos = position + 
                vec3(simplex3D(position * energyLevel * m_warpFactor));
            
            // Multi-octave sampling for different energy scales
            float result = 0.0f;
            float amplitude = 1.0f;
            float frequency = m_baseFrequency;
            
            for(int i = 0; i < m_octaves; ++i) {
                result += amplitude * simplex4D(vec4(
                    warpedPos * frequency, 
                    energyLevel * m_energyScale
                ));
                
                frequency *= 2.0f;
                amplitude *= 0.5f;
            }
            
            return result;
        }

        // Optimization: Cache frequently accessed regions
        struct CachedRegion {
            BoundingBox bounds;
            float energyMin, energyMax;
            Grid3D<float> samples;
        };
        
        std::vector<CachedRegion> m_cachedRegions;
    };

    // Energy level as a dimensional value
    struct EnergySpectrum {
        vec4 spectralComponents;  // Different energy manifestations
        
        // Project to scalar when needed
        float toScalar() const {
            return glm::dot(spectralComponents, 
                           vec4(0.2f, 0.3f, 0.3f, 0.2f));  // Weighted projection
        }
        
        // Combine with field modulation
        EnergySpectrum modulate(float fieldValue) const {
            // Non-linear modulation based on field strength
            float modFactor = std::pow(1.0f + std::abs(fieldValue), 2.0f);
            return EnergySpectrum{spectralComponents * modFactor};
        }
    };

    // Wave interaction with energy dimension
    struct ModulatedWave {
        WaveProperty baseProperties;
        EnergySpectrum energySpectrum;
        
        // Compute interaction with energy field
        void interact(const ExcitationField& field, const vec3& position) {
            float fieldValue = field.sampleField(position, 
                                               energySpectrum.toScalar());
            
            // Modulate wave properties based on field interaction
            EnergySpectrum modulated = energySpectrum.modulate(fieldValue);
            
            // Update wave characteristics
            baseProperties.frequency *= 
                1.0f + fieldValue * m_config.frequencyModulation;
            
            // Quantum effects at high energy levels
            if (modulated.toScalar() > FrequencyBand::kXRay) {
                triggerQuantumEffects(modulated);
            }
        }
    };

    void propagateModulatedWave(const vec3& origin, 
                               const vec3& direction, 
                               const EnergySpectrum& initialEnergy) {
        ModulatedWave wave;
        wave.energySpectrum = initialEnergy;
        wave.baseProperties = createBaseWaveProperties(initialEnergy);
        
        // Sample energy field at propagation points
        vec3 samplePoint = origin;
        float stepSize = m_config.propagationStepSize;
        
        while (wave.energySpectrum.toScalar() > m_config.minEnergyThreshold) {
            // Interact with energy dimension
            wave.interact(m_excitationField, samplePoint);
            
            // Update propagation
            samplePoint += direction * stepSize;
            
            // Generate field effects based on modulated energy
            if (shouldGenerateEffects(wave)) {
                generateFieldEffects(samplePoint, wave);
            }
        }
    }

private:
    ExcitationField m_excitationField;
    float m_baseFrequency = 1.0f;
    float m_energyScale = 0.1f;
    float m_warpFactor = 0.5f;
    int m_octaves = 4;
    
    void triggerQuantumEffects(const EnergySpectrum& energy) {
        // Handle high-energy quantum interactions
        // Like particle decay, pair production, etc.
    }
};

} // namespace hd


