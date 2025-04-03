/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file BuildingRuleset.h
 * @brief Header file for the BuildingRuleset class.
 * 
 * ARCHITECTURAL NOTES:
 * - BuildingRuleset is a class that represents a ruleset for procedural generation of buildings in Hydragon.
 * - It is designed to be used with the Building class to generate buildings procedurally.
 * - It inherits from ProceduralRuleset, which is a base class for all procedural rulesets, adding some building-specific features. The basic architecture of a
 * procedural building is:
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
 * - It works as a general-purpose procedural ruleset/expression which can be placed in a node graph to setup a building. Imagine the building as a node graph, with one 
 * to many ruleset nodes forming a tree (from the building's root node, Building, to the building's leaves, like windows, doors, etc.), then the Building ruleset node is
 * responsible for branching the tree, and the leaves are the building's nodes with visual representation, like windows, doors, also effects (eg.g a chimney's smoke).
 * - It supports procedural features, such as building generation, building growth, and building "death" (cracks, debris, crumbling). 
 */
#pragma once
#include "ProceduralRuleset.h"

namespace hd {

struct BuildingRulesetInfo : public ProceduralRulesetInfo {
    BuildingRulesetInfo() {
        nodeType = "Gameplay/BuildingRuleset";
        
        inputs = {
            "rules",           // Array of building rules
            "ruleState",       // Current state of the rules
            "ruleSpeed",       // Speed of the rules
            "ruleLoop"         // Whether the rules should loop
        };
        
        outputs = {
            "ruleStatus",      // Status of the rules
            "ruleMetrics"      // Performance metrics of the rules
        };
    }
};

class BuildingRuleset : public ProceduralRuleset {
public:
    // === Allocation, Initialization, Loading ===
    explicit BuildingRuleset(const BuildingRulesetInfo& info = BuildingRulesetInfo())
        : ProceduralRuleset(info) {}
    initialize();
    load();
    
    // === Processing ===
    void processNode();
    void update();
    
    // === Cleanup ===
    void unload();
    void cleanup();
    ~BuildingRuleset();
};

} // namespace hd