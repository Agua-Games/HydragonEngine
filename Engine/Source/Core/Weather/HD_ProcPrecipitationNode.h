/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_ProcPrecipitationNode.h
 * @brief ProcPrecipitationNode represents a procedural precipitation node in the engine's node graph.
 */
#pragma once

#include "HD_Node.h"
#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "HD_WeatherTypes.h"

namespace hd {

struct HD_PrecipitationInfo : public HD_NodeInfo {
    HD_PrecipitationInfo() {
        NodeType = "Weather/Precipitation";
        
        Inputs = {
            "AtmosphereState",     // From AtmosphereNode
            "CloudData",           // From CloudNode
            "WindVector",          // Wind direction/speed
            "Intensity",           // Precipitation intensity
            "Temperature",         // For determining rain/snow
            "ParticleParams",      // Particle system settings
            "CollisionMask"        // World collision data
        };
        
        Outputs = {
            "ParticleData",        // Precipitation particles
            "SurfaceEffects",      // Surface interaction effects
            "AudioData",           // Precipitation sound data
            "WetnessMap",          // Surface wetness information
            "AccumulationData"     // Snow/water accumulation
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
        IsStreamable = true;
    }
};

class HD_ProcPrecipitationNode : public HD_Node<ParticleData, SurfaceEffects, AudioData, AccumulationData> {
public:
    explicit HD_ProcPrecipitationNode(const HD_PrecipitationInfo& info = HD_PrecipitationInfo())
        : HD_Node(info) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        precipPatternId = orchestrator.RegisterPattern(CreateDefaultPrecipitationPattern());
    }

    ~HD_ProcPrecipitationNode() {
        if (!precipPatternId.empty()) {
            auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
            orchestrator.UnregisterPattern(precipPatternId);
        }
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();

        // Get input values
        auto atmosphereState = GetPortValue<AtmosphereState>("AtmosphereState");
        auto cloudData = GetPortValue<CloudData>("CloudData");
        auto windVector = GetPortValue<glm::vec3>("WindVector");
        auto intensity = GetPortValue<float>("Intensity");
        auto temperature = GetPortValue<float>("Temperature");
        auto particleParams = GetPortValue<ParticleParameters>("ParticleParams");
        auto collisionMask = GetPortValue<CollisionData>("CollisionMask");

        // Update procedural pattern
        ProceduralStructureParams patternParams;
        patternParams.intensity = intensity;
        patternParams.temperature = temperature;
        patternParams.windInfluence = glm::length(windVector);
        
        precipPatternId = orchestrator.CreateMaterialPattern(patternParams);
        auto precipPattern = orchestrator.GetProceduralPattern(precipPatternId);

        // Process precipitation using pattern data
        auto particleData = GenerateParticleData(cloudData, precipPattern, particleParams);
        auto surfaceEffects = ProcessSurfaceEffects(particleData, collisionMask);
        auto audioData = GenerateAudioData(particleData, surfaceEffects);
        auto wetnessMap = UpdateWetnessMap(surfaceEffects, temperature);
        auto accumulationData = ProcessAccumulation(particleData, temperature);

        // Set outputs
        SetPortValue("ParticleData", particleData);
        SetPortValue("SurfaceEffects", surfaceEffects);
        SetPortValue("AudioData", audioData);
        SetPortValue("WetnessMap", wetnessMap);
        SetPortValue("AccumulationData", accumulationData);
    }

    std::vector<std::string> GetInputPorts() const override {
        return GetNodeInfo().Inputs;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return GetNodeInfo().Outputs;
    }

    void OnResume() override {}
    void OnPause() override {}
    void OnDirty() override {
        MarkDirty();
    }

    uint64_t ComputeCacheKey() const override {
        std::size_t seed = 0;
        HashCombine(seed, GetPortValue<AtmosphereState>("AtmosphereState"));
        HashCombine(seed, GetPortValue<CloudData>("CloudData"));
        HashCombine(seed, GetPortValue<float>("Intensity"));
        HashCombine(seed, GetPortValue<float>("Temperature"));
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