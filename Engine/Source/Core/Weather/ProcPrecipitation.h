/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcPrecipitation.h
 * @brief ProcPrecipitation represents a procedural precipitation node in the engine's node graph.
 */
#pragma once

#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"
#include "WeatherTypes.h"

namespace hd {

struct PrecipitationInfo : public NodeInfo {
    PrecipitationInfo() {
        NodeType = "Weather/Precipitation";
        
        inputs = {
            "AtmosphereState",     // From AtmosphereNode
            "CloudData",           // From CloudNode
            "WindVector",          // Wind direction/speed
            "Intensity",           // Precipitation intensity
            "Temperature",         // For determining rain/snow
            "ParticleParams",      // Particle system settings
            "CollisionMask"        // World collision data
        };
        
        outputs = {
            "ParticleData",        // Precipitation particles
            "SurfaceEffects",      // Surface interaction effects
            "AudioData",           // Precipitation sound data
            "WetnessMap",          // Surface wetness information
            "AccumulationData"     // Snow/water accumulation
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
        isStreamable = true;
    }
};

class ProcPrecipitation : public Node<ParticleData, SurfaceEffects, AudioData, AccumulationData> {
public:
    explicit ProcPrecipitation(const PrecipitationInfo& info = PrecipitationInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        precipPatternId = orchestrator.registerPattern(createDefaultPrecipitationPattern());
    }

    ~ProcPrecipitation() {
        if (!precipPatternId.empty()) {
            auto& orchestrator = ProceduralOrchestrator::getInstance();
            orchestrator.unregisterPattern(precipPatternId);
        }
    }

    void () override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();

        // Get input values
        auto atmosphereState = getPortValue<AtmosphereState>("AtmosphereState");
        auto cloudData = getPortValue<CloudData>("CloudData");
        auto windVector = getPortValue<glm::vec3>("WindVector");
        auto intensity = getPortValue<float>("Intensity");
        auto temperature = getPortValue<float>("Temperature");
        auto particleParams = getPortValue<ParticleParameters>("ParticleParams");
        auto collisionMask = getPortValue<CollisionData>("CollisionMask");

        // Update procedural pattern
        ProceduralStructureParams patternParams;
        patternParams.intensity = intensity;
        patternParams.temperature = temperature;
        patternParams.windInfluence = glm::length(windVector);
        
        precipPatternId = orchestrator.createMaterialPattern(patternParams);
        auto precipPattern = orchestrator.getProceduralPattern(precipPatternId);

        // Process precipitation using pattern data
        auto particleData = generateParticleData(cloudData, precipPattern, particleParams);
        auto surfaceEffects = processSurfaceEffects(particleData, collisionMask);
        auto audioData = generateAudioData(particleData, surfaceEffects);
        auto wetnessMap = updateWetnessMap(surfaceEffects, temperature);
        auto accumulationData = processAccumulation(particleData, temperature);

        // Set outputs
        setPortValue("ParticleData", particleData);
        setPortValue("SurfaceEffects", surfaceEffects);
        setPortValue("AudioData", audioData);
        setPortValue("WetnessMap", wetnessMap);
        setPortValue("AccumulationData", accumulationData);
    }

    std::vector<std::string> getInputPorts() const override {
        return getNodeInfo().inputs;
    }

    std::vector<std::string> getOutputPorts() const override {
        return getNodeInfo().outputs;
    }

    void onResume() override {}
    void onPause() override {}
    void onDirty() override {
        markDirty();
    }

    uint64_t computeCacheKey() const override {
        std::size_t seed = 0;
        HashCombine(seed, getPortValue<AtmosphereState>("AtmosphereState"));
        HashCombine(seed, getPortValue<CloudData>("CloudData"));
        HashCombine(seed, getPortValue<float>("Intensity"));
        HashCombine(seed, getPortValue<float>("Temperature"));
        HashCombine(seed, precipPatternId);
        return seed;
    }

private:
    std::string precipPatternId;

    std::unique_ptr<IPattern> CreateDefaultPrecipitationPattern() {
        return std::make_unique<ProceduralPattern>(
            ProceduralPatternType::VolumeTexture,
            ProceduralStructureParams{}
        );
    }

    ParticleData GenerateParticleData(const CloudData& clouds, const ProceduralPatternData& pattern, const ParticleParameters& params);
    SurfaceEffects ProcessSurfaceEffects(const ParticleData& particles, const CollisionData& collision);
    AudioData GenerateAudioData(const ParticleData& particles, const SurfaceEffects& effects);
    WetnessMap UpdateWetnessMap(const SurfaceEffects& effects, float temperature);
    AccumulationData ProcessAccumulation(const ParticleData& particles, float temperature);
};

} // namespace hd