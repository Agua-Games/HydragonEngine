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

struct HD_FogNodeInfo : public HD_NodeInfo {
    bool useVolumetrics = true;
    bool supportsDynamicPatterns = true;

    HD_FogNodeInfo() {
        NodeType = "Weather/Fog";
        IsStreamable = true;
        IsAsyncLoadable = true;
        
        Inputs = {
            "WindIntensity",     // Wind effect on fog
            "Humidity",          // Moisture content
            "Temperature",       // Temperature influence
            "DensityPattern",    // Base density distribution
            "TurbulencePattern", // Turbulence influence
            "GlobalAtmosphere",  // Atmospheric state
            "TimeScale",         // Time-based evolution
            "Evolution"          // Pattern evolution
        };
        
        Outputs = {
            "VolumetricTexture", // 3D fog texture
            "DensityField",      // 3D density distribution
            "ScatteringParams",  // Light scattering parameters
            "FogState"           // Current fog state
        };
    }
};

class HD_FogNode : public HD_Node<VolumetricTexture, DensityField, ScatteringParams> {
public:
    explicit HD_FogNode(const HD_FogNodeInfo& info = HD_FogNodeInfo())
        : HD_Node(info), FogInfo(info) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        fogPatternId = orchestrator.RegisterPattern(CreateDefaultFogPattern());
    }

    ~HD_FogNode() {
        if (!fogPatternId.empty()) {
            auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
            orchestrator.UnregisterPattern(fogPatternId);
        }
    }

    void ProcessNodeGraph() override {
        // Get input values
        float windIntensity = GetInputValue<float>("WindIntensity");
        float humidity = GetInputValue<float>("Humidity");
        float temperature = GetInputValue<float>("Temperature");
        
        auto densityPattern = GetInputValue<ProceduralPatternData>("DensityPattern");
        auto turbulencePattern = GetInputValue<ProceduralPatternData>("TurbulencePattern");
        auto atmosphereState = GetInputValue<AtmosphereState>("GlobalAtmosphere");
        
        // Create procedural pattern if none provided
        if (!densityPattern.IsValid()) {
            ProceduralStructureParams fogParams;
            fogParams.type = ProceduralStructureType::Organic;
            fogParams.regularity = windIntensity * 0.5f;
            fogParams.complexity = humidity;
            
            auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
            fogPatternId = orchestrator.CreateVolumeTexturePattern(fogParams);
            densityPattern = orchestrator.GetProceduralPattern(fogPatternId);
        }

        // Process patterns and generate outputs
        auto volumetricTexture = GenerateVolumetricTexture(densityPattern, turbulencePattern);
        auto densityField = ComputeDensityField(densityPattern, temperature);
        auto scatteringParams = CalculateScatteringParams(humidity, temperature, atmosphereState);

        // Set output values
        SetOutputValue("VolumetricTexture", volumetricTexture);
        SetOutputValue("DensityField", densityField);
        SetOutputValue("ScatteringParams", scatteringParams);
        SetOutputValue("FogState", GenerateFogState());
    }

    std::vector<std::string> GetInputPorts() const override {
        return FogInfo.Inputs;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return FogInfo.Outputs;
    }

    void DrawInNodeGraph() override {
        ImGui::BeginGroup();
        ImGui::Text("Fog Node");
        
        // Draw input ports
        DrawInputPort("WindIntensity", "Wind");
        DrawInputPort("Humidity", "Humidity");
        DrawInputPort("Temperature", "Temp");
        DrawInputPort("DensityPattern", "Density");
        DrawInputPort("TurbulencePattern", "Turbulence");
        
        // Draw output ports
        DrawOutputPort("VolumetricTexture", "Volume");
        DrawOutputPort("DensityField", "Density");
        DrawOutputPort("ScatteringParams", "Scatter");
        
        ImGui::EndGroup();
    }

private:
    HD_FogNodeInfo FogInfo;
    std::string fogPatternId;

    VolumetricTexture GenerateVolumetricTexture(const ProceduralPatternData& density, 
                                               const ProceduralPatternData& turbulence);
    DensityField ComputeDensityField(const ProceduralPatternData& pattern, float temperature);
    ScatteringParams CalculateScatteringParams(float humidity, float temperature, 
                                             const AtmosphereState& atmosphere);
    FogState GenerateFogState();
};

} // namespace hd
