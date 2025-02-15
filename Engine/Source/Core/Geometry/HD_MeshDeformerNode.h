#pragma once
#include "Core/Procedural/HD_DeformerNode.h"
#include "Core/Geometry/HD_MeshTypes.h"

namespace hd {

struct HD_MeshDeformerInfo : public HD_DeformerInfo {
    HD_MeshDeformerInfo() {
        NodeType = "Geometry/MeshDeformer";
        
        // Add mesh-specific inputs
        Inputs.insert(Inputs.end(), {
            "VertexWeights",     // Per-vertex influence weights
            "DeformationSpace",  // Local/World space deformation
            "NormalRecalc",      // Normal recalculation method
            "TangentUpdate"      // Tangent space update strategy
        });
        
        // Add mesh-specific outputs
        Outputs.insert(Outputs.end(), {
            "DeformedVertices",
            "DeformedNormals",
            "DeformedTangents"
        });
    }
};

class HD_MeshDeformerNode : public HD_DeformerNode {
public:
    explicit HD_MeshDeformerNode(const HD_MeshDeformerInfo& info = HD_MeshDeformerInfo())
        : HD_DeformerNode(info) {}

    void BakeForRuntime() override {
        // Optimize for runtime performance
        RuntimeMeshData data;
        
        // Pre-compute and optimize vertex transformations
        data.vertexTransformLUT = PrecomputeVertexTransforms();
        
        // Pack weights and indices for SIMD
        data.packedWeights = PackWeightsForSIMD();
        
        // Generate optimized normal/tangent update tables
        data.normalUpdateTable = GenerateNormalUpdateLUT();
        
        StoreRuntimeData(std::move(data));
    }

protected:
    void ApplyDeformation(const ProceduralDeformationData& data) override {
        if (IsRuntimeBaked()) {
            ApplyBakedDeformation();
        } else {
            ApplyDynamicDeformation(data);
        }
    }

private:
    struct RuntimeMeshData : RuntimeBakedData {
        std::vector<glm::mat4> vertexTransformLUT;
        std::vector<float> packedWeights;
        std::vector<uint32_t> normalUpdateTable;
    };

    void ApplyBakedDeformation();
    void ApplyDynamicDeformation(const ProceduralDeformationData& data);
    std::vector<glm::mat4> PrecomputeVertexTransforms();
    std::vector<float> PackWeightsForSIMD();
    std::vector<uint32_t> GenerateNormalUpdateLUT();
};

} // namespace hd