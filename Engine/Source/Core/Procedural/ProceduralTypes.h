/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralTypes.h
 * @brief ProceduralTypes defines the types and structures used in procedural generation.
 * 
 * ARCHITECTURAL NOTES:
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */
#pragma once
#include <functional>
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace hd {

enum class NoiseType {
    Perlin,
    Simplex,
    Worley,
    Value,
    Gabor,
    Custom
};

enum class ProceduralStructureType {
    Geometric,
    Voronoi,
    Organic,
    Fractal,
    Evolving,
    Custom
};

// Base pattern types that can emerge in any system
enum class PatternType {
    Structural,     // Strong organizational patterns
    Behavioral,     // Dynamic interaction patterns
    Temporal,       // Time-based patterns
    Spatial,        // Space and layout patterns
    Procedural,     // Concrete procedural generation patterns
    Custom          // User-defined patterns
};

// Concrete procedural pattern types
enum class ProceduralPatternType {
    Material,       // Material patterns (albedo, roughness, etc.)
    Texture,        // Direct texture patterns
    VolumeTexture, // 3D textures (fog, clouds, etc.)
    Noise,         // Base noise patterns
    Geometry,      // Geometric patterns
    Evolution,     // Evolving patterns
    Custom         // User-defined procedural patterns
};

// Abstract representation of system domains
enum class SystemDomain {
    Narrative,      // Story and quest patterns
    Environment,    // World and atmosphere
    Character,      // NPCs and entities
    Audio,          // Sound and music
    Visual,         // Graphics and effects
    Interaction,    // Gameplay and mechanics
    Procedural      // Concrete procedural generation
};

// Mutation strategies
enum MutationType {
    Random,
    Adaptive,
    Hybrid
};

// Crossover strategies
enum CrossoverType {
    Uniform,
    Blend,
    Hybrid
};

// Adaptation strategies
enum AdaptationType {
    Dynamic,
    Static,
    Hybrid
};

struct EvolutionParameters {
    float mutationRate = 0.1f;
    float crossoverRate = 0.7f;
    float adaptationRate = 0.5f;
    int generationLimit = 100;
    int populationSize = 100;
    float stabilityThreshold = 0.9f;
};

struct EvolutionState {
    int currentGeneration = 0;
    float currentFitness = 0.0f;
    float bestFitness = 0.0f;
    ProceduralPatternData bestPattern;
};

// Procedural structure parameters (from Material)
struct ProceduralStructureParams {
    ProceduralStructureType type = ProceduralStructureType::Organic;
    float regularity = 0.5f;     // 0 = chaotic, 1 = regular
    float complexity = 0.5f;     // Influences pattern detail level
    float variation = 0.5f;      // Pattern variation amount
    std::function<float(float, float, float)> customAlgorithm;
};

// Octave-based control parameters (inspired by frequency bands)
struct OctaveParams {
    float lowEnd = 0.5f;     // Coarse/macro patterns (0-1)
    float highEnd = 0.5f;    // Fine/micro details (0-1)
    float intensity = 1.0f;  // Overall strength (0-1)
    float coherence = 0.7f;  // Pattern consistency (0-1)

    // Convert to procedural structure params
    ProceduralStructureParams ToStructureParams() const {
        ProceduralStructureParams params;
        params.regularity = coherence;
        params.complexity = highEnd;
        params.variation = 1.0f - lowEnd;
        return params;
    }
};

// Harmony parameters for cross-system coordination
struct HarmonyParams {
    float balance = 0.5f;        // System equilibrium
    float influence = 0.5f;      // Cross-system effect
    float adaptation = 0.5f;     // Response to changes
    std::unordered_map<SystemDomain, float> weights;
};

// Concrete procedural pattern data
struct ProceduralPatternData {
    ProceduralPatternType type;
    ProceduralStructureParams structureParams;
    std::unordered_map<std::string, float> parameters;  // Additional pattern-specific params
};

// Fitness function for evolution
struct FitnessFunction {
    float operator()(const ProceduralPatternData& pattern) const {
        // Implement fitness calculation logic
        return 0.0f;
    }
};

// Base interface for all patterns
class IPattern {
public:
    virtual ~IPattern() = default;
    virtual void Extract() = 0;
    virtual void Apply(const OctaveParams& octaves) = 0;
    virtual void harmonize(const HarmonyParams& harmony) = 0;
    virtual PatternType GetType() const = 0;
    virtual SystemDomain GetDomain() const = 0;
    
    // Optional procedural interface
    virtual bool IsProceduralPattern() const { return false; }
    virtual ProceduralPatternData GetProceduralData() const { return {}; }
};

// Concrete procedural pattern implementation
class ProceduralPattern : public IPattern {
public:
    ProceduralPattern(ProceduralPatternType type, const ProceduralStructureParams& params)
        : patternType(type), structureParams(params) {}

    PatternType GetType() const override { return PatternType::Procedural; }
    SystemDomain GetDomain() const override { return SystemDomain::Procedural; }
    bool IsProceduralPattern() const override { return true; }
    
    ProceduralPatternData GetProceduralData() const override {
        return ProceduralPatternData{
            .type = patternType,
            .structureParams = structureParams,
            .parameters = parameters
        };
    }

    void SetParameter(const std::string& name, float value) {
        parameters[name] = value;
    }

private:
    ProceduralPatternType patternType;
    ProceduralStructureParams structureParams;
    std::unordered_map<std::string, float> parameters;
};

// Core pattern operations
using PatternExtractor = std::function<void(const OctaveParams&)>;
using HarmonyResolver = std::function<void(const HarmonyParams&)>;

// Utility functions
float ComputeHarmony(const HarmonyParams& params);
float ModulateOctave(float value, const OctaveParams& octaves);
bool ValidatePatternCoherence(const IPattern& pattern, float threshold);

} // namespace hd
