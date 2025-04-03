/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralRuleset.h
 * @brief Header file for the ProceduralRuleset class.
 * 
 * ARCHITECTURAL NOTES:
 * - ProceduralRuleset is a class that represents a ruleset for procedural generation in Hydragon.
 * - It is designed to be used with the Procedural class to generate procedural content.
 * - It works as a general-purpose procedural ruleset/expression which can beplaced in a node graph to setup a procedural content.
 * - It supports various procedural generation types, such as terrain, vegetation, buildings, etc.
 */
#pragma once
#include "Node.h"

namespace hd {

struct ProceduralRulesetInfo : public NodeInfo {
    ProceduralRulesetInfo() {
        NodeType = "Procedural/ProceduralRuleset";
        
        inputs = {
            "Rules",            // Array of procedural rules
            "RuleState",        // Current state of the rules
            "RuleSpeed",        // Speed of the rules
            "RuleLoop"          // Whether the rules should loop
        };
        
        outputs = {
            "RuleStatus",       // Status of the rules
            "RuleMetrics"       // Performance metrics of the rules
        };
    }
};

class ProceduralRuleset : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ProceduralRuleset(const ProceduralRulesetInfo& info = ProceduralRulesetInfo())
        : Node(info) {}
    initialize();
    load();
    
    // === Processing ===
    void processNode();
    void update();
    
    // === Cleanup ===
    void unload();
    void cleanup();
    ~ProceduralRuleset();
};

} // namespace hd