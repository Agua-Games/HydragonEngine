/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file MaterialNode.h
 * @brief MaterialNode represents a material node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Material nodes are used to define and manage material properties.
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

#include "Core/NodeGraph/Node.h"
#include "Core/Rendering/ShaderCache.h"
#include "Core/Rendering/RenderTypes.h"
#include "Core/SceneGraph/RuntimeVariants.h"
#include <spirv_reflect.h>
#include <vulkan/vulkan.h>
#include <pxr/usd/usdShade/material.h>
#include <glm/glm.hpp>
#include "Core/Procedural/ProceduralTypes.h"

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
struct MaterialInfo : public NodeInfo {
    MaterialDomain Domain = MaterialDomain::Surface;
    MaterialBlendMode BlendMode = MaterialBlendMode::Opaque;
    bool UsesProcedural = false;
    bool RequiresRayTracing = false;
    std::vector<MaterialVariantConfig> VariantConfigs;
    
    MaterialInfo() {
        NodeType = "Material";
        IsStreamable = true;
        IsAsyncLoadable = true;
    }
};

/**
 * @brief Advanced material node supporting USD, procedural generation, and variants
 */
class MaterialNode : public Node {
public:
    explicit MaterialNode(const MaterialInfo& info = MaterialInfo())
        : Node(info), MaterialInfo(info) {
        InitializeDefaultPorts();
        InitializeShaderCache();
    }

    // Node Graph Integration
    std::vector<std::string> getInputPorts() const override {
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

    std::vector<std::string> getOutputPorts() const override {
        return {"SurfaceOutput", "CustomData1", "CustomData2"};
    }

    // USD Integration
    void convertToUsdMaterial(pxr::UsdShadeMaterial& usdMaterial) {
        // Implementation for USD material conversion
    }

    void loadFromUsdMaterial(const pxr::UsdShadeMaterial& usdMaterial) {
        // Implementation for loading from USD material
    }

    // Shader Management
    struct ShaderVariant {
        VkShaderModule shaderModule;
        SpvReflectShaderModule reflection;
        std::vector<VkDescriptorSetLayout> descriptorLayouts;
    };

    void compileShaderVariants() {
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

    void bakeForRuntime() {
        // Bake material data for optimal runtime performance
    }

    // Procedural Generation
    void generateProceduralTextures() {
        if (!MaterialInfo.UsesProcedural) return;
        
        // Get structure algorithm parameters from input port
        ProceduralStructureParams structureParams;
        if (hasConnectedInput("StructureAlgorithm")) {
            structureParams = getInputValue<ProceduralStructureParams>("StructureAlgorithm");
        }

        // Apply structure algorithm to noise generation
        float noiseScale = getInputValue<float>("NoiseScale");
        int octaves = getInputValue<int>("NoiseOctaves");
        float persistence = getInputValue<float>("NoisePersistence");

        // Generate base noise with structure influence
        auto structuredNoise = [&](float x, float y, float z) {
            float baseNoise = 0.0f;
            
            switch (structureParams.type) {
                case ProceduralStructureType::Geometric:
                    baseNoise = generateGeometricNoise(x, y, z, structureParams);
                    break;
                case ProceduralStructureType::Voronoi:
                    baseNoise = generateVoronoiNoise(x, y, z, structureParams);
                    break;
                case ProceduralStructureType::Organic:
                    baseNoise = generateOrganicNoise(x, y, z, structureParams);
                    break;
                case ProceduralStructureType::Fractal:
                    baseNoise = generateFractalNoise(x, y, z, structureParams);
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
                float octaveNoise = generateStructuredOctave(
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
    void updateLODLevel(float performanceMetric) {
        for (size_t i = 0; i < MaterialInfo.VariantConfigs.size(); ++i) {
            if (performanceMetric <= MaterialInfo.VariantConfigs[i].performanceThreshold) {
                switchToVariant(i);
                break;
            }
        }
    }

protected:
    MaterialInfo MaterialInfo;
    std::unordered_map<uint32_t, ShaderVariant> ShaderVariants;
    RuntimeMaterialData RuntimeData;

    void initializeDefaultPorts() {
        // Setup default PBR ports
    }

    void initializeShaderCache() {
        // Initialize shader caching system
    }

    void compileVariant(const MaterialVariantConfig& config) {
        // Async shader compilation
        std::async(std::launch::async, [this, config]() {
            // Compile shader variant
            // Update shader cache
        });
    }

    void switchToVariant(uint32_t variantIndex) {
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
    
    void processShaderCompilationQueue() {
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
    float generateGeometricNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float generateVoronoiNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float generateOrganicNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float generateFractalNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float generateStructuredOctave(float x, float y, float z, const ProceduralStructureParams& params);
};

} // namespace hd
