/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PrismaRenderer.h
 * @brief Header file for the PrismaRenderer(c) class.
 * 
 * ARCHITECTURAL NOTES:
 * - PrismaRenderer(c) is a class that represents a renderer in the engine's node graph.
 * - It is used to define and manage rendering passes.
 * - It is subclassed from Renderer, which is a base class for all renderers. This will allow us a flexible approach for rendering, like we usually see in DCC apps, 
 * where we can easily switch between different renderers. Also better for prototyping new renderers, in a modular way. And possibly having a lower end renderer 
 * for mobile (Mobilight(c)), and next gen renderers (Prismax(c)).
 * 
 * @todo Declare-define all default member variables.
 * @todo Assign all member variables to port inputs inside of processNode(). Also computeResult(), and setOutputValue() for all outputs.
 * @todo Move implementation to .cpp file.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "VulkanCore.h"
#include "Renderer.h"
#include "NodeManager.h"

namespace hd {

struct PrismaRendererInfo : public RendererInfo {
    struct Shader {
        enum class Type {
            VERTEX,
            FRAGMENT,
            GEOMETRY,
            TESSELLATION,
            COMPUTE,
            RAY_TRACING,
            CUSTOM
        } type;

        enum class Model {
            PBR,
            LIT,
            UNLIT,
            CUSTOM
        } model;

        enum class Feature {
            WORLD_NORMAL,
            WORLD_POSITION,
            POSITION,
            TESSELATION,
            UV,
            TANGENT,
            BITANGENT,
            COLOR,
            OPACITY,
            EMISSIVE,
            NORMAL,
            ALBEDO,
            METALLIC,
            SPECULAR,
            ANISOTROPIC,
            AMBIENT_OCCLUSION,
            ROUGHNESS,
            CLEARCOAT,
            SUBSURFACE_SCATTERING,
            ANISOTROPY,
            TRANSMISSION,
            IOR,
            THIN_FILM,
            VOLUMETRIC,
            CUSTOM
        } features;
    };

    struct Texture {
        uint32_t width, height;

        enum class Format {
            RGBA,
            RGB,
            RG,
            R,
            CUSTOM
        } format;

        enum Dimensions {
            ONE,
            TWO,
            THREE,
            CUBE,
            CUSTOM
        } dimensions;

        enum class Channel {
            RED,
            GREEN,
            BLUE,
            ALPHA,
            CUSTOM
        } channel;

        enum class UV {
            UV0,
            UV1,
            UV2,
            UV3,
            UDIM,
            CUSTOM
        } uv;

        enum class Wrap {
            REPEAT,
            MIRROR,
            CLAMP,
            BORDER,
            CUSTOM
        } wrap;

        enum class Mip {
            NONE,
            AUTO,
            CUSTOM
        } mip;

        enum class Filter {
            NEAREST,
            LINEAR,
            CUBIC,
            TRILINEAR,
            ANISOTROPIC,
            CUSTOM
        } filter;

        enum class Method {
            TILED,
            MIPMAP,
            CUSTOM
        } method;

        enum class Type {
            ALBEDO,
            NORMAL,
            METALLIC,
            ROUGHNESS,
            AMBIENT_OCCLUSION,
            EMISSIVE,
            SPECULAR,
            ANISOTROPY,
            TRANSMISSION,
            IOR,
            THIN_FILM,
            VOLUMETRIC,
            CUSTOM
        } type;

        enum class Compression {
            NONE,
            DXT,
            BC,
            ASTC,
            PVRTC,
            ETC,
            CUSTOM
        } compression;
    };

    struct RenderPath {
        enum class Type {
            FORWARD,
            DEFERRED,
            TILED_DEFERRED,
            RAY_TRACING,
            HYBRID
        } type;
    };

    struct RenderPass {
        enum class Type {
            G_BUFFER,
            LIGHTING,
            POST_PROCESSING,
            SHADOWS,
            MULTI_SAMPLING,
            SSAO,
            SSR,
            BLOOM,
            TAA,
            FXAA,
            VOLUMETRIC_LIGHTING
        } type;
    };

