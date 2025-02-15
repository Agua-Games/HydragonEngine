/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_Node.h"
#include "Core/Rendering/HD_ShaderCache.h"
#include "Core/Rendering/HD_RenderTypes.h"
#include "Core/SceneGraph/HD_RuntimeVariants.h"
#include <spirv_reflect.h>
#include <vulkan/vulkan.h>
#include <pxr/usd/usdShade/material.h>
#include <glm/glm.hpp>
#include "Core/Procedural/HD_ProceduralTypes.h"

namespace hd {

enum class MaterialDomain {
    Surface,        // Standard surface shading
    PostProcess,    // Full-screen effects
    Volumetric,    // Volume rendering
    Compute,        // Compute shaders (e.g., for procedural generation)
    RayTracing     // Ray tracing shaders
};

enum class MaterialBlendMode {
    Opaque,
    Masked,
    Translucent,
    Additive,
    Modulate
};

struct MaterialVariantConfig {
    float performanceThreshold;
    uint32_t maxTextureResolution;
    bool enableProcedural;
    uint32_t shaderComplexityLevel;
    std::vector<std::string> enabledFeatures;
};

/**
 * @brief Enhanced material node info with USD compatibility
 */
struct HD_MaterialInfo : public HD_NodeInfo {
    MaterialDomain Domain = MaterialDomain::Surface;
    MaterialBlendMode BlendMode = MaterialBlendMode::Opaque;
    bool UsesProcedural = false;
    bool RequiresRayTracing = false;
    std::vector<MaterialVariantConfig> VariantConfigs;
    
    HD_MaterialInfo() {
        NodeType = "Material";
        IsStreamable = true;
        IsAsyncLoadable = true;
    }
};

/**
 * @brief Advanced material node supporting USD, procedural generation, and variants
 */
class HD_MaterialNode : public HD_Node {
public:
    explicit HD_MaterialNode(const HD_MaterialInfo& info = HD_MaterialInfo())
        : HD_Node(info), MaterialInfo(info) {
        InitializeDefaultPorts();
        InitializeShaderCache();
    }

    // Node Graph Integration
    std::vector<std::string> GetInputPorts() const override {
        std::vector<std::string> ports = {
            "BaseColor",
            "Metallic",
            "Roughness",
            "Normal",
            "Emissive",
            "Opacity",
            "DisplacementAmount",
            "AmbientOcclusion"
        };
        // Add procedural inputs if enabled
        if (MaterialInfo.UsesProcedural) {
            ports.insert(ports.end(), {
                "NoiseScale",
                "NoiseOctaves",
                "NoisePersistence",
                "StructureAlgorithm"  // New port for procedural structure
            });
        }
        return ports;
    }

    std::vector<std::string> GetOutputPorts() const override {
        return {"SurfaceOutput", "CustomData1", "CustomData2"};
    }

    // USD Integration
    void ConvertToUsdMaterial(pxr::UsdShadeMaterial& usdMaterial) {
        // Implementation for USD material conversion
    }

    void LoadFromUsdMaterial(const pxr::UsdShadeMaterial& usdMaterial) {
        // Implementation for loading from USD material
    }

    // Shader Management
    struct ShaderVariant {
        VkShaderModule shaderModule;
        SpvReflectShaderModule reflection;
        std::vector<VkDescriptorSetLayout> descriptorLayouts;
    };

    void CompileShaderVariants() {
        for (const auto& config : MaterialInfo.VariantConfigs) {
            CompileVariant(config);
        }
    }

    // Runtime Optimization
    struct RuntimeMaterialData {
        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkDescriptorSet> descriptorSets;
        uint32_t currentVariantIndex;
    };

    void BakeForRuntime() {
        // Bake material data for optimal runtime performance
    }

