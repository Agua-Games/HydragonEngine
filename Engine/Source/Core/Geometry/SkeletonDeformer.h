/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SkeletonDeformer.h
 * @brief SkeletonDeformer represents a skeleton deformer node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Skeleton deformer nodes are used to represent and process skeletal animation data.
 * - To avoid confusion: SkeletonDeformer derives from VertexBlendDeformer, which is a base class for all deformers. VertexBlendDeformer is more general and 
 * can accept not only Skeleton hierarchy as input, but also other blendable data.
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
#include "VertexBlendDeformer.h"

namespace hd {

struct SkeletonDeformerInfo : public VertexBlendInfo {
    SkeletonDeformerInfo() {
        NodeType = "Geometry/SkeletonDeformer";
        
        inputs = {
            "JointTransforms",    // Array of joint transforms
            "JointParents",       // Parent hierarchy
            "BindPose",           // Reference pose
            "AnimationPose",      // Current animation state
            "AdditiveLayer",      // Optional additive animations
            "IKTargets"           // Optional IK constraints
        };
        
        outputs = {
            "SkeletonPose",       // Current computed pose
            "JointMatrices",      // Final transformation matrices
            "SkeletonMetrics"     // Performance and quality metrics
        };
    }
};

class SkeletonDeformer : public VertexBlendDeformer {
public:
    // === Allocation, Initialization, Loading ===
    explicit SkeletonDeformer(const SkeletonDeformerInfo& info = SkeletonDeformerInfo())
        : VertexBlendDeformer(info) {}

    // === Processing ===
    void processNodeGraph() override;
};

} // namespace hd