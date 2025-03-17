/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MeshDeformerNode.h
 * @brief MeshDeformerNode represents a mesh deformer node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Mesh deformer nodes are used to apply deformations to mesh geometry.
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
#include "DeformerNode.h"
#include "MeshTypes.h"

namespace hd {

struct MeshDeformerInfo : public DeformerInfo {
    MeshDeformerInfo() {
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

class MeshDeformerNode : public DeformerNode {
public:
    explicit MeshDeformerNode(const MeshDeformerInfo& info = MeshDeformerInfo())
        : DeformerNode(info) {}

    void bakeForRuntime() override {
        // Optimize for runtime performance
        RuntimeMeshData data;
        
        // Pre-compute and optimize vertex transformations
        data.vertexTransformLUT = precomputeVertexTransforms();
        
        // Pack weights and indices for SIMD
        data.packedWeights = packWeightsForSIMD();
        
        // Generate optimized normal/tangent update tables
        data.normalUpdateTable = generateNormalUpdateLUT();
        
        storeRuntimeData(std::move(data));
    }

protected:
    void applyDeformation(const ProceduralDeformationData& data) override {
        if (isRuntimeBaked()) {
            applyBakedDeformation();
        } else {
            applyDynamicDeformation(data);
        }
    }

private:
    struct RuntimeMeshData : RuntimeBakedData {
        std::vector<glm::mat4> vertexTransformLUT;
        std::vector<float> packedWeights;
        std::vector<uint32_t> normalUpdateTable;
    };

    void applyBakedDeformation();
    void applyDynamicDeformation(const ProceduralDeformationData& data);
    std::vector<glm::mat4> precomputeVertexTransforms();
    std::vector<float> packWeightsForSIMD();
    std::vector<uint32_t> generateNormalUpdateLUT();
};

} // namespace hd