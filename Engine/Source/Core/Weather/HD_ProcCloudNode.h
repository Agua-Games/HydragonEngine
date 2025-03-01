/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_ProcCloudNode.h
 * @brief ProcCloudNode represents a procedural cloud node in the engine's node graph.
 */
#pragma once

#include "HD_Node.h"
#include "HD_ProceduralTypes.h"
#include "HD_ProceduralOrchestrator.h"
#include "HD_VolumetricTypes.h"

namespace hd {

struct HD_CloudInfo : public HD_NodeInfo {
    HD_CloudInfo() {
        NodeType = "Weather/Clouds";
        
        Inputs = {
            "AtmosphereState",  // From AtmosphereNode
            "WindVector",       // Wind direction/speed
            "Coverage",         // Cloud coverage
            "Types",           // Cloud type distribution
            "Evolution",       // Time-based evolution
            "Turbulence",      // Wind turbulence
            "Temperature",     // For cloud formation
            "Humidity",        // Moisture content
            "Pressure",        // Atmospheric pressure
            "NoiseParams"      // Procedural noise settings
        };
        
        Outputs = {
            "VolumetricData",   // 3D cloud data
            "ShadowData",       // Cloud shadows
            "LightingData",     // Cloud lighting
            "Precipitation",    // Rain/snow potential
            "DensityField",     // 3D density distribution
            "ProceduralState"   // Current procedural state
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
        IsStreamable = true;
    }
};

class HD_ProcCloudNode : public HD_Node<VolumetricData, ShadowData, LightingData, PrecipitationData> {
public:
    explicit HD_ProcCloudNode(const HD_CloudInfo& info = HD_CloudInfo())
        : HD_Node(info) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        cloudPatternId = orchestrator.RegisterPattern(CreateDefaultCloudPattern());
    }

    ~HD_ProcCloudNode() {
        if (!cloudPatternId.empty()) {
            auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
            orchestrator.UnregisterPattern(cloudPatternId);
        }
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
        // Get input values
        auto atmosphereState = GetPortValue<AtmosphereState>("AtmosphereState");
        auto windVector = GetPortValue<glm::vec3>("WindVector");
        auto coverage = GetPortValue<float>("Coverage");
        auto types = GetPortValue<CloudTypeDistribution>("Types");
        auto evolution = GetPortValue<float>("Evolution");
        auto turbulence = GetPortValue<TurbulenceParams>("Turbulence");
        auto temperature = GetPortValue<float>("Temperature");
        auto humidity = GetPortValue<float>("Humidity");
        auto pressure = GetPortValue<float>("Pressure");
        auto noiseParams = GetPortValue<NoiseParameters>("NoiseParams");

        // Update procedural pattern
        ProceduralCloudParams params;
        params.atmosphereState = atmosphereState;
        params.windVector = windVector;
        params.coverage = coverage;
        params.typeDistribution = types;
        params.evolutionTime = evolution;
        params.turbulence = turbulence;
        params.temperature = temperature;
        params.humidity = humidity;
        params.pressure = pressure;
        params.noise = noiseParams;

        // Generate cloud pattern
        cloudPatternId = orchestrator.UpdateCloudPattern(cloudPatternId, params);
        auto cloudData = orchestrator.GetProceduralPattern(cloudPatternId);

        // Process outputs
        auto volumetricData = GenerateVolumetricData(cloudData);
        auto shadowData = GenerateShadowData(volumetricData);
        auto lightingData = ComputeLightingData(volumetricData, atmosphereState);
        auto precipitationData = CalculatePrecipitationPotential(cloudData);
        auto densityField = GenerateDensityField(cloudData);

        // Set outputs
        SetPortValue("VolumetricData", volumetricData);
        SetPortValue("ShadowData", shadowData);
        SetPortValue("LightingData", lightingData);
        SetPortValue("Precipitation", precipitationData);
        SetPortValue("DensityField", densityField);
        SetPortValue("ProceduralState", cloudData);
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
        // Combine all input values into a hash
        std::size_t seed = 0;
        HashCombine(seed, GetPortValue<AtmosphereState>("AtmosphereState"));
        HashCombine(seed, GetPortValue<glm::vec3>("WindVector"));
        HashCombine(seed, GetPortValue<float>("Coverage"));
        HashCombine(seed, GetPortValue<CloudTypeDistribution>("Types"));
        HashCombine(seed, GetPortValue<float>("Evolution"));
        return seed;
    }

private:
    std::string cloudPatternId;

    VolumetricData GenerateVolumetricData(const ProceduralPattern& pattern);
    ShadowData GenerateShadowData(const VolumetricData& volumetricData);
    LightingData ComputeLightingData(const VolumetricData& volumetricData, const AtmosphereState& atmosphere);
    PrecipitationData CalculatePrecipitationPotential(const ProceduralPattern& pattern);
    DensityField GenerateDensityField(const ProceduralPattern& pattern);
};

} // namespace hd