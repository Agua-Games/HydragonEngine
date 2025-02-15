/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Procedural/HD_ProceduralTypes.h"
#include "Core/Procedural/HD_ProceduralOrchestrator.h"

namespace hd {

struct HD_VegetationInfo : public HD_NodeInfo {
    HD_VegetationInfo() {
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

class HD_VegetationNode : public HD_Node {
public:
    explicit HD_VegetationNode(const HD_VegetationInfo& info = HD_VegetationInfo())
        : HD_Node(info) {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        vegetationPatternId = orchestrator.RegisterPattern(CreateDefaultVegetationPattern());
    }

    void ProcessNodeGraph() override {
        auto& orchestrator = HD_ProceduralOrchestrator::GetInstance();
        
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