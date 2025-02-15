/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Procedural/HD_ProceduralTypes.h"
#include "Core/Procedural/HD_ProceduralOrchestrator.h"
#include "Core/Rendering/HD_VolumetricTypes.h"

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
        auto atmosphereState = GetInputValue<AtmosphereState>("AtmosphereState");
        auto windVector = GetInputValue<glm::vec3>("WindVector");
        auto coverage = GetInputValue<float>("Coverage");
        auto types = GetInputValue<CloudTypeDistribution>("Types");
        auto evolution = GetInputValue<float>("Evolution");
        auto turbulence = GetInputValue<TurbulenceParams>("Turbulence");
        auto temperature = GetInputValue<float>("Temperature");
        auto humidity = GetInputValue<float>("Humidity");
        auto pressure = GetInputValue<float>("Pressure");
        auto noiseParams = GetInputValue<NoiseParameters>("NoiseParams");

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
        SetOutputValue("VolumetricData", volumetricData);
        SetOutputValue("ShadowData", shadowData);
        SetOutputValue("LightingData", lightingData);
        SetOutputValue("Precipitation", precipitationData);
        SetOutputValue("DensityField", densityField);
        SetOutputValue("ProceduralState", cloudData);
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
```

Would you like me to continue with `HD_FogNode.h` and `HD_PrecipitationNode.h`?
