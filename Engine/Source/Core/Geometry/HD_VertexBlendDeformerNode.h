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