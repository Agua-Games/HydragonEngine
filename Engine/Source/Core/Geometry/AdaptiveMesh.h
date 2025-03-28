/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AdaptiveMesh.h
 * @brief AdaptiveMesh(c) is a mesh node which uses subdivision and adaptive compression.
 * It results in subdivision meshes which support high polygon counts and details and which are much smaller (in memory and disk) than their original counterparts, 
 * while maintaining high visual fidelity.
 * 
 * ARCHITECTURAL NOTES:
 * - Advanced multi-resolution system with hierarchical subdivision
 * - Smart delta encoding with multiple schemes
 * - Progressive precision adjustment
 * - Delta-based vertex storage with sparse optimization (for unchanged vertices)
 * - Adaptive precision encoding with progressive packing
 * - Implements all specified encoding schemes with adaptive selection
 * - Supports progressive precision packing
 * - Enhanced compression:
  * - Normal-aligned optional method for more aggressive compression
 * - Packing and encoding of vertex deltas as single value with low bit-depth (angle + displacement, packed)
 * - Integrates wavelet compression
 * - Hybrid compression: delta + wavelet + adaptive tessellation
 * - Includes SIMD optimization for displacement reconstruction
 * - Supports polygon streaming
 * - Enables adaptive tessellation
 * PROCEDURAL PREDICTION (both active/creative and passive)
 * - Pattern matching against archetypal forms
 * - Delta prediction based on previous samples
 * - Configurable activation levels and blending
 * - Error thresholds and quality controls
 * - GPU acceleration, using Vulkan Compute pipelines for parallel processing
 * - Dedicated storage buffers for patterns and predictions
 * - Async compute support
 * - Efficient memory management, memory savings potential:
 * - High-frequency details: 30-50% additional reduction
 * - Pattern-matched regions: Up to 70% reduction
 * - Predicted deltas: 40-60% reduction
 * - Performance benefits:
 * - Parallel GPU acceleration for real-time processing of predictions
 * - Async compute overlap with graphics
 * - Minimal CPU overhead
 * 
