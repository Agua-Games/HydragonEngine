/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Annotation.h
 * @brief Header file for the Annotation class.
 * 
 * ARCHITECTURAL NOTES:
 * - Annotation is a class that represents an annotation in Hydragon.
 * - It is used to represent any annotation in the game world, such as comments, notes, etc.
 * - It supports interactive features, such as editing, formatting, and styling.
 * - It also supports two-way messaging with other annotations, environment, character, etc.
 */
#pragma once
#include "Engine.h"
#include "Node.h"
#include "DataTable.h"
#include <string>
#include <vector>
#include <map>

namespace hd {

struct AnnotationInfo : public NodeInfo {
    AnnotationInfo() {
        NodeType = "Utilities/Annotation";
        
        inputs = {
            "AnnotationData",  // Annotation data
            "Environment",     // Environment data
            "Character",       // Character data
            "AnnotationState"  // Annotation state
        };
        
        outputs = {
            "AnnotationStatus",  // Annotation status
            "AnnotationMetrics"  // Annotation performance metrics
        };
    }
};

class Annotation : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit Annotation(const AnnotationInfo& info = AnnotationInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}

    // Set default values
    AnnotationInfo info;
    DataTable dataTable;

    // === Processing ===
    void processNode() override {
    
    }
    void processAnnotation();
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~Annotation() = default;     // Default destructor
};

} // namespace hd
