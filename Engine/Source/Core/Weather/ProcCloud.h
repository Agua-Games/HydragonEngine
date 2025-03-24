/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralCloud.h
 * @brief ProceduralCloud represents a procedural cloud node in the engine's node graph.
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

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
        IsStreamable = true;
    }
};

class ProceduralCloud : public Node<VolumetricData, ShadowData, LightingData, PrecipitationData> {
public:
    explicit ProceduralCloud(const CloudInfo& info = CloudInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        cloudPatternId = orchestrator.registerPattern(createDefaultCloudPattern());
    }

    ~ProceduralCloud() {
        if (!cloudPatternId.empty()) {
            auto& orchestrator = ProceduralOrchestrator::getInstance();
            orchestrator.unregisterPattern(cloudPatternId);
        }
    }

    void processNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Get input values
        auto atmosphereState = getPortValue<AtmosphereState>("AtmosphereState");
        auto windVector = getPortValue<glm::vec3>("WindVector");
        auto coverage = getPortValue<float>("Coverage");
        auto types = getPortValue<CloudTypeDistribution>("Types");
        auto evolution = getPortValue<float>("Evolution");
        auto turbulence = getPortValue<TurbulenceParams>("Turbulence");
        auto temperature = getPortValue<float>("Temperature");
        auto humidity = getPortValue<float>("Humidity");
        auto pressure = getPortValue<float>("Pressure");
        auto noiseParams = getPortValue<NoiseParameters>("NoiseParams");

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
        cloudPatternId = orchestrator.updateCloudPattern(cloudPatternId, params);
        auto cloudData = orchestrator.getProceduralPattern(cloudPatternId);

        // Process outputs
        auto volumetricData = generateVolumetricData(cloudData);
        auto shadowData = generateShadowData(volumetricData);
        auto lightingData = computeLightingData(volumetricData, atmosphereState);
        auto precipitationData = calculatePrecipitationPotential(cloudData);
        auto densityField = generateDensityField(cloudData);

        // Set outputs
        setPortValue("VolumetricData", volumetricData);
        setPortValue("ShadowData", shadowData);
        setPortValue("LightingData", lightingData);
        setPortValue("Precipitation", precipitationData);
        setPortValue("DensityField", densityField);
        setPortValue("ProceduralState", cloudData);
    }

    std::vector<std::string> getInputPorts() const override {
        return getNodeInfo().Inputs;
    }

    std::vector<std::string> getOutputPorts() const override {
        return getNodeInfo().Outputs;
    }

    void onResume() override {}
    void onPause() override {}
    void onDirty() override {
        markDirty();
    }

    uint64_t computeCacheKey() const override {
        // Combine all input values into a hash
        std::size_t seed = 0;
        hashCombine(seed, getPortValue<AtmosphereState>("AtmosphereState"));
        hashCombine(seed, getPortValue<glm::vec3>("WindVector"));
        hashCombine(seed, getPortValue<float>("Coverage"));
        hashCombine(seed, getPortValue<CloudTypeDistribution>("Types"));
        hashCombine(seed, getPortValue<float>("Evolution"));
        return seed;
    }

private:
    std::string cloudPatternId;

    VolumetricData generateVolumetricData(const ProceduralPattern& pattern);
    ShadowData generateShadowData(const VolumetricData& volumetricData);
    LightingData computeLightingData(const VolumetricData& volumetricData, const AtmosphereState& atmosphere);
    PrecipitationData calculatePrecipitationPotential(const ProceduralPattern& pattern);
    DensityField generateDensityField(const ProceduralPattern& pattern);
};

} // namespace hd