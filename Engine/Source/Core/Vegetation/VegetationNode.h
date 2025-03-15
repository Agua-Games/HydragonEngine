/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VegetationNode.h
 * @brief VegetationNode represents a vegetation node in the engine's node graph.
 */
#pragma once

#include "Node.h"
#include "ProceduralTypes.h"
#include "ProceduralOrchestrator.h"

namespace hd {

struct VegetationInfo : public NodeInfo {
    VegetationInfo() {
        NodeType = "Vegetation/VegetationSystem";
        
        Inputs = {
            "Terrain",           // Terrain data
            "Climate",           // Climate zone data
            "Density",           // Vegetation density
            "BiomeType",         // Biome classification
            "GrowthParams",      // Growth parameters
            "WindInfluence",     // Wind effect on vegetation
            "SeasonalParams",    // Seasonal changes
            "ProceduralIntent"   // For procedural variation
        };
        
        Outputs = {
            "VegetationData",    // Generated vegetation data
            "DensityMap",        // Vegetation density distribution
            "GrowthStates",      // Current growth states
            "WindResponse",      // Wind animation data
            "LODData",           // Level of detail data
            "PerformanceMetrics"
        };

        IsSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class VegetationNode : public Node {
public:
    explicit VegetationNode(const VegetationInfo& info = VegetationInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        vegetationPatternId = orchestrator.RegisterPattern(CreateDefaultVegetationPattern());
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        
        // Process inputs
        auto terrain = GetInputValue<TerrainData>("Terrain");
        auto climate = GetInputValue<ClimateData>("Climate");
        auto density = GetInputValue<float>("Density");
        auto biomeType = GetInputValue<BiomeType>("BiomeType");
        auto growthParams = GetInputValue<GrowthParams>("GrowthParams");
        auto intent = GetInputValue<OctaveParams>("ProceduralIntent");
        
        // Create vegetation pattern parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Vegetation;
        params.terrainData = terrain;
        params.climateData = climate;
        params.density = density;
        params.biomeType = biomeType;
        params.growthParams = growthParams;
        
        // Update vegetation pattern
        vegetationPatternId = orchestrator.CreateVegetationPattern(params);
        auto vegetationData = orchestrator.GetProceduralPattern(vegetationPatternId);
        
        // Update outputs
        SetOutputValue("VegetationData", vegetationData);
        SetOutputValue("DensityMap", ComputeDensityMap(vegetationData));
        SetOutputValue("GrowthStates", ComputeGrowthStates(vegetationData));
        SetOutputValue("WindResponse", ComputeWindResponse(vegetationData));
        SetOutputValue("LODData", GenerateLODData(vegetationData));
        SetOutputValue("PerformanceMetrics", ComputePerformanceMetrics());
    }

private:
    std::string vegetationPatternId;
};

} // namespace hd