/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_SkeletonNode.h
 * @brief SkeletonNode represents a skeleton node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Skeleton nodes are used to represent and process skeletal animation data.
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
#include "Node.h"

namespace hd {

struct HD_SkeletonInfo : public NodeInfo {
    HD_SkeletonInfo() {
        NodeType = "Geometry/Skeleton";
        
        Inputs = {
            "JointTransforms",    // Array of joint transforms
            "JointParents",       // Parent hierarchy
            "BindPose",           // Reference pose
            "AnimationPose",      // Current animation state
            "AdditiveLayer",      // Optional additive animations
            "IKTargets"           // Optional IK constraints
        };
        
        Outputs = {
            "SkeletonPose",       // Current computed pose
            "JointMatrices",      // Final transformation matrices
            "SkeletonMetrics"     // Performance and quality metrics
        };
    }
};

class HD_SkeletonNode : public Node {
public:
    explicit HD_SkeletonNode(const HD_SkeletonInfo& info = HD_SkeletonInfo())
        : Node(info) {}

    void ProcessNodeGraph() override;
};

} // namespace hd