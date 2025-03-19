/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AdaptiveMeshNode.h
 * @brief AdaptiveMeshNode is a mesh node which uses subdivision and adaptive compression.
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
 * TODO:
 * - Check with assistant reason for most of the functions inside of private scope (safety reasons, probably)
 */
#pragma once
#include "WaveletMeshNode.h"
//#include "MeshTypes.h"
#include "ProceduralTypes.h"
#include <immintrin.h> // For AVX2 intrinsics

namespace hd {

struct AdaptiveMeshInfo : public NodeInfo {
    AdaptiveMeshInfo() {
        NodeType = "Geometry/AdaptiveMesh";
        
        Inputs = {
            "BaseMesh",           // Base low-res mesh
            "SubdivConfig",       // Subdivision parameters
            "DeltaThreshold",     // Sparse storage threshold
            "CompressionConfig",  // Compression settings
            "TessellationParams", // Runtime tessellation config
            "StreamingConfig"     // Streaming parameters
        };
        
        Outputs = {
            "ProcessedMesh",      // Final mesh data
            "SubdivisionLevels",  // Hierarchy information
            "CompressionStats",   // Compression metrics
            "StreamingState"      // Streaming status
        };
    }
};

class AdaptiveMeshNode : public Node {
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

    // === Allocation, Initialization, Loading ===
    explicit AdaptiveMeshNode(const AdaptiveMeshInfo& info = AdaptiveMeshInfo()) : Node(info) {}

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
};

} // namespace hd