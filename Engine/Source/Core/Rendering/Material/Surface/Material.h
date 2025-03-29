/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Material.h
 * @brief Material represents a material node in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - Material nodes are used to define and manage material properties.
 * - They can be used to create complex materials with procedural generation and variants.
 * - Multi-sample processing: uses MSAA as virtual high-resolution buffer:
 *      - Enables per-sample procedural patterns
 *      - Maintains quality without vertex count increase
 * - Procedural Patterns:
 *      - Fragment-level displacement and noise
 *      - Pattern-based detail enhancement
 *      - Harmony parameters for consistent results
 * - PBR Value Derivation:
 *      - Multiple source options (vertex color, low-res texture, procedural)
 *      - Rule-based channel generation
 *      - Gradient and pattern-based processing
 * - Memory Optimization:
 *      - Texture size reduction
 *      - MSAA buffer reuse
 *      - Efficient pattern storage
 * 
| Texture Type      | Traditional (MB) | Our Method (MB)  | Savings % | Notes                                           |
|-------------------|------------------|------------------|-----------|-------------------------------------------------|
| Albedo (BC7)      | 21.33            | 5.33             | 75%       | MSAA from vertex color + procedural enhancement |
| Normal (BC5)      | 16.00            | 4.00             | 75%       | Generated from height + pattern matching        |
| Roughness (BC4)   | 8.00             | 2.00             | 75%       | Derived from albedo + procedural rules          |
| Metallic (BC4)    | 8.00             | 2.00             | 75%       | Derived from albedo + material rules            |
| Height (R8)       | 8.00             | 2.00             | 75%       | Delta-encoded, adaptive precision               |
| AO (R8)           | 8.00             | 0.00             | 100%      | Fully procedural from geometry                  |
|-------------------|------------------|------------------|-----------|-------------------------------------------------|
| Total Per Mat.    | 69.33            | 15.33            | 77.9%     | Base 2K textures                                |
|-------------------|------------------|------------------|-----------|-------------------------------------------------|
| With Streaming    | 69.33            | 3.83             | 94.5%     | Using sparse binding + residency                |
| With Pattern DB   | 69.33            | 2.87             | 95.9%     | Shared pattern library                          |
| Max Compression   | 34.67            | 1.92             | 94.5%     | Using BC7/BC5/BC4 + pattern matching            |
|-------------------|------------------|------------------|-----------|-------------------------------------------------|
 *
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
#include <spirv_reflect.h>
#include <vulkan/vulkan.h>
#include <pxr/usd/usdShade/material.h>
#include <glm/glm.hpp>
#include "Node.h"
#include "MaterialTypes.h"
#include "PhysicsMaterial.h"
#include "ShaderCache.h"
#include "RenderTypes.h"
#include "RuntimeVariants.h"
#include "ProceduralTypes.h"

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
        NodeType = "Rendering/Material";
        isStreamable = true;
        isAsyncLoadable = true;

        inputs = {
            "BaseColor",
            "Metallic",
            "Roughness",
            "Normal",
            "Emissive",
            "Opacity",
            "DisplacementAmount",
            "AmbientOcclusion"
        };

        outputs = {
            "SurfaceOutput",
            "CustomData1",
            "CustomData2"
        };
    }
};

/**
 * @brief Advanced material node supporting USD, procedural generation, and variants
 */
class Material : public Node {
public:
    // === Structure Definitions ===
    // Shader Management
    struct ShaderVariant {
        VkShaderModule shaderModule;
        SpvReflectShaderModule reflection;
        std::vector<VkDescriptorSetLayout> descriptorLayouts;
    };

    // Runtime Optimization
    struct RuntimeMaterialData {
        VkPipeline pipeline;
        VkPipelineLayout pipelineLayout;
        std::vector<VkDescriptorSet> descriptorSets;
        uint32_t currentVariantIndex;
    };

    struct ProceduralPBRConfig {
        bool enableProceduralGeneration;
        bool useVertexColorSource;
        bool allowTextureReduction;

        struct Quality {
            float detailLevel;         // 0-1, affects pattern complexity
            float msaaSampleCount;     // Multi-sample level
            float patternDensity;      // Pattern frequency multiplier
        };

        struct MemoryOptimization {
            bool enableLowResTextures;
            uint32_t maxTextureSize;
            float qualityThreshold;    // Acceptable quality loss
        };
    };

    // === Allocation, Initialization, Loading ===
    explicit Material(const MaterialInfo& info = MaterialInfo())
        : Node(info), MaterialInfo(info) {
        InitializeDefaultPorts();
        InitializeShaderCache();
    }

    // USD Integration
    void convertToUsdMaterial(pxr::UsdShadeMaterial& usdMaterial) {
        // Implementation for USD material conversion
    }

    void loadFromUsdMaterial(const pxr::UsdShadeMaterial& usdMaterial) {
        // Implementation for loading from USD material
    }

    // === Port Management ===
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

    // === Procedural Generation ===
    void setupProceduralPBR(const ProceduralPBRConfig& config);

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

    void generatePBRMaps();

    // === Caching, Optimization ===
    uint64_t computeCacheKey() const override {
        // Compute cache key based on material properties
        return 0;
    }

    void bakeForRuntime() {
        // Bake material data for optimal runtime performance
    }

    void optimizeTextureMemory();

    // === Processing ===
    // Performance-based LOD
    void updateLODLevel(float performanceMetric) {
        for (size_t i = 0; i < MaterialInfo.VariantConfigs.size(); ++i) {
            if (performanceMetric <= MaterialInfo.VariantConfigs[i].performanceThreshold) {
                switchToVariant(i);
                break;
            }
        }
    }

    // === Compilation ===
    void compileShaderVariants() {
        for (const auto& config : MaterialInfo.VariantConfigs) {
            CompileVariant(config);
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
    // === Structure Definitions ===
    // Shader compilation and caching
    struct ShaderCompilationTask {
        std::string source;
        MaterialVariantConfig config;
        std::promise<ShaderVariant> result;
    };

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

    // === Allocation, Initialization, Loading ===
    std::queue<ShaderCompilationTask> CompilationQueue;
    std::thread CompilationThread;
    std::mutex CompilationMutex;
    
    // === Processing ===
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

    // Helper functions for different structure types
    float generateGeometricNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float generateVoronoiNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float generateOrganicNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float generateFractalNoise(float x, float y, float z, const ProceduralStructureParams& params);
    float generateStructuredOctave(float x, float y, float z, const ProceduralStructureParams& params);
};

} // namespace hd

