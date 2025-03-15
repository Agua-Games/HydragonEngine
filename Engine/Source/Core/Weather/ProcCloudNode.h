/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProcCloudNode.h
 * @brief ProcCloudNode represents a procedural cloud node in the engine's node graph.
 */
#pragma once

#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"
#include "VolumetricTypes.h"

namespace hd {

struct CloudInfo : public NodeInfo {
    CloudInfo() {
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

class ProcCloudNode : public Node<VolumetricData, ShadowData, LightingData, PrecipitationData> {
public:
    explicit ProcCloudNode(const CloudInfo& info = CloudInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        cloudPatternId = orchestrator.RegisterPattern(CreateDefaultCloudPattern());
    }

    ~ProcCloudNode() {
        if (!cloudPatternId.empty()) {
            auto& orchestrator = ProceduralOrchestrator::GetInstance();
            orchestrator.UnregisterPattern(cloudPatternId);
        }
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        
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