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
        nodeType = "Vegetation/VegetationSystem";
        
        inputs = {
            "terrain",           // Terrain data
            "climate",           // Climate zone data
            "density",           // Vegetation density
            "variation",         // Species variation
            "biomeType",         // Biome classification
            "growthParams",      // Growth parameters
            "windInfluence",     // Wind effect on vegetation
            "seasonalParams",    // Seasonal changes
            "proceduralIntent"   // For procedural variation
        };
        
        outputs = {
            "vegetationData",    // Generated vegetation data
            "densityMap",        // Vegetation density distribution
            "growthStates",      // Current growth states
            "windResponse",      // Wind animation data
            "LODData",           // Level of detail data
            "performanceMetrics"
        };

        isSerializable = true;
        isEditableInEditor = true;
        isProcedural = true;
    }
};

class Vegetation : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Vegetation(const VegetationInfo& info = VegetationInfo())
        : Node(info) {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        vegetationPatternId = orchestrator.registerPattern(createDefaultVegetationPattern());
    }
    void initialize() override {}
    void load() override {}

    // Set default values
    TerrainData terrain;
    ClimateData climate;
    float density = 0.0f;
    float variation = 0.0f;
    bool windEffect = false;
    float LODBias = 0.0f;
    BiomeType biomeType = BiomeType::Forest;
    GrowthParams growthParams;
    OctaveParams intent;
    std::string vegetationPatternId;

    // === Processing ===
    void processNode() override {
        auto& orchestrator = ProceduralOrchestrator::getInstance();
        
        // Process inputs
        terrain = getInputValue<TerrainData>("terrain");
        climate = getInputValue<ClimateData>("climate");
        density = getInputValue<float>("density");
        variation = getInputValue<float>("variation");
        biomeType = getInputValue<BiomeType>("biomeType");
        growthParams = getInputValue<GrowthParams>("growthParams");
        intent = getInputValue<OctaveParams>("proceduralIntent");
        
        // Create vegetation pattern parameters
        ProceduralStructureParams params;
        params.type = ProceduralStructureType::Vegetation;
        params.terrainData = terrain;
        params.climateData = climate;
        params.density = density;
        params.variation = variation;
        params.biomeType = biomeType;
        params.growthParams = growthParams;
        
        // Update vegetation pattern
        vegetationPatternId = orchestrator.createVegetationPattern(params);
        auto vegetationData = orchestrator.getProceduralPattern(vegetationPatternId);
        
        // Update outputs
        setOutputValue("vegetationData", vegetationData);
        setOutputValue("densityMap", computeDensityMap(vegetationData));
        setOutputValue("growthStates", computeGrowthStates(vegetationData));
        setOutputValue("windResponse", computeWindResponse(vegetationData));
        setOutputValue("LODData", generateLODData(vegetationData));
        setOutputValue("performanceMetrics", computePerformanceMetrics());
    }
    
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Vegetation() = default;     // Default destructor

private:
};

} // namespace hd