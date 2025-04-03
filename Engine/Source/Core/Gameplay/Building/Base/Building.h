/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Building.h
 * @brief Header file for the Building class.
 * 
 * ARCHITECTURAL NOTES:
 * - Building is a class that represents a building in Hydragon.
 * - It is used to represent any building in the game world, such as houses, offices, factories, etc.
 * - It's a base class for all buildings, used to fill the placement slots of a settlement, for example.
 * - It's meant to be used as the first node (the "root" node) in a procedural building graph, as it has the specific features (properties, functionality) of a building
 * category of buildings (bridge, fortress, etc). These properties can be passed down the graph, to govern and be used by the other nodes.
 * - We have specialized convenience nodes for each category of buildings, like Bridge, Fortress, etc.
 * - The basic architecture of a procedural building is:
 * 
 *      Building -->BuildingRuleset -> BuildingElement (array) -> BuildingRuleset -> BuildingElement (array) -> BuildingRuleset -> ...
 * 
 * So, the ruleset is responsible for the placement/distribution rules. And the building elements are responsible for filling the placement slots with the actual visual 
 * representation of the building (doors, windows, etc.). We also have convenience specialized elements derived from BuildingElement, like Room, Staircase, etc.
 * For representing, for example, a windmill:
 * 
 *      Mill --> BuildingRuleset (radial placement) -> Walls -> BuildingRuleset (vertical placement) -> Floors -> BuildingRuleset (radial placement) -> Fan
 *      (of course, the mill would have other ruleset branches, for the door in the first floor, the chimney, roof, Setc.)
 * 
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other buildings, environment, character, etc.
 * 
 * @todo Make sure all proper building categories inherit from Building, instead of Node.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct BuildingInfo : public NodeInfo {
    struct Section {
        std::string name;
        float height;
        // Other section properties
    };

    BuildingInfo() {
        nodeType = "Gameplay/Building";
        
        inputs = {
            "model",        // Model of the building
            "position",     // Position of the building
            "rotation",     // Rotation of the building
            "scale",        // Scale of the building
            "material",     // Material of the building
            "animation",    // Animation of the building
            "physics",      // Physics of the building
            "collision",    // Collision of the building
            "script"        // Script of the building
        };
        
        outputs = {
            "model",        // Model of the building
            "position",     // Position of the building
            "rotation",     // Rotation of the building
            "scale",        // Scale of the building
            "material",     // Material of the building
            "animation",    // Animation of the building
            "physics",      // Physics of the building
            "collision",    // Collision of the building
            "script"        // Script of the building
        };
    }
};

class Building : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Building(const BuildingInfo& info = BuildingInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
 
    }
    void addSection(const std::string& sectionName);
    void removeSection(const std::string& sectionName);
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Building() = default;     // Default destructor
};

} // namespace hd