/*
| Polycount | FBX (Binary) | USD (Binary) | glTF 2.0  | OBJ      | *Optimized Format | Adaptive Mesh | Adaptive + Prediction | Normal-Aligned + Prediction | With Huffman (Disk) | Ultra Packed* |
|-----------|--------------|--------------|-----------|----------|-------------------|---------------|---------------------|---------------------------|-------------------|--------------|
| 20K polys |              |              |           |          |                   |               |                     |                           |                   |              |
| Disk      | 1.8 MB       | 1.2 MB       | 980 KB    | 2.4 MB   | 1.38 MB           | 92 KB         | 46-55 KB            | 20-24 KB                  | 14-17 KB          | 10-12 KB     |
| Memory    | 2.3 MB       | 1.9 MB       | 1.7 MB    | 2.8 MB   | 1.38 MB           | 138 KB        | 69-83 KB            | 30-36 KB                  | 30-36 KB          | 20-24 KB     |
|           |              |              |           |          |                   |               |                     |                           |                   |              |
| 50K polys |              |              |           |          |                   |               |                     |                           |                   |              |
| Disk      | 4.5 MB       | 3.1 MB       | 2.4 MB    | 6.0 MB   | 3.45 MB           | 230 KB        | 115-138 KB          | 58-70 KB                  | 41-49 KB          | 29-35 KB     |
| Memory    | 5.8 MB       | 4.8 MB       | 4.2 MB    | 7.0 MB   | 3.45 MB           | 345 KB        | 173-207 KB          | 87-104 KB                 | 87-104 KB         | 58-70 KB     |
|           |              |              |           |          |                   |               |                     |                           |                   |              |
| 128K polys|              |              |           |          |                   |               |                     |                           |                   |              |
| Disk      | 11.5 MB      | 7.9 MB       | 6.2 MB    | 15.4 MB  | 8.83 MB           | 460 KB        | 230-276 KB          | 115-138 KB                | 81-97 KB          | 58-69 KB     |
| Memory    | 14.8 MB      | 12.3 MB      | 10.8 MB   | 17.9 MB  | 8.83 MB           | 690 KB        | 345-414 KB          | 173-207 KB                | 173-207 KB        | 115-138 KB   |
|           |              |              |           |          |                   |               |                     |                           |                   |              |
| 512K polys|              |              |           |          |                   |               |                     |                           |                   |              |
| Disk      | 46.0 MB      | 31.6 MB      | 24.8 MB   | 61.6 MB  | 35.32 MB          | 1.61 MB       | 805-966 KB          | 403-483 KB                | 282-338 KB        | 201-241 KB   |
| Memory    | 59.2 MB      | 49.2 MB      | 43.2 MB   | 71.6 MB  | 35.32 MB          | 2.42 MB       | 1.21-1.45 MB        | 605-725 KB                | 605-725 KB        | 403-483 KB   |
|           |              |              |           |          |                   |               |                     |                           |                   |              |
| 1M polys  |              |              |           |          |                   |               |                     |                           |                   |              |
| Disk      | 92.0 MB      | 63.2 MB      | 49.6 MB   | 123.2 MB | 69.06 MB          | 2.88 MB       | 1.44-1.73 MB        | 720-864 KB                | 504-605 KB        | 360-432 KB   |
| Memory    | 118.4 MB     | 98.4 MB      | 86.4 MB   | 143.2 MB | 69.06 MB          | 4.32 MB       | 2.16-2.59 MB        | 1.08-1.30 MB              | 1.08-1.30 MB      | 720-864 KB   |

*Optimized Format: is the format utilizing lower bit Vulkan formats, like VK_FORMAT_R8G8B8A8_UNORM (also packing a PBR channel, like metallic in the alpha) to store normals and vertex colors.
We must have a user-exposed variable to enable or disable this (if the user needs an artifact-free normal, etc). If skipping the Optimized format step, we arrive at values like
(Disk: 467-560 KB | Memory: 975-1170 KB for a 1M poly mesh). Same goes for less polygons:
20K polys (16-18 KB | 30-36 KB). 50K polys (37-45 KB | 75-90 KB). 128K polys (75-90 KB | 150-180 KB). 512K polys (262-315 KB | 525-630 KB).

* Ultra Packed column represents new optimizations:
1. Progressive bit depth per subdivision level:
   - Level 1: 4 bits (-6.4 to 6.4cm)
   - Level 2: 3 bits (-3.2 to 3.2cm)
   - Level 3: 2 bits (-1.6 to 1.6cm)
   - Level 4+: 1 bit  (±0.8cm)
*
Key improvements with Huffman:
5. ~30% additional reduction in disk size
6. No memory overhead (decompressed at load time)
7. Most effective on larger meshes (>128K polys)
8. Particularly efficient with normal-aligned prediction data
 * 
 * TODO:
 * - For level 1 and maybe 2, to have smaller step (smaller than 0.8cm), maybe we can have a multiplier value. If multiplier, for example, 0.5, then we can have 0.4cm step
 * and max delta for this level of 3.2 cm - a good setting for skin details. Or even use a multiplier like 0.25 for even finer steps. Ideally this multiplier is automatically 
 * chosen based on displacement range.
 * - Check with assistant reason for most of the functions inside of private scope (safety reasons, probably)
 * - Refactor the compute shader for the Adaptive Packed Delta Encoding as needed, because the updates here may 
 * have made it outdated, incompatible.
 * - Lossless Compression: Apply additional compression techniques (e.g., Huffman coding) to the stored values.
 */
#pragma once
#include "WaveletMesh.h"
//#include "MeshTypes.h"
#include "ProceduralTypes.h"
#include <immintrin.h> // For AVX2 intrinsics

