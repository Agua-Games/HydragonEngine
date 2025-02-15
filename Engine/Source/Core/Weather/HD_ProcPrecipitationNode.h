/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Procedural/HD_ProceduralTypes.h"
#include "Core/Procedural/HD_ProceduralOrchestrator.h"
#include "Core/Weather/HD_WeatherTypes.h"

namespace hd {

struct HD_ProcPrecipitationInfo : public HD_NodeInfo {
    HD_ProcPrecipitationInfo() {
        NodeType = "Weather/Precipitation";
        
        Inputs = {
            "CloudData",        // From CloudNode
            "Temperature",      // For rain/snow decision
            "Intensity",       // Precipitation amount
            "WindInfluence",   // Wind effect
            "CollisionMask"    // Scene collision
        };
        
        Outputs = {
            "ParticleEmitters", // Rain/snow particles
            "SurfaceEffects",   // Surface impacts
            "AccumulationData", // Snow accumulation
            "WetnessData"      // Surface wetness
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class HD_ProcPrecipitationNode : public HD_Node<ParticleEmitterData, SurfaceEffectData, AccumulationData, WetnessData> {
public:
    explicit HD_ProcPrecipitationNode(const HD_ProcPrecipitationInfo& info = HD_ProcPrecipitationInfo())
        : HD_Node(info), PrecipInfo(info) {
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
        // Get input values
        auto cloudData = GetInputValue<CloudData>("CloudData");
        float temperature = GetInputValue<float>("Temperature");
        float intensity = GetInputValue<float>("Intensity");
        auto windInfluence = GetInputValue<WindData>("WindInfluence");
        auto collisionMask = GetInputValue<CollisionData>("CollisionMask");

        // Determine precipitation type based on temperature
        PrecipitationType precipType = (temperature <= 0.0f) ? PrecipitationType::Snow 
                                                           : PrecipitationType::Rain;

        // Generate precipitation data
        auto emitters = GenerateParticleEmitters(precipType, intensity, windInfluence);
        auto surfaceEffects = GenerateSurfaceEffects(precipType, intensity, collisionMask);
        auto accumulation = ComputeAccumulation(precipType, intensity, temperature);
        auto wetness = ComputeWetness(precipType, intensity, temperature);

        // Set output values
        SetOutputValue("ParticleEmitters", emitters);
        SetOutputValue("SurfaceEffects", surfaceEffects);
        SetOutputValue("AccumulationData", accumulation);
        SetOutputValue("WetnessData", wetness);
    }

    std::vector<std::string> GetInputPorts() const override {
        return PrecipInfo.Inputs;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return PrecipInfo.Outputs;
    }

    void DrawInNodeGraph() override {
        ImGui::BeginGroup();
        ImGui::Text("Precipitation Node");
        
        // Draw input ports
        DrawInputPort("CloudData", "Clouds");
        DrawInputPort("Temperature", "Temp");
        DrawInputPort("Intensity", "Intensity");
        DrawInputPort("WindInfluence", "Wind");
        
        // Draw output ports
        DrawOutputPort("ParticleEmitters", "Particles");
        DrawOutputPort("SurfaceEffects", "Effects");
        DrawOutputPort("AccumulationData", "Accumulation");
        
        ImGui::EndGroup();
    }

private:
    HD_ProcPrecipitationInfo PrecipInfo;
    std::string precipPatternId;

    enum class PrecipitationType {
        Rain,
        Snow
    };

    ParticleEmitterData GenerateParticleEmitters(PrecipitationType type, 
                                                float intensity, 
                                                const WindData& wind);

    SurfaceEffectData GenerateSurfaceEffects(PrecipitationType type, 
                                            float intensity,
                                            const CollisionData& collision);

    AccumulationData ComputeAccumulation(PrecipitationType type,
                                       float intensity,
                                       float temperature);

    WetnessData ComputeWetness(PrecipitationType type,
                              float intensity,
                              float temperature);

    ProceduralPattern CreateDefaultPrecipitationPattern() const;
};

} // namespace hd
```

This implementation includes:
1. A node info struct defining inputs/outputs
2. The main precipitation node class with type-safe template parameters
3. Processing logic for both rain and snow
4. Surface effect generation
5. Accumulation and wetness computation
6. Node graph editor integration

Would you like me to proceed with implementing any of the private member functions, or would you prefer to move on to another file?
