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
| Albedo (BC7)      | 21.33            | 1.33             | 93.7%     | Vertex color + multi-level MSAA deltas          |
| Normal (BC5)      | 16.00            | 1.00             | 93.7%     | Height-derived + packed delta encoding          |
| Roughness (BC4)   | 8.00             | 0.50             | 93.7%     | Albedo-derived + 2-bit delta precision          |
| Metallic (BC4)    | 8.00             | 0.50             | 93.7%     | Albedo-derived + 2-bit delta precision          |
| Height (R8)       | 8.00             | 0.50             | 93.7%     | Progressive 4/2/1-bit delta encoding            |
| AO (R8)           | 8.00             | 0.00             | 100%      | Fully procedural from geometry                  |
|-------------------|------------------|------------------|-----------|-------------------------------------------------|
| Total Per Mat.    | 69.33            | 3.83             | 94.5%     | Base 2K textures                                |
|-------------------|------------------|------------------|-----------|-------------------------------------------------|
| With Streaming    | 69.33            | 0.96             | 98.6%     | Sparse binding + progressive loading            |
| With Pattern DB   | 69.33            | 0.72             | 99.0%     | Shared patterns + multi-level deltas            |
| Max Compression   | 34.67            | 0.48             | 98.6%     | BC7/BC5/BC4 + optimal delta packing             |
|-------------------|------------------|------------------|-----------|-------------------------------------------------|

Notes:
- Traditional: Standard PBR material with 2K textures
- Our Method: Multi-level delta encoding (4/2/1-bit per level)
- Base calculations for 2048x2048 textures
- Streaming assumes 25% resident textures
- Pattern DB with optimized delta storage
- Max compression leverages both BC formats and optimal delta packing
- Progressive precision reduction per MSAA level
- Memory calculations account for packed deltas (up to 8 values per byte)
 *
 * TODO:
 * - Introduce BC7 and other aggressive compression techniques (see table above to understand why).
 * - Study creative solution for procedurally generating much cheaper AO, or impression of AO. e.g. curvature + blur pass + very encoded and packed, 
 * simplified spherical harmonics, maybe only in screen space, or even rely only in the fullscreen SSAO pass.
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

    /*
    | Packing Scheme    | Bits/Delta | Max Delta | Values/Byte | Memory (4K tex) | Use Case                                    |
    |------------------|------------|-----------|-------------|----------------|---------------------------------------------|
    | Original (int8)  | 8 bits     | ±128      | 1           | 16 MB         | Too excessive for MSAA deltas               |
    | Quarter (2 bits) | 2 bits     | ±3        | 4           | 4 MB          | Perfect for final MSAA level                |
    | Octal (1 bit)    | 1 bit      | ±1        | 8           | 2 MB          | Ideal for fine detail levels                |
    | Packed-16 (4b)   | 4 bits     | ±15       | 2           | 8 MB          | Good for first MSAA level                   |

    Proposed Multi-Level Delta Encoding:
    | MSAA Level | Max Delta Needed | Bits Required | Samples/Byte |
    |------------|------------------|---------------|--------------|
    | Level 0->1 | ±15             | 4 bits        | 2            |
    | Level 1->2 | ±7              | 3 bits        | 2            |
    | Level 2->3 | ±3              | 2 bits        | 4            |
    | Level 3->4 | ±1              | 1 bit         | 8            |

    Memory Impact (4K texture):
    - Traditional: 16 MB (single int8 per delta)
    - Proposed: 3.75 MB (mixed precision levels)
    - Additional Savings: ~76.5%
    */

    struct MSAADelta {
        union {
            int8_t full_delta;     // Original format (deprecated)
            struct {
                uint8_t delta1: 4; // ±15 range (first level)
                uint8_t delta2: 4; // ±15 range (first level)
            } packed_16;
            struct {
                uint8_t delta1: 2; // ±3 range (fine level)
                uint8_t delta2: 2; // ±3 range (fine level)
                uint8_t delta3: 2; // ±3 range (fine level)
                uint8_t delta4: 2; // ±3 range (fine level)
            } packed_4;
            struct {
                uint8_t deltas: 8; // ±1 range (8 finest level deltas)
            } packed_8;
        };
        
        enum class DeltaPrecision {
            LEVEL1_4BIT,  // ±15 range
            LEVEL2_2BIT,  // ±3 range
            LEVEL3_1BIT   // ±1 range
        } precision;
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

