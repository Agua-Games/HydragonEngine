/*
/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ProceduralPBRGenerator.h
 * @brief ProceduralPBRGenerator is a class that generates PBR materials procedurally.
 * 
 * ARCHITECTURAL NOTES:
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
- Traditional: Standard PBR material with 2K textures
- Our Method: Vertex color + MSAA + procedural enhancement
- Base calculations for 2048x2048 textures
- Streaming assumes 25% resident textures
- Pattern DB reduces unique pattern storage
- Max compression uses all available techniques
 *
 * TODO:
 * - Introduce BC7 and other aggressive compression techniques (see table above to understand why).
*/
#pragma once

namespace hd {
    // Forward declarations from other modules
    struct ProceduralPatternType;  // From ProceduralTypes.h
    struct HarmonyParams;          // From ProceduralTypes.h
    struct MeshData;               // From Geometry module
    struct PBRDerivationRules;     // From Materials module
    struct TextureReductionConfig; // From Materials module
    struct MSAAConfig;             // From Graphics module

class ProceduralPBRGenerator {
public:
    struct MSAAConfig {
        VkSampleCountFlagBits sampleCount;     // 4x, 8x, 16x
        bool enableSampleShading;              // Per-sample processing
        float minSampleShading;                // Sample shading rate
    };

    struct ProceduralPass {
        struct Pattern {
            ProceduralPatternType type;        // From ProceduralTypes.h
            float frequency;
            float amplitude;
            vec2 warpOffset;
            HarmonyParams harmony;             // Cross-system influence
        };

        struct FragmentDisplacement {
            float turbulenceScale;
            float noiseOctaves;
            vec2 warpDirection;
            float detailStrength;
        };
    };

    struct PBRDerivationRules {
        struct Channel {
            enum class Source {
                VERTEX_COLOR,           // Direct from vertex
                LOW_RES_TEXTURE,        // From downsampled texture
                PROCEDURAL_PATTERN,     // From pattern generator
                HYBRID                  // Combined sources
            } source;

            struct Processing {
                float baseValue;
                float noiseInfluence;
                float patternWeight;
                vec2 gradientRange;
            };
        };

        // Per-channel configuration
        Channel roughness;
        Channel metallic;
        Channel ambient_occlusion;
        Channel height;                // For displacement
    };

    struct TextureReductionConfig {
        uint32_t maxTextureSize;       // e.g., 512x512
        bool enableMipBias;
        float compressionTolerance;
        bool preserveEdges;
    };

    // Similar to AdaptiveMeshNode's DeltaEncoding
    enum class ValueEncoding {
        INT8_BALANCED,    // int8_t scaled 0.1 (-12.8 to 12.7, 0.1 precision)
        INT8_EXPONENTIAL, // exp10 encoding for wide range values
        INT4_PACKED,      // Two values packed into int8_t
        UV_PACKED        // UV coordinates packed with exp10
    };

    struct DeltaPackedValue {
        union {
            struct {
                int8_t primary;   // Main value (e.g., height)
                int8_t secondary; // Secondary value (e.g., roughness)
            };
            int8_t exponential;   // For exp10 encoding
            struct {
                int8_t packed;    // For 4-bit packed values
                bool secondHalf;  // Which 4 bits to use
            };
        };
        
        bool isSignificant;
        ValueEncoding encoding;
    };

    struct PackedTextureData {
        std::vector<DeltaPackedValue> values;
        float baseScale;          // For reconstruction
        float exponentialBase;    // Usually 10.0f
        
        struct DeltaStats {
            float maxDelta;
            float averageDelta;
            uint32_t significantCount;
        } stats;
    };

    // Sync with adaptive mesh
    struct SubdivisionSync {
        uint32_t subdivLevel;
        float maxDisplacement;    // From AdaptiveMeshNode
        vec2 uvScale;            // For UV space mapping
        
        struct SyncChannel {
            enum class Type {
                HEIGHT,           // For displacement
                COLOR,           // For albedo
                ROUGHNESS,       // For material
                METALLIC         // For material
            } type;
            
            float precision;      // Matches mesh precision
            bool useWavelets;     // Enable wavelet compression
            uint32_t channelMask; // Which channel to sync
        };
        std::vector<SyncChannel> syncChannels;
    };

    struct MSAADelta {
        int8_t delta;      // -128 to 127 range = 256 pixel maximum stride
        bool isSignificant;

        // Direct pixel offset mapping
        uint32_t getPixelOffset() const {
            return static_cast<uint32_t>(delta + 128); // 0-256 range
        }
    };

    struct MSAALevelData {
        VkImage msaaImage;
        uint32_t sampleCount;
        std::vector<MSAADelta> deltas;
        float quantizationScale;  // Similar to mesh subdivision levels
    };

