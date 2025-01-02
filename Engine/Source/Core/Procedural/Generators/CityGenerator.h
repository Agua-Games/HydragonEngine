/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * City generation system
 */

#pragma once
#include "../ProceduralTypes.h"
#include <memory>
#include <vector>

namespace Hydragon::Procedural {

class ICityGenerator {
public:
    virtual ~ICityGenerator() = default;
    
    struct CityParams {
        float citySize = 1000.0f;
        float roadDensity = 1.0f;
        float buildingDensity = 1.0f;
        uint32_t maxBuildings = 1000;
        bool generateInteriors = false;
        std::vector<std::string> buildingStyles;
    };
    
    virtual void SetParameters(const CityParams& params) = 0;
    virtual void Generate(const GenerationContext& context) = 0;
    virtual const std::vector<Building>& GetBuildings() const = 0;
    virtual const std::vector<Road>& GetRoads() const = 0;
};

class CityGenerator : public ICityGenerator {
public:
    explicit CityGenerator(const CityParams& params = {});
    
    void SetParameters(const CityParams& params) override;
    void Generate(const GenerationContext& context) override;
    const std::vector<Building>& GetBuildings() const override;
    const std::vector<Road>& GetRoads() const override;

private:
    CityParams m_Params;
    std::unique_ptr<IRoadNetwork> m_RoadNetwork;
    std::unique_ptr<IBuildingPlacer> m_BuildingPlacer;
    std::vector<Building> m_Buildings;
    std::vector<Road> m_Roads;
};

} // namespace Hydragon::Procedural 