#pragma once
#include "Core/NodeGraph/HD_Node.h"
#include "Core/Procedural/HD_ProceduralTypes.h"

namespace hd {

struct HD_DeformerInfo : public HD_NodeInfo {
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

class HD_DeformerNode : public HD_Node {
public:
    explicit HD_DeformerNode(const HD_DeformerInfo& info = HD_DeformerInfo())
        : HD_Node(info) {}

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