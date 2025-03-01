/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_VertexBlendDeformerNode.h
 * @brief VertexBlendDeformerNode represents a vertex blend deformer node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Vertex blend deformer nodes are used to blend between vertex positions.
 * 
 * TODO:
 * - Update the whole content to match the latest HD_Object and HD_Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 */

#pragma once
#include "HD_DeformerNode.h"

namespace hd {

struct HD_VertexBlendInfo : public HD_DeformerInfo {
    HD_VertexBlendInfo() {
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

class HD_VertexBlendDeformerNode : public HD_DeformerNode {
public:
    explicit HD_VertexBlendDeformerNode(const HD_VertexBlendInfo& info = HD_VertexBlendInfo())
        : HD_DeformerNode(info) {}

protected:
    void ApplyDeformation() override;
};

} // namespace hd