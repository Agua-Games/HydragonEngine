/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_FogNode.h
 * @brief FogNode represents a fog node in the engine's node graph.
 */
#pragma once

#include "HD_Node.h"
#include "HD_VolumetricTypes.h"
#include "HD_WeatherTypes.h"

namespace hd {

struct HD_FogInfo : public HD_NodeInfo {
    HD_FogInfo() {
        NodeType = "Weather/Fog";
        
        Inputs = {
            "AtmosphereState",    // From AtmosphereNode
            "WindVector",         // Wind direction/speed
            "Density",           // Fog density
            "Height",            // Height-based falloff
            "Temperature",       // For fog formation
            "Humidity",          // Moisture content
            "NoiseParams"        // Procedural noise settings
        };
        
        Outputs = {
            "VolumetricData",     // 3D fog data
            "DensityField",       // 3D density distribution
            "ScatteringParams",   // Light scattering parameters
            "VisibilityData"      // Visibility reduction info
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
        IsStreamable = true;
    }
};

class HD_FogNode : public HD_Node<VolumetricData, DensityField, ScatteringParams, VisibilityData> {
public:
    explicit HD_FogNode(const HD_FogInfo& info = HD_FogInfo())
        : HD_Node(info) {}

    void ProcessNodeGraph() override {
        // Get input values
        auto atmosphereState = GetPortValue<AtmosphereState>("AtmosphereState");
        auto windVector = GetPortValue<glm::vec3>("WindVector");
        auto density = GetPortValue<float>("Density");
        auto height = GetPortValue<float>("Height");
        auto temperature = GetPortValue<float>("Temperature");
        auto humidity = GetPortValue<float>("Humidity");
        auto noiseParams = GetPortValue<NoiseParameters>("NoiseParams");

        // Process fog simulation
        auto volumetricData = GenerateVolumetricData(atmosphereState, density, height);
        auto densityField = GenerateDensityField(volumetricData, noiseParams);
        auto scatteringParams = ComputeScatteringParams(volumetricData, atmosphereState);
        auto visibilityData = CalculateVisibilityData(densityField, atmosphereState);

        // Set outputs
        SetPortValue("VolumetricData", volumetricData);
        SetPortValue("DensityField", densityField);
        SetPortValue("ScatteringParams", scatteringParams);
        SetPortValue("VisibilityData", visibilityData);
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
        HashCombine(seed, GetPortValue<glm::vec3>("WindVector"));
        HashCombine(seed, GetPortValue<float>("Density"));
        HashCombine(seed, GetPortValue<float>("Height"));
        return seed;
    }

private:
    VolumetricData GenerateVolumetricData(const AtmosphereState& atmosphere, float density, float height);
    DensityField GenerateDensityField(const VolumetricData& volumetricData, const NoiseParameters& noise);
    ScatteringParams ComputeScatteringParams(const VolumetricData& volumetricData, const AtmosphereState& atmosphere);
    VisibilityData CalculateVisibilityData(const DensityField& densityField, const AtmosphereState& atmosphere);
};

} // namespace hd