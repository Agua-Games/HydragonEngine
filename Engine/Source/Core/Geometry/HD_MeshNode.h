/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Geometry/HD_MeshTypes.h"
#include "Core/Materials/HD_MaterialNode.h"
#include "Core/SceneGraph/HD_RuntimeVariants.h"
#include "Core/Transform/HD_TransformNode.h"
#include "Core/Procedural/HD_ProceduralTypes.h"
#include "Core/USD/HD_UsdSchema.h"
#include <glm/glm.hpp>

namespace hd {

struct HD_MeshInfo : public HD_NodeInfo {
    HD_MeshInfo() {
        NodeType = "Geometry/Mesh";
        
        Inputs = {
            "Transform",          // Base transform
            "Material",           // Material assignment
            "Deformers",         // Array of deformer nodes
            "LODMetric",         // LOD selection metric
            "GeometryModifier"   // Optional geometry modifications
        };
        
        Outputs = {
            "MeshData",          // Final processed geometry
            "BoundingBox",
            "LODLevel",
            "VisibilityMask",
            "DeformationState"   // Current deformation state
        };
    }
};

class HD_MeshNode : public HD_Node {
public:
    explicit HD_MeshNode(const HD_MeshInfo& info = HD_MeshInfo())
        : HD_Node(info) {}

    void ProcessNodeGraph() override {
        // Process base transform
        auto transform = GetInputValue<std::shared_ptr<HD_TransformNode>>("Transform");
        
        // Process deformers in sequence
        auto deformers = GetInputValue<std::vector<std::shared_ptr<HD_DeformerNode>>>("Deformers");
        ProcessDeformerChain(deformers);
        
        // Apply material and finish processing
        auto material = GetInputValue<std::shared_ptr<HD_MaterialNode>>("Material");
        ApplyMaterial(material);
        
        UpdateOutputs();
    }

private:
    void ProcessDeformerChain(const std::vector<std::shared_ptr<HD_DeformerNode>>& deformers);
    void UpdateOutputs();
};

} // namespace hd
