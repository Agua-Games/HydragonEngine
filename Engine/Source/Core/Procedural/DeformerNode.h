/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HD_DeformerNode.h
 * @brief DeformerNode represents a deformer node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Deformer nodes are used to apply procedural deformations to objects.
 * 
 * TODO:
 * - Update the whole content to match the latest Object and Node design.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 */

#pragma once
#include "Core/NodeGraph/Node.h"
#include "Core/Procedural/HD_ProceduralTypes.h"

namespace hd {

struct HD_DeformerInfo : public NodeInfo {
    HD_DeformerInfo() {
        NodeType = "Procedural/Deformer";
        IsProcedural = true;
        
        // Generic deformer inputs
        Inputs = {
            "PrimitiveIn",    // Generic primitive input (mesh, volume, etc.)
            "Strength",       // Global deformation strength
            "Mask",          // Optional deformation mask
            "BlendMode",     // How to blend with previous deformations
            "NoiseParams",   // Procedural noise parameters
            "Structure",     // Procedural structure influence
            "TimeOffset"     // For time-based procedural deformations
        };
        
        Outputs = {
            "PrimitiveOut",   // Deformed primitive
            "DeformMetrics",  // Performance and quality metrics
            "ProceduralData", // Generated procedural data
            "CacheKey"        // For deformation caching
        };
    }
};

class HD_DeformerNode : public Node {
public:
    explicit HD_DeformerNode(const HD_DeformerInfo& info = HD_DeformerInfo())
        : Node(info) {}

    virtual void ProcessNodeGraph() override {
        // Cache validation
        auto cacheKey = GenerateCacheKey();
        if (TryLoadFromCache(cacheKey)) {
            return;
        }

        // Procedural generation
        auto noiseParams = GetInputValue<ProceduralNoiseParams>("NoiseParams");
        auto structure = GetInputValue<ProceduralStructureParams>("Structure");
        
        // Generate and apply procedural deformation
        auto proceduralData = GenerateProceduralDeformation(noiseParams, structure);
        
        // Apply final deformation
        ApplyDeformation(proceduralData);
        
        // Cache results
        CacheDeformation(cacheKey);
    }

    // Runtime optimization interface
    virtual void BakeForRuntime() {
        // Bake procedural parameters and cache for runtime
        RuntimeBakedData data;
        data.proceduralParams = SerializeProceduralParams();
        data.optimizedBuffers = PrecomputeDeformationData();
        StoreRuntimeData(std::move(data));
    }

protected:
    virtual void ApplyDeformation(const ProceduralDeformationData& data) = 0;

    struct RuntimeBakedData {
        std::vector<uint8_t> proceduralParams;
        std::vector<float> optimizedBuffers;
        uint32_t deformationFlags;
    };

private:
    std::string GenerateCacheKey();
    bool TryLoadFromCache(const std::string& key);
    void CacheDeformation(const std::string& key);
    ProceduralDeformationData GenerateProceduralDeformation(
        const ProceduralNoiseParams& noise,
        const ProceduralStructureParams& structure);
};

} // namespace hd