namespace hd {

struct AdaptiveMeshInfo : public NodeInfo {
    AdaptiveMeshInfo() {
        NodeType = "Geometry/AdaptiveMesh";
        
        inputs = {
            "BaseMesh",           // Base low-res mesh
            "SubdivConfig",       // Subdivision parameters
            "DeltaThreshold",     // Sparse storage threshold
            "CompressionConfig",  // Compression settings
            "TessellationParams", // Runtime tessellation config
            "StreamingConfig"     // Streaming parameters
        };
        
        outputs = {
            "ProcessedMesh",      // Final mesh data
            "SubdivisionLevels",  // Hierarchy information
            "CompressionStats",   // Compression metrics
            "StreamingState"      // Streaming status
        };
    }
};

class AdaptiveMesh : public Node {
public:
    // === Structure Definitions ===
    enum class DeltaEncoding {
        INT8_BALANCED,    // int8_t scaled 0.1 (-12.8cm to 12.8cm, 0.1cm precision)
        INT8_EXTENDED,    // int8_t scaled 0.2 (-25.4cm to 25.4cm, 0.2cm precision)
        INT4_PACKED,      // Two int4_t packed (-8cm to 7cm, 1cm precision)
        ADAPTIVE          // Dynamically chosen based on displacement range
    };

    struct SubdivLevel {
        uint32_t level;                  // Current level
        float maxDisplacement;           // Maximum allowed displacement
        float precision;                 // Required precision
        DeltaEncoding encoding;          // Chosen encoding scheme
        float quantizationScale;         // Quantization factor
        
        struct DeltaStats {
            float maxObservedDelta;      // Maximum delta in level
            float averageDelta;          // Average delta magnitude
            uint32_t nonzeroDeltaCount;  // Number of significant deltas
        } stats;
    };

    struct VertexDelta {
        union {
            struct {
                int8_t x;     // Only this is used in normal-aligned mode
                int8_t y;     // Ignored in normal-aligned
                int8_t z;     // Ignored in normal-aligned
            };
            int8_t balanced[3];    // For balanced encoding
            int8_t extended[3];    // For extended encoding
            struct {
                int8_t packed;     // For packed encoding
                bool secondVertex;
            };
        };
        
        uint32_t vertexIndex;
        bool isSignificant;
        bool isNormalAligned;  // Flag to indicate normal-aligned mode
    };

    struct DeltaCompressionScheme {
        // Scale multiplier for finer control
        struct ScaleMultiplier {
            uint8_t multiplierID : 2;     // 4 preset multipliers (1.0, 0.5, 0.25, 0.125)
            static constexpr float multipliers[4] = { 1.0f, 0.5f, 0.25f, 0.125f };
        };

        // Base level (Level 0 -> 1): 4-bit encoding
        struct Level1Delta {
            uint32_t vertexIndex : 24;    // Up to 16M vertices
            uint8_t deltaX : 4;           // Now scaled by multiplier
            uint8_t flags : 2;            // Reduced flags
            uint8_t multiplierID : 2;     // Multiplier selection
        }; // Still 4 bytes total

        // Mid level (Level 1 -> 2): 3-bit encoding
        struct Level2Delta {
            uint32_t vertexIndex : 24;    // Up to 16M vertices
            uint8_t deltaX : 3;           // Now scaled by multiplier
            uint8_t flags : 3;            // Reduced flags
            uint8_t multiplierID : 2;     // Multiplier selection
        }; // Still 4 bytes total

        // Fine level (Level 2 -> 3): 2-bit encoding - unchanged
        struct Level3Delta {
            uint32_t vertexIndex : 24;    // Up to 16M vertices
            uint8_t deltaX : 2;           // -1.6 to 1.6cm (0.8cm steps)
            uint8_t flags : 6;            // More pattern matching bits
        }; // 4 bytes total
    
        // Micro level (Level 3+): 1-bit encoding
        struct Level4PlusDelta {
            uint32_t vertexIndex : 24;    // Up to 16M vertices
            uint8_t deltaX : 1;           // ±0.8cm
            uint8_t flags : 7;            // Maximum pattern matching
        }; // 4 bytes total
    
        // Decoding functions
        // Level 1 delta
        static float computeActualDelta(const Level1Delta& delta) {
            return (delta.deltaX * 0.8f) * ScaleMultiplier::multipliers[delta.multiplierID];
        }
    
