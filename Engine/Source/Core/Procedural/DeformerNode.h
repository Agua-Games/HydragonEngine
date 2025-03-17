/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DeformerNode.h
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
#include "Core/Procedural/ProceduralTypes.h"

namespace hd {

struct DeformerInfo : public NodeInfo {
    DeformerInfo() {
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

class DeformerNode : public Node {
public:
    explicit DeformerNode(const DeformerInfo& info = DeformerInfo())
        : Node(info) {}

    virtual void processNodeGraph() override {
        // Cache validation
        auto cacheKey = generateCacheKey();
        if (tryLoadFromCache(cacheKey)) {
            return;
        }

        // Procedural generation
        auto noiseParams = getInputValue<ProceduralNoiseParams>("NoiseParams");
        auto structure = getInputValue<ProceduralStructureParams>("Structure");
        
        // Generate and apply procedural deformation
        auto proceduralData =generateProceduralDeformation(noiseParams, structure);
        
        // Apply final deformation
        applyDeformation(proceduralData);
        
        // Cache results
        cacheDeformation(cacheKey);
    }

    // Runtime optimization interface
    virtual void bakeForRuntime() {
        // Bake procedural parameters and cache for runtime
        RuntimeBakedData data;
        data.proceduralParams = serializeProceduralParams();
        data.optimizedBuffers = precomputeDeformationData();
        storeRuntimeData(std::move(data));
    }

protected:
    virtual void applyDeformation(const ProceduralDeformationData& data) = 0;

    struct RuntimeBakedData {
        std::vector<uint8_t> proceduralParams;
        std::vector<float> optimizedBuffers;
        uint32_t deformationFlags;
    };

private:
    std::string generateCacheKey();
    bool tryLoadFromCache(const std::string& key);
    void cacheDeformation(const std::string& key);
    ProceduralDeformationData generateProceduralDeformation(
        const ProceduralNoiseParams& noise,
        const ProceduralStructureParams& structure);
};

} // namespace hd