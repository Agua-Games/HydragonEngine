/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file VertexBlendDeformer.h
 * @brief VertexBlendDeformer represents a vertex blend deformer node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Vertex blend deformer nodes are used to blend between vertex positions.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */

#pragma once
#include "Deformer.h"

namespace hd {

struct VertexBlendInfo : public DeformerInfo {
    VertexBlendInfo() {
        NodeType = "Geometry/VertexBlendDeformer";
        
        // Add blend-specific inputs
        Inputs.insert(Inputs.end(), {
            "BlendTargets",      // Array of target positions
            "BlendWeights",      // Corresponding weights
            "BlendSpace",        // Local/World space blending
            "NormalizeWeights"   // Whether to normalize weights
        });
        
        Outputs.push_back("BlendMetrics");
    }
};

class VertexBlendDeformer : public Deformer {
public:
    explicit VertexBlendDeformer(const VertexBlendInfo& info = VertexBlendInfo())
        : Deformer(info) {}

protected:
    void applyDeformation() override;
};

} // namespace hd