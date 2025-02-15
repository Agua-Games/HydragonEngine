#pragma once
#include "Core/NodeGraph/HD_Node.h"

namespace hd {

struct HD_DeformerInfo : public HD_NodeInfo {
    HD_DeformerInfo() {
        NodeType = "Geometry/Deformer";
        
        // Base deformer inputs
        Inputs = {
            "GeometryIn",     // Input geometry to deform
            "Strength",       // Global deformation strength
            "Mask",          // Optional deformation mask
            "BlendMode"      // How to blend with previous deformations
        };
        
        Outputs = {
            "GeometryOut",    // Deformed geometry
            "DeformMetrics"   // Performance and quality metrics
        };
    }
};

class HD_DeformerNode : public HD_Node {
public:
    explicit HD_DeformerNode(const HD_DeformerInfo& info = HD_DeformerInfo())
        : HD_Node(info) {}

    virtual void ProcessNodeGraph() override {
        // Base deformation processing
    }

protected:
    virtual void ApplyDeformation() = 0;
};

} // namespace hd