        // Level 2 delta
        static float computeActualDelta(const Level2Delta& delta) {
            return (delta.deltaX * 0.8f) * ScaleMultiplier::multipliers[delta.multiplierID];
        }
    
        // Auto-selection of multiplier based on displacement range
        static uint8_t selectMultiplier(float maxDisplacement) {
            if (maxDisplacement < 1.6f) return 3;      // 0.125x for very fine detail
            if (maxDisplacement < 3.2f) return 2;      // 0.25x for fine detail
            if (maxDisplacement < 6.4f) return 1;      // 0.5x for medium detail
            return 0;                                  // 1.0x for large displacement
        }

        // Pattern matching optimization
        struct DeltaPattern {
            uint16_t patternID : 12;      // Up to 4096 common patterns
            uint16_t scale : 4;           // Pattern scale factor
        }; // 2 bytes total
    
        // Batch processing for SIMD
        struct alignas(32) SIMDDeltaBatch {
            __m256i packedDeltas;         // 8 deltas packed
            __m256 scales;                // Individual scaling factors
            __m256i indices;              // Vertex indices
        };
    };
    
    struct CompressionOptimizations {
        // Pattern-based optimizations
        static constexpr uint32_t PATTERN_CACHE_SIZE = 4096;
        static constexpr float MIN_PATTERN_FREQUENCY = 0.01f; // 1% threshold
    
        // Delta prediction
        static constexpr float PREDICTION_THRESHOLD = 0.85f;  // 85% accuracy required
        static constexpr uint32_t PREDICTION_WINDOW = 8;      // Look-back window
    
        // Quantization parameters
        struct QuantizationParams {
            float baseScale = 0.8f;       // Base quantization step (0.8cm)
            float levelScale[4] = {
                1.0f,   // Level 1: full range
                0.5f,   // Level 2: half range
                0.25f,  // Level 3: quarter range
                0.125f  // Level 4+: eighth range
            };
        };
    
        // Memory optimization
        struct CacheOptimization {
            uint32_t blockSize = 64;      // Cache line size
            uint32_t prefetchDistance = 2; // Prefetch 2 blocks ahead
        };
    };

    // SIMD-optimized structures
    struct alignas(32) SIMDVertexBatch {
        __m256i packedDeltas;            // 8 packed deltas
        __m256 scales;                   // Scaling factors
        __m256i indices;                 // Vertex indices
    };

    struct CompressionConfig {
        float deltaThreshold;            // Minimum significant delta
        bool enableWavelets;             // Use wavelet compression
        bool enableSparsity;             // Enable sparse storage
        bool enableProgressivePacking;   // Progressive precision
        
        struct AdaptiveParams {
            float baseScale;             // Base quantization scale
            float levelScaleFactor;      // Scale reduction per level
            float minPrecision;          // Minimum allowed precision
        } adaptive;
    };

    struct ProceduralPredictionConfig {
        bool enabled = false;
        uint32_t activationLevel = 2;    // Start at which subdivision level
        float blendFactor = 0.75f;       // How much to blend with actual data
        
        struct PatternMatching {
            bool enabled = false;
            float similarityThreshold = 0.85f;
            std::vector<std::string> activePatterns = {
                "rock_formation", "crack_pattern", 
                "organic_detail", "surface_noise"
            };
        } patterns;

        struct DeltaPrediction {
            bool enabled = false;
            uint32_t previousSamplesCount = 4;  // How many previous deltas to analyze
            float predictionWeight = 0.6f;      // Weight of prediction vs actual
            float errorThreshold = 0.1f;        // Max allowed prediction error
        } prediction;
    };

    struct ComputeAccelerationConfig {
        bool enableGPUProcessing = true;
        uint32_t workgroupSize = 256;
        bool useAsyncCompute = true;
        
        struct BufferConfig {
            VkDeviceSize patternBufferSize = 1024 * 1024;  // 1MB pattern buffer
            VkDeviceSize predictionBufferSize = 512 * 1024; // 512KB prediction buffer
            bool enableBufferCompaction = true;
        } buffers;
    };

