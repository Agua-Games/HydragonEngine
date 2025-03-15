/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MeshNode.h
 * @brief MeshNode represents a mesh node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Mesh nodes are used to represent and process mesh geometry.
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
#include "MeshTypes.h"
#include "MaterialNode.h"
#include "RuntimeVariants.h"
#include "TransformNode.h"
#include "ProceduralTypes.h"
#include "UsdSchema.h"
#include <glm/glm.hpp>

namespace hd {

struct MeshInfo : public NodeInfo {
    MeshInfo() {
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

class MeshNode : public Node {
public:
    explicit MeshNode(const MeshInfo& info = MeshInfo())
        : Node(info) {}

    void ProcessNodeGraph() override {
        // Process base transform
        auto transform = GetInputValue<std::shared_ptr<TransformNode>>("Transform");
        
        // Process deformers in sequence
        auto deformers = GetInputValue<std::vector<std::shared_ptr<DeformerNode>>>("Deformers");
        ProcessDeformerChain(deformers);
        
        // Apply material and finish processing
        auto material = GetInputValue<std::shared_ptr<MaterialNode>>("Material");
        ApplyMaterial(material);
        
        UpdateOutputs();
    }

private:
    void ProcessDeformerChain(const std::vector<std::shared_ptr<DeformerNode>>& deformers);
    void UpdateOutputs();
};

} // namespace hd
