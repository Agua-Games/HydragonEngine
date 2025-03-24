/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Vegetation.h
 * @brief Vegetation represents a vegetation node in the engine's node graph.
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

        isSerializable = true;
        IsEditableInEditor = true;
        IsProcedural = true;
    }
};

class Vegetation : public Node {
public:
    explicit Vegetation(const VegetationInfo& info = VegetationInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        vegetationPatternId = orchestrator.registerPattern(createDefaultVegetationPattern());
    }

    void processNodeGraph() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Process inputs
        auto terrain = getInputValue<TerrainData>("Terrain");
        auto climate = getInputValue<ClimateData>("Climate");
        auto density = getInputValue<float>("Density");
        auto biomeType = getInputValue<BiomeType>("BiomeType");
        auto growthParams = getInputValue<GrowthParams>("GrowthParams");
        auto intent = getInputValue<OctaveParams>("ProceduralIntent");
        
        // Create vegetation pattern parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Vegetation;
        params.terrainData = terrain;
        params.climateData = climate;
        params.density = density;
        params.biomeType = biomeType;
        params.growthParams = growthParams;
        
        // Update vegetation pattern
        vegetationPatternId = orchestrator.createVegetationPattern(params);
        auto vegetationData = orchestrator.getProceduralPattern(vegetationPatternId);
        
        // Update outputs
        setOutputValue("VegetationData", vegetationData);
        setOutputValue("DensityMap", computeDensityMap(vegetationData));
        setOutputValue("GrowthStates", computeGrowthStates(vegetationData));
        setOutputValue("WindResponse", computeWindResponse(vegetationData));
        setOutputValue("LODData", generateLODData(vegetationData));
        setOutputValue("PerformanceMetrics", computePerformanceMetrics());
    }

private:
    std::string vegetationPatternId;
};

} // namespace hd