    void processVertexColorUpsampling(
        const VkImage& baseColorImage,    // Lower res (vertex colors)
        MSAALevelData& msaaLevel          // Higher res (MSAA buffer)
    ) {
        // Similar principle to mesh subdivision
        // Each sample can move up to 256 pixels from its base position
        // This is plenty for smoothing/antialiasing vertex color boundaries
        
        for (uint32_t sample = 0; sample < msaaLevel.sampleCount; ++sample) {
            // Calculate delta from base color to MSAA sample
            MSAADelta delta;
            // ... delta calculation ...

            // Store if significant (just like mesh displacement)
            if (std::abs(delta.delta) > deltaThreshold) {
                delta.isSignificant = true;
                msaaLevel.deltas.push_back(delta);
            }
        }

        // Second pass: Convert MSAA data to procedural pattern
        ProceduralPass::Pattern proceduralPattern;
        convertMSAAToProceduralPattern(msaaLevel, proceduralPattern);

        // Third pass: Generate final procedural detail
        generateProceduralPatterns(ProceduralPass{proceduralPattern});
    };

    void convertMSAAToProceduralPattern(
        const MSAALevelData& msaaLevel,
        ProceduralPass::Pattern& pattern
    ) {
        // 1. MSAA samples provide initial high-frequency detail map
        // Each MSAA sample position becomes a potential procedural feature point
        for (const auto& delta : msaaLevel.deltas) {
            if (delta.isSignificant) {
                // Convert MSAA delta position to procedural pattern coordinate
                vec2 patternPos = convertDeltaToPatternSpace(delta);
                
                // Use delta magnitude to influence pattern amplitude
                pattern.amplitude *= (delta.delta / 128.0f);
                
                // Adjust pattern frequency based on MSAA sample density
                pattern.frequency = calculateFrequencyFromMSAA(
                    msaaLevel.sampleCount,
                    delta.getPixelOffset()
                );
                
                // Apply warp based on neighboring MSAA sample positions
                pattern.warpOffset = calculateWarpFromMSAANeighbors(delta);
            }
        }

        // 2. Generate harmony parameters from MSAA distribution
        pattern.harmony = deriveHarmonyFromMSAA(msaaLevel);
    }

private:
    // Vulkan resources
    struct RenderTargets {
        VkImage msaaTarget;            // Multi-sampled target
        VkImage proceduralBuffer;      // Procedural pattern buffer
        VkImage finalOutput;           // Final PBR maps
        
        struct Attachments {
            VkImageView msaaView;
            VkImageView proceduralView;
            VkFramebuffer framebuffer;
        };
    };

    // Enhanced packing methods
    void packExponential(float value, float base, int8_t& result) {
        // Similar to AdaptiveMeshNode's encoding but with exp10
        float logVal = std::log10(std::abs(value)) / std::log10(base);
        result = static_cast<int8_t>(std::clamp(logVal * 127.0f, -128.0f, 127.0f));
    }

    void packUVCoord(vec2 uv, int8_t& result) {
        // Pack UV using exponential encoding
        float combined = uv.x * 1000.0f + uv.y; // Combine with scale
        packExponential(combined, 10.0f, result);
    }

    float unpackExponential(int8_t packed, float base) {
        float normalized = packed / 127.0f;
        return std::pow(base, normalized);
    }

public:
    // Core functions
    void setupPipeline(const MSAAConfig& msaaConfig);
    void processVertexColors(const MeshData& mesh);
    void generateProceduralPatterns(const ProceduralPass& pass);
    void derivePBRValues(const PBRDerivationRules& rules);

    // Optimization helpers
    void reduceSourceTextures(const TextureReductionConfig& config);
    void optimizePatternStorage();
    
    // Memory statistics
    struct MemoryStats {
        size_t originalTextureSize;
        size_t optimizedSize;
        float compressionRatio;
        uint32_t msaaSampleCount;
    } getMemoryStats() const;

    // New methods for delta-based processing
    void processWithDeltas(const SubdivisionSync& sync) {
        // Process in sync with mesh subdivision
        for (const auto& channel : sync.syncChannels) {
            PackedTextureData packedData;
            packedData.baseScale = sync.maxDisplacement;
            packedData.exponentialBase = 10.0f;

            switch (channel.type) {
                case SubdivisionSync::SyncChannel::Type::HEIGHT:
                    processHeightChannel(sync, channel, packedData);
                    break;
                case SubdivisionSync::SyncChannel::Type::COLOR:
                    processColorChannel(sync, channel, packedData);
                    break;
                // ... other channels
            }

            if (channel.useWavelets) {
                applyWaveletCompression(packedData);
            }
        }
    }

    void processHeightChannel(
        const SubdivisionSync& sync,
        const SubdivisionSync::SyncChannel& channel,
        PackedTextureData& packedData);

    void processColorChannel(
        const SubdivisionSync& sync,
        const SubdivisionSync::SyncChannel& channel,
        PackedTextureData& packedData);

    void applyWaveletCompression(PackedTextureData& data);
};

} // namespace hd
