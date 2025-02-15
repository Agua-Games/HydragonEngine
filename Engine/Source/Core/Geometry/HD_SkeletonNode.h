#pragma once
#include "Core/NodeGraph/HD_Node.h"

namespace hd {

struct HD_SkeletonInfo : public HD_NodeInfo {
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

class HD_SkeletonNode : public HD_Node {
public:
    explicit HD_SkeletonNode(const HD_SkeletonInfo& info = HD_SkeletonInfo())
        : HD_Node(info) {}

    void ProcessNodeGraph() override;
};

} // namespace hd