    // Procedural Generation
    void GenerateProceduralTextures() {
        if (!MaterialInfo.UsesProcedural) return;
        
        // Get structure algorithm parameters from input port
        ProceduralStructureParams structureParams;
        if (HasConnectedInput("StructureAlgorithm")) {
            structureParams = GetInputValue<ProceduralStructureParams>("StructureAlgorithm");
        }

        // Apply structure algorithm to noise generation
        float noiseScale = GetInputValue<float>("NoiseScale");
        int octaves = GetInputValue<int>("NoiseOctaves");
        float persistence = GetInputValue<float>("NoisePersistence");

        // Generate base noise with structure influence
        auto structuredNoise = [&](float x, float y, float z) {
            float baseNoise = 0.0f;
            
            switch (structureParams.type) {
                case ProceduralStructureType::Geometric:
                    baseNoise = GenerateGeometricNoise(x, y, z, structureParams);
                    break;
                case ProceduralStructureType::Voronoi:
                    baseNoise = GenerateVoronoiNoise(x, y, z, structureParams);
                    break;
                case ProceduralStructureType::Organic:
                    baseNoise = GenerateOrganicNoise(x, y, z, structureParams);
                    break;
                case ProceduralStructureType::Fractal:
                    baseNoise = GenerateFractalNoise(x, y, z, structureParams);
                    break;
                case ProceduralStructureType::Custom:
                    if (structureParams.customAlgorithm) {
                        baseNoise = structureParams.customAlgorithm(x, y, z);
                    }
                    break;
            }

            // Apply octaves with structure influence
            float amplitude = 1.0f;
            float frequency = 1.0f;
            float maxValue = 0.0f;
            float result = baseNoise;

            for (int i = 1; i < octaves; ++i) {
                frequency *= 2.0f;
                amplitude *= persistence;
                maxValue += amplitude;
                
                // Blend structure with octaves
                float octaveNoise = GenerateStructuredOctave(
                    x * frequency, 
                    y * frequency, 
                    z * frequency, 
                    structureParams
                );
                
                result += octaveNoise * amplitude;
            }

            return result / maxValue;
        };

        // Use the structured noise in texture generation
        // ... rest of texture generation code
    }

    // Performance-based LOD
    void UpdateLODLevel(float performanceMetric) {
        for (size_t i = 0; i < MaterialInfo.VariantConfigs.size(); ++i) {
            if (performanceMetric <= MaterialInfo.VariantConfigs[i].performanceThreshold) {
                SwitchToVariant(i);
                break;
            }
        }
    }

protected:
    HD_MaterialInfo MaterialInfo;
    std::unordered_map<uint32_t, ShaderVariant> ShaderVariants;
    RuntimeMaterialData RuntimeData;

    void InitializeDefaultPorts() {
        // Setup default PBR ports
    }

    void InitializeShaderCache() {
        // Initialize shader caching system
    }

    void CompileVariant(const MaterialVariantConfig& config) {
        // Async shader compilation
        std::async(std::launch::async, [this, config]() {
            // Compile shader variant
            // Update shader cache
        });
    }

    void SwitchToVariant(uint32_t variantIndex) {
        // Implementation for runtime variant switching
    }

private:
    // Shader compilation and caching
    struct ShaderCompilationTask {
        std::string source;
        MaterialVariantConfig config;
        std::promise<ShaderVariant> result;
    };

    std::queue<ShaderCompilationTask> CompilationQueue;
    std::thread CompilationThread;
    std::mutex CompilationMutex;
    
    void ProcessShaderCompilationQueue() {
        while (true) {
            ShaderCompilationTask task;
            {
                std::lock_guard<std::mutex> lock(CompilationMutex);
                if (CompilationQueue.empty()) break;
                task = std::move(CompilationQueue.front());
                CompilationQueue.pop();
            }
            // Compile shader and set result
        }
    }

    // Add enum to define structure generation types
    enum class ProceduralStructureType {
        Geometric,      // Regular patterns (grid, hexagonal, etc.)
        Voronoi,        // Cell-based patterns
        Organic,        // Natural, irregular patterns
        Fractal,        // Self-similar patterns
        Custom          // User-defined algorithm
    };

    struct ProceduralStructureParams {
        ProceduralStructureType type = ProceduralStructureType::Organic;
        float regularity = 0.5f;     // 0 = chaotic, 1 = regular
        float complexity = 0.5f;     // Influences pattern detail level
        float variation = 0.5f;      // Pattern variation amount
        std::function<float(float, float, float)> customAlgorithm; // For custom patterns
    };

    // Helper functions for different structure types
    float GenerateGeometricNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float GenerateVoronoiNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float GenerateOrganicNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float GenerateFractalNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float GenerateStructuredOctave(float x, float y, float z, const ProceduralStructureParams& params);
};

} // namespace hd