    struct RenderMode {
        enum class Type {
            RENDER_TO_TEXTURE,
            RENDER_TO_SCREEN
        } type;
    };

    struct GBuffer {
        enum class Type {
            FORWARD,
            DEFERRED,
            TILED_DEFERRED,
            RAY_TRACING,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
    };

    struct Lighting {
        enum class Type {
            FORWARD,
            DEFERRED,
            TILED_DEFERRED,
            RAY_TRACING,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
    };

    struct Shadow {
        enum class Type {
            DEPTH_MAP,
            VIRTUAL_SHADOWS,
            RAY_TRACING,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
        bool contactShadows;
    };

    struct MultiSampling {
        enum class Type {
            MSAA,
            TAA,
            FXAA,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
    };

    struct SSAO {
        enum class Type {
            SCREEN_SPACE,
            VOLUMETRIC,
            RAY_TRACING,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
    };

    struct GI {
        enum class Type {
            PATH_TRACING,
            RAY_TRACING,
            VOLUMETRIC,
            SCREEN_SPACE,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
    };

    struct Reflection {
        enum class Type {
            SCREEN_SPACE,
            VOLUMETRIC,
            RAY_TRACING,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
    };
    
    struct Refraction {
        enum class Type {
            SCREEN_SPACE,
            VOLUMETRIC,
            RAY_TRACING,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
    };

    struct PostProcess {
        enum class Type {
            ANTIALIASING,
            DEPTH_OF_FIELD,
            LENS_DIRT,
            FILM_GRAIN,
            LENS_DISTORTION,
            CHROMATIC_ABERRATION,
            TONEMAPPING,
            COLOR_GRADING,
            MOTION_BLUR,
            SHARPENING,
            VIGNETTE,
            GI,
            SSAO,
            BLOOM,
            ANAMORPHIC_FLARES,
            SSR,
            VOLUMETRIC_LIGHTING,
            VOLUMETRIC_FOG,
            VOLUMETRIC_CLOUDS,
            UPSCALING,
            TOON_SHADING,
            HYBRID
        } type;
        uint32_t resolution;
        uint32_t sampleCount;
    };

    PrismaRendererInfo() {
        nodeType = "Rendering/PrismaRenderer";
        name = "PrismaRenderer";
        inputs = {
            "renderTargets",   // Render targets to render to
            "renderMode",      // Render mode (forward, deferred, etc.)
            "renderPath",      // Render path (forward, deferred, etc.)
            "renderPass",      // Render pass (g-buffer, lighting, etc.)
            "gBuffer",         // G-buffer settings
            "multiSampling",   // Multi-sampling settings
            "shader",          // Shader to use for rendering
            "texture",         // Texture to use for rendering
            "lighting",        // Lighting settings
            "shadow",          // Shadow settings
            "postProcess",     // Post-process settings
            "reflection",      // Reflection settings
            "refraction",      // Refraction settings
            "ssao",            // Screen-space ambient occlusion settings
            "gi",              // Global illumination settings
            "volumetrics",     // Volumetric settings
            "renderSettings"   // Render settings
        };
        outputs = {
            "renderedImage",   // Rendered image
            "renderMetrics"    // Render performance metrics
        };
    }
};

/**
 * @brief PrismaRenderer(c) class.
 * Modular Physically-based renderer optimized for visual quality and throughput.
 */
class PrismaRenderer : public Renderer {
public:
    // === Allocation, Initialization, Loading ===
    explicit PrismaRenderer(const PrismaRendererInfo& info = PrismaRendererInfo())
        : Renderer(info) { }    // Default constructor

