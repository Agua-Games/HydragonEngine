/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Terrain generation system
 */

#pragma once
#include "../ProceduralTypes.h"
#include <memory>

namespace Hydragon::Procedural {

class ITerrainGenerator {
public:
    virtual ~ITerrainGenerator() = default;
    
    struct TerrainParams {
        float baseHeight = 100.0f;
        float mountainScale = 1000.0f;
        float roughness = 0.5f;
        float erosionIterations = 1000;
        bool generateTextures = true;
        bool generateNormals = true;
    };
    
    virtual void SetParameters(const TerrainParams& params) = 0;
    virtual void Generate(const GenerationContext& context) = 0;
    virtual float SampleHeight(float x, float z) const = 0;
    virtual Vector3 SampleNormal(float x, float z) const = 0;
};

class TerrainGenerator : public ITerrainGenerator {
public:
    explicit TerrainGenerator(const TerrainParams& params = {});
    
    void SetParameters(const TerrainParams& params) override;
    void Generate(const GenerationContext& context) override;
    float SampleHeight(float x, float z) const override;
    Vector3 SampleNormal(float x, float z) const override;

private:
    TerrainParams m_Params;
    std::unique_ptr<IHeightfield> m_Heightfield;
    std::unique_ptr<IErosionSimulator> m_Erosion;
};

} // namespace Hydragon::Procedural 