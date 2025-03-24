/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Fog.h
 * @brief Fog represents a fog node in the engine's node graph.
 */
#pragma once

#include "Node.h"
#include "VolumetricTypes.h"
#include "WeatherTypes.h"

namespace hd {

struct FogInfo : public NodeInfo {
    FogInfo() {
        NodeType = "Weather/Fog";
        
        inputs = {
            "AtmosphereState",    // From AtmosphereNode
            "WindVector",         // Wind direction/speed
            "Density",           // Fog density
            "Height",            // Height-based falloff
            "Temperature",       // For fog formation
            "Humidity",          // Moisture content
            "NoiseParams"        // Procedural noise settings
        };
        
        outputs = {
            "VolumetricData",     // 3D fog data
            "DensityField",       // 3D density distribution
            "ScatteringParams",   // Light scattering parameters
            "VisibilityData"      // Visibility reduction info
        };

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
        isStreamable = true;
    }
};

class Fog : public Node<VolumetricData, DensityField, ScatteringParams, VisibilityData> {
public:
    explicit Fog(const FogInfo& info = FogInfo())
        : Node(info) {}

    void processNodeGraph() override {
        // Get input values
        auto atmosphereState = getPortValue<AtmosphereState>("AtmosphereState");
        auto windVector = getPortValue<glm::vec3>("WindVector");
        auto density = getPortValue<float>("Density");
        auto height = getPortValue<float>("Height");
        auto temperature = getPortValue<float>("Temperature");
        auto humidity = getPortValue<float>("Humidity");
        auto noiseParams = getPortValue<NoiseParameters>("NoiseParams");

        // Process fog simulation
        auto volumetricData = GenerateVolumetricData(atmosphereState, density, height);
        auto densityField = GenerateDensityField(volumetricData, noiseParams);
        auto scatteringParams = ComputeScatteringParams(volumetricData, atmosphereState);
        auto visibilityData = CalculateVisibilityData(densityField, atmosphereState);

        // Set outputs
        setPortValue("VolumetricData", volumetricData);
        setPortValue("DensityField", densityField);
        setPortValue("ScatteringParams", scatteringParams);
        setPortValue("VisibilityData", visibilityData);
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
        HashCombine(seed, getPortValue<glm::vec3>("WindVector"));
        HashCombine(seed, getPortValue<float>("Density"));
        HashCombine(seed, getPortValue<float>("Height"));
        return seed;
    }

private:
    VolumetricData GenerateVolumetricData(const AtmosphereState& atmosphere, float density, float height);
    DensityField GenerateDensityField(const VolumetricData& volumetricData, const NoiseParameters& noise);
    ScatteringParams ComputeScatteringParams(const VolumetricData& volumetricData, const AtmosphereState& atmosphere);
    VisibilityData CalculateVisibilityData(const DensityField& densityField, const AtmosphereState& atmosphere);
};

} // namespace hd