    struct CompressionStats {
        float totalCompressionRatio;
        float averagePrecision;
        uint32_t significantDeltaCount;
        std::vector<float> levelMetrics;
    }
    
    struct GPUResources {
        VkBuffer patternBuffer;
        VkBuffer predictionBuffer;
        VkBuffer deltaBuffer;
        VkDescriptorSet computeDescriptorSet;
        VkPipeline computePipeline;
    }

    // === Enhanced Structure Definitions ===
    struct CompressedDelta {
        int8_t encodedAngleDistance;  // Combined angle and distance
        int8_t normalAlignedValue;    // Z-component/normal-aligned value
        uint32_t vertexIndex;
        bool isSignificant;
    };

    struct SubdivLevel {
        uint32_t level;
        float maxDisplacement;
        
        // Enhanced compression
        struct MaxDeltaInfo {
            int8_t value;      // Actual delta magnitude, using the same scaling as VertexDelta
            int8_t angle;      // 60 steps (0-354 degrees, 6 degrees per step)
            int8_t distance;   // 4 distance steps
        } maxDelta;
        
        // Quantization parameters
        struct QuantizationParams {
            float angleStep;         // Default: 6 degrees
            float distanceStep;      // Default: 0.25 (4 steps)
            float valueScale;        // For normal-aligned value
        } quantization;
    };

    struct CompressionConfig {
        // Existing fields...
        
        struct EnhancedCompression {
            bool enabled = true;
            bool useMaxDeltaOnly = true;    // Store only highest level delta
            uint32_t sparseRate = 20;       // Sample 1 in every N vertices
            
            struct Quantization {
                uint32_t angleBits = 6;     // 60 steps (0-354 degrees)
                uint32_t distanceBits = 2;  // 4 distance steps
                float angleStep = 6.0f;     // 6 degrees per step
                float minDistance = 0.0f;
                float maxDistance = 1.0f;
            } quantization;
            
            struct Prediction {
                bool enabled = true;
                float blendFactor = 0.75f;
                bool useHybridSampling = true;
            } prediction;
        } enhanced;
    };

    struct StorageConfig {
        enum class CompressionMode {
            RUNTIME_OPTIMIZED,    // Current method: Direct access, SIMD-friendly
            DISK_OPTIMIZED,       // Huffman + additional compression for storage
            HYBRID               // Huffman for rare/large deltas only
        };
        
        struct HuffmanConfig {
            bool enableForStorage = true;     // Use for file storage
            bool enableForLargeDeltas = true; // Use for deltas > threshold
            float deltaThreshold = 25.4f;     // When to use Huffman
            bool cacheDecodedValues = true;   // Cache frequently accessed values
        };
    };

    // === Processing Methods ===
    /**
     * @brief Encodes angle and distance into a single 8-bit value.
     */
    int8_t encodeAngleDistance(int8_t angle, int8_t distance) {
        // Combine: (angle << 2) | distance
        // We're using 6 bits for angle (0-59 steps) and 2 bits for distance (0-3 steps)
        uint8_t combined = ((static_cast<uint8_t>(angle) & 0x3F) << 2) | 
                          (static_cast<uint8_t>(distance) & 0x03);
        return static_cast<int8_t>(combined);
    }
    
    /**
     * @brief Decodes a single 8-bit value back into angle and distance.
     */
    std::pair<int8_t, int8_t> decodeAngleDistance(int8_t encoded) {
        uint8_t combined = static_cast<uint8_t>(encoded);
        
        // Extract components
        int8_t angle = static_cast<int8_t>(combined >> 2);    // Upper 6 bits
        int8_t distance = static_cast<int8_t>(combined & 0x03); // Lower 2 bits
        
        return {angle, distance};
    }
    
