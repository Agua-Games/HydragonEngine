/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BuildingElement.h
 * @brief Header file for the BuildingElement class.
 * 
 * ARCHITECTURAL NOTES:
 * - BuildingElement is a class that represents an element of a building in Hydragon.
 * - It is used to represent any element of a building in the game world, such as rooms, floors, walls, etc.
 * - It is a base class for all building elements, used to fill the placement slots of a building. The basic architecture of a procedural building is:
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
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other building elements, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "Node.h"
#include "DataTable.h"

namespace hd {

struct BuildingElementInfo : public NodeInfo {
    BuildingElementInfo() {
        nodeType = "Gameplay/BuildingElement";
        
        inputs = {
            "model",        // Model of the building element
            "position",     // Position of the building element
            "rotation",     // Rotation of the building element
            "scale",        // Scale of the building element
            "material",     // Material of the building element
            "animation",    // Animation of the building element
            "physics",      // Physics of the building element
            "environment",  // Environment data
            "characterData",// Character data
            "buildingState" // Building state
        };
        
        outputs = {
            "buildingStatus",  // Building status
            "buildingMetrics"  // Building performance metrics
        };
    }
};

class BuildingElement : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit BuildingElement(const BuildingElementInfo& info = BuildingElementInfo())
        : Node(info) {}
    initialize();
    load();
    
    // === Processing ===
    void processNode();
    void update();
    
    // === Cleanup ===
    void unload();
    void cleanup();
    ~BuildingElement();
};

} // namespace hd