    void initialize() override {
        // 1. Vulkan setup
        vulkanContext = std::make_unique<VulkanContext>();
        vulkanContext->validateFeatures(config.vulkanFeatures);
        
        // 2. Resource management
        resourceManager = std::make_unique<ResourceManager>(vulkanContext.get());
        
        // 3. Pipeline setup
        pipelineCache = std::make_unique<PipelineCache>(vulkanContext.get());
        descriptorManager = std::make_unique<DescriptorManager>();
        
        // 4. Render graph setup
        renderGraph = std::make_unique<RenderGraph>();
        setupRenderPasses();
        
        // 5. Post-process chain
        postProcess = std::make_unique<PostProcessChain>();
        setupDefaultPostProcess();
        
        // 6. Wave physics integration
        if (config.features.enableWavePhysicsIntegration) {
            initializeWavePhysics();
        }
        
        // 7. Other integrations
        if (config.features.enableOtherIntegrations) {
            initializeOtherIntegrations();
        }
    }

    void load() override {}

    PrismaRendererInfo config;

    void setupPipeline(const Config& config);

    // === Validation ===
    void validateGeneratedCode(const std::string& code) override;
    void enableVulkanValidation(bool enable);
    void enablePrismaValidation(bool enable);

    // === Caching & Optimization ===
    uint64_t computeCacheKey() const override;
    void generateRuntimeCode(CodeGenContext& context) override;
    void optimize() override;
    void invalidateCache() override;

    void cachePipeline(const PipelineConfig& config);

    // === Processing ===
    // Wave-based lighting calculations (WavePhysics integration)
    void computeWaveOptics(const WaveField& field, RenderParams& params) {
        // Convert EM waves to PBR parameters
        params.refraction = field.computeRefractionIndex();
        params.reflection = field.computeReflectionCoefficient();
        params.diffraction = field.computeDiffractionPattern();
    }
    
    // PBR Energy conservation (WavePhysics integration)
    void enforceEnergyConservation(const WaveField& field) {
        // Ensure PBR energy conservation aligns with wave physics
        field.validateEnergyTransfer();
    }

    void render() override {
        // 1. Update wave physics
        if (wavePhysics) {
            wavePhysics->update(deltaTime);
        }
        
        // 2. Execute render graph
        renderGraph->execute([this](RenderContext& context) {
            // G-buffer pass
            gBufferPass->execute(context);
            
            // Lighting pass
            lightingPass->execute(context);
            
            // Post-process chain
            postProcess->execute(context);
        });
    }

    void processNode() override {
        renderTargets = getInputValue<std::vector<RenderTarget>>("renderTargets");
        renderMode = getInputValue<RenderMode>("renderMode");
        renderPath = getInputValue<RenderPath>("renderPath");
        renderPass = getInputValue<RenderPass>("renderPass");
        gBuffer = getInputValue<GBuffer>("gBuffer");
        multiSampling = getInputValue<MultiSampling>("multiSampling");
        shader = getInputValue<Shader>("shader");
        texture = getInputValue<Texture>("texture");
        lighting = getInputValue<Lighting>("lighting");
        shadow = getInputValue<Shadow>("shadow");
        postProcess = getInputValue<PostProcess>("postProcess");
        reflection = getInputValue<Reflection>("reflection");
        refraction = getInputValue<Refraction>("refraction");
        ssao = getInputValue<SSAO>("ssao");
        gi = getInputValue<GI>("gi");
        volumetrics = getInputValue<Volumetrics>("volumetrics");
        renderSettings = getInputValue<RenderSettings>("renderSettings");
    }
    void update();



private:
    // Core systems
    std::unique_ptr<VulkanContext> vulkanContext;
    std::unique_ptr<RenderGraph> renderGraph;
    std::unique_ptr<ResourceManager> resourceManager;
    
    // Pipeline management
    std::unique_ptr<PipelineCache> pipelineCache;
    std::unique_ptr<DescriptorManager> descriptorManager;
    
    // Render passes
    std::unique_ptr<GBufferPass> gBufferPass;
    std::unique_ptr<LightingPass> lightingPass;
    std::unique_ptr<PostProcessChain> postProcess;
    
    // Wave physics integration
    std::shared_ptr<WavePhysics> wavePhysics;
    
    // Node management
    NodeManager nodeManager;
};

} // namespace hd
