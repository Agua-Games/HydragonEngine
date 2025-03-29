/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ReflectionVector.h
 * @brief Header file for the ReflectionVector class.
 * 
 * ARCHITECTURAL NOTES:
 * - ReflectionVector is a class that represents a reflection vector in Hydragon.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct ReflectionVectorInfo : public NodeInfo {
    ReflectionVectorInfo() {
        NodeType = "Math/ReflectionVector";
        
        inputs = {
            "IncidentVector",  // Incident vector
            "NormalVector"    // Normal vector
        };
        
        outputs = {
            "ReflectionVector" // Reflection vector
        };
    }
};

class ReflectionVector : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit ReflectionVector(const ReflectionVectorInfo& info = ReflectionVectorInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    ReflectionVectorInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processReflectionVector();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ReflectionVector() = default;     // Default destructor
};

} // namespace hd