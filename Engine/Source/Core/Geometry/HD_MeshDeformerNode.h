/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_MeshDeformerNode.h
 * @brief MeshDeformerNode represents a mesh deformer node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Mesh deformer nodes are used to apply deformations to mesh geometry.
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
#include "HD_MeshTypes.h"

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