    /**
     * @brief Computes intermediate level delta from max delta.
     */
    CompressedDelta computeIntermediateDelta(
        const SubdivLevel::MaxDeltaInfo& maxDelta,
        uint32_t currentLevel,
        uint32_t maxLevel) {
        
        // Calculate level ratio (0-255 for int8_t precision)
        uint8_t levelRatio = static_cast<uint8_t>((currentLevel * 255) / maxLevel);
        
        CompressedDelta delta;
        
        // Scale the value based on level ratio
        delta.normalAlignedValue = static_cast<int8_t>(
            (static_cast<int16_t>(maxDelta.value) * levelRatio) >> 8
        );
        
        // Scale angle and distance
        int8_t scaledAngle = static_cast<int8_t>(
            (static_cast<int16_t>(maxDelta.angle) * levelRatio) >> 8
        );
        int8_t scaledDistance = static_cast<int8_t>(
            (static_cast<int16_t>(maxDelta.distance) * levelRatio) >> 8
        );
        
        delta.encodedAngleDistance = encodeAngleDistance(scaledAngle, scaledDistance);
        
        return delta;
    }

    // === Allocation, Initialization, Loading ===
    explicit AdaptiveMesh(const AdaptiveMeshInfo& info = AdaptiveMeshInfo()) : Node(info) {}

    void setupComputeResources();

protected:
    // === Caching & Optimization ===
    uint64_t computeCacheKey() const override;
    void generateRuntimeCode(CodeGenContext& context) override;

private:
    // === Structure Definitions ===
    std::vector<SubdivLevel> subdivLevels;
    std::vector<VertexDelta> deltaStorage;
    CompressionStats compressionStats;
    GPUResources gpuResources;
    PredictionState predictionState;

    // === Procedural Generation ===
    /**
     * @brief Create the prediction compute pipeline.
     */
    void createPredictionPipeline();

    /**
     * @brief Determines if procedural prediction should be applied at a given level.
     */
    bool shouldApplyPrediction(uint32_t level, const CompressionConfig& config);

    /**
     * @brief Blends predicted vertex deltas with actual deltas.
     */
    void blendPredictedDeltas(uint32_t level);

    // === Processing ===
    // GPU prediction
    void processGPUPrediction(uint32_t level);
    void dispatchComputePrediction(VkCommandBuffer cmd, uint32_t vertexCount);

    // Adaptive encoding
    DeltaEncoding selectOptimalEncoding(const SubdivLevel& level);
    void updateProgressivePrecision(SubdivLevel& level);
    float computeQuantizationScale(float maxDelta, float targetPrecision);

    // Subdivision and delta processing
    void computeVertexDeltas(uint32_t level);
    void optimizeDeltaStorage(const CompressionConfig& config);
    /**
     * @brief Packs a vertex delta using progressive precision and regressive encoding bits.
     */
    void packDeltaProgressive(VertexDelta& delta);
    /**
     * @brief Packs a vertex delta using balanced encoding. Range: -12.8cm to 12.8cm. Precision 0.1cm.
     */
    void packDeltaBalanced(VertexDelta& delta);
    /**
     * @brief Packs a vertex delta using extended encoding. Range: -25.4cm to 25.4cm. Precision 0.2cm.
     */
    void packDeltaExtended(VertexDelta& delta);
    /**
     * @brief Packs a vertex delta using packed encoding. Range: -8cm to 7cm. Precision 1cm.
     */
    void packDeltaPacked(VertexDelta& delta);

    void buildSubdivisionHierarchy(const MeshData& mesh);

    // SIMD operations
    void processSIMDVertexBatch(SIMDVertexBatch& batch);
    __m256 reconstructDisplacements(const __m256i& packedDeltas, const __m256& scales);

    // Compression utilities
    void applyWaveletCompression();
    void generateStreamingChunks();
    void optimizeForGPU();

    /**
    * @brief Reconstructs displacements from packed deltas using SIMD instructions.
    */
    void processAdaptiveMesh(const MeshData& mesh, const CompressionConfig& config);

    /**
     * @brief Processes the node graph. Method inherited from Node.
     */
    void processNode();

    // === Storage ===
    std::vector<CompressedDelta> maxLevelDeltas;  // Only highest level deltas
    std::vector<uint32_t> sampledVertexIndices;   // For hybrid sampling
};

} // namespace hd
