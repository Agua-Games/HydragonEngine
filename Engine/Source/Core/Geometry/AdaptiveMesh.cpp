/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AdaptiveMesh.cpp
 * @brief AdaptiveMesh is a mesh node which uses subdivision and adaptive compression.
 * 
 * ARCHITECTURAL NOTES:
 * - (See the header file for more details)
 * - This implementation is still totally a first phase design sketch, not functional yet.
 */

#include "AdaptiveMesh.h"
#include "VulkanCore.h"
#include "VulkanCompute.h"
#include "MeshUtils.h"
#include "WaveletCompression.h"
#include "Logger.h"

namespace hd {

// === Allocation, Initialization, Loading ===
/**
 * @brief Sets up compute resources for GPU prediction.
 */
void AdaptiveMesh::setupComputeResources() {
    auto& vk = VulkanCore::get();
    
    // Create pattern and prediction buffers
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.usage = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT | 
                      VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    
    // Pattern buffer
    bufferInfo.size = config.compute.buffers.patternBufferSize;
    vk.createBuffer(bufferInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                   gpuResources.patternBuffer);
    
    // Prediction buffer
    bufferInfo.size = config.compute.buffers.predictionBufferSize;
    vk.createBuffer(bufferInfo, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
                   gpuResources.predictionBuffer);
    
    // Create compute pipeline
    createPredictionPipeline();
}

/**
 * @brief Creates the prediction compute pipeline.
 */
void AdaptiveMesh::createPredictionPipeline() {
    auto& vk = VulkanCore::get();
    
    // Load compute shader
    auto computeShader = vk.loadShader("shaders/prediction.comp.spv");
    
    // Create pipeline layout
    std::vector<VkDescriptorSetLayoutBinding> bindings = {
        { 0, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT },
        { 1, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT },
        { 2, VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1, VK_SHADER_STAGE_COMPUTE_BIT }
    };
    
    VkDescriptorSetLayout descriptorLayout;
    vk.createDescriptorSetLayout(bindings, descriptorLayout);
    
    VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
    pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
    pipelineLayoutInfo.setLayoutCount = 1;
    pipelineLayoutInfo.pSetLayouts = &descriptorLayout;
    
    VkPipelineLayout pipelineLayout;
    vkCreatePipelineLayout(vk.device, &pipelineLayoutInfo, nullptr, &pipelineLayout);
    
    // Create compute pipeline
    VkComputePipelineCreateInfo pipelineInfo{};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
    pipelineInfo.stage.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    pipelineInfo.stage.stage = VK_SHADER_STAGE_COMPUTE_BIT;
    pipelineInfo.stage.module = computeShader;
    pipelineInfo.stage.pName = "main";
    pipelineInfo.layout = pipelineLayout;
    
    vkCreateComputePipelines(vk.device, VK_NULL_HANDLE, 1, 
                            &pipelineInfo, nullptr, &gpuResources.computePipeline);
}

// === // === Procedural ===
/**
 * @brief Predicts a delta from a pattern.
 */
glm::vec3 AdaptiveMesh::predictDeltaFromPattern(
    const SubdivLevel& level,
    uint32_t vertexIndex,
    const std::vector<VertexDelta>& previousDeltas) {
    
    // Get previous samples
    std::vector<glm::vec3> samples;
    const uint32_t sampleCount = config.proceduralPrediction.prediction.previousSamplesCount;
    
    for (uint32_t i = 0; i < sampleCount && i < previousDeltas.size(); ++i) {
        if (previousDeltas[i].isSignificant) {
            samples.push_back(previousDeltas[i].value);
        }
    }
    
    if (samples.empty()) {
        return glm::vec3(0.0f);
    }
    
    // Simple linear prediction (could be enhanced with more sophisticated methods)
    glm::vec3 prediction(0.0f);
    float totalWeight = 0.0f;
    
    for (size_t i = 0; i < samples.size(); ++i) {
        float weight = 1.0f / (i + 1); // More recent samples have higher weight
        prediction += samples[i] * weight;
        totalWeight += weight;
    }
    
    return prediction / totalWeight;
}

/**
 * @brief Evaluates the similarity between a segment of deltas and a pattern.
 */
float AdaptiveMesh::evaluatePatternSimilarity(
    const std::vector<VertexDelta>& segment,
    const ProceduralPattern& pattern) {
    
    if (segment.empty() || pattern.samples.empty()) {
        return 0.0f;
    }
    
    float similarity = 0.0f;
    const size_t compareLength = std::min(segment.size(), pattern.samples.size());
    
    for (size_t i = 0; i < compareLength; ++i) {
        float deltaSimilarity = glm::distance(
            segment[i].value,
            pattern.samples[i]
        );
        similarity += 1.0f / (1.0f + deltaSimilarity);
    }
    
    return similarity / static_cast<float>(compareLength);
}

bool AdaptiveMesh::shouldApplyPrediction(uint32_t level, const CompressionConfig& config) {
    return config.proceduralPrediction.enabled && 
           level >= config.proceduralPrediction.activationLevel;
}

// === Processing ===
void AdaptiveMesh::blendPredictedDeltas(uint32_t level) {
    const float blend = config.proceduralPrediction.blendFactor;
    
    for (auto& delta : deltaStorage) {
        if (delta.level == level && delta.isSignificant) {
            const auto predicted = predictionState.predictedDeltas[delta.vertexIndex];
            delta.value = glm::mix(delta.value, predicted, blend);
        }
    }
}

/**
 * @brief Dispatches compute work for prediction.
 */
void AdaptiveMesh::dispatchComputePrediction(VkCommandBuffer cmd, 
                                                uint32_t vertexCount) {
    const uint32_t workgroupSize = config.compute.workgroupSize;
    const uint32_t numGroups = (vertexCount + workgroupSize - 1) / workgroupSize;
    
    vkCmdDispatch(cmd, numGroups, 1, 1);
}

/**
 * @brief Processes GPU prediction for a given subdivision level.
 */
void AdaptiveMesh::processGPUPrediction(uint32_t level) {
    auto& vk = VulkanCore::get();
    
    // Begin command buffer
    VkCommandBuffer cmd = vk.beginSingleTimeCommands();
    
    // Update descriptor sets
    std::vector<VkDescriptorBufferInfo> bufferInfos = {
        { gpuResources.patternBuffer, 0, VK_WHOLE_SIZE },
        { gpuResources.predictionBuffer, 0, VK_WHOLE_SIZE },
        { gpuResources.deltaBuffer, 0, VK_WHOLE_SIZE }
    };
    
    vk.updateDescriptorSets(gpuResources.computeDescriptorSet, bufferInfos);
    
    // Bind pipeline and descriptor set
    vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_COMPUTE, 
                     gpuResources.computePipeline);
    vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_COMPUTE,
                           pipelineLayout, 0, 1, 
                           &gpuResources.computeDescriptorSet, 0, nullptr);
    
    // Dispatch compute work
    const uint32_t vertexCount = subdivLevels[level].vertices.size();
    dispatchComputePrediction(cmd, vertexCount);
    
    // End and submit command buffer
    vk.endSingleTimeCommands(cmd);
}

/**
 * @brief Computes vertex deltas for a given subdivision level, optimizing storage.
 */
void AdaptiveMesh::optimizeDeltaStorage(const CompressionConfig& config) {
    if (!config.enableSparsity) {
        return;
    }
    
    // Remove insignificant deltas
    deltaStorage.erase(
        std::remove_if(
            deltaStorage.begin(),
            deltaStorage.end(),
            [&](const VertexDelta& delta) {
                return !delta.isSignificant &&
                       glm::length(delta.value) < config.deltaThreshold;
            }
        ),
        deltaStorage.end()
    );
    
    // Pack remaining deltas
    if (config.enableProgressivePacking) {
        for (auto& delta : deltaStorage) {
            packDeltaProgressive(delta);
        }
    }
}

void AdaptiveMesh::packDeltaProgressive(VertexDelta& delta) {
    // Choose encoding based on magnitude
    float magnitude = glm::length(delta.value);
    
    if (magnitude < 12.8f) {
        delta.encoding = DeltaEncoding::INT8_BALANCED;
        packDeltaBalanced(delta);
    }
    else if (magnitude < 25.4f) {
        delta.encoding = DeltaEncoding::INT8_EXTENDED;
        packDeltaExtended(delta);
    }
    else {
        delta.encoding = DeltaEncoding::INT4_PACKED;
        packDeltaPacked(delta);
    }
}

void AdaptiveMesh::packDeltaBalanced(VertexDelta& delta) {
    const float scale = 10.0f; // 0.1cm precision
    for (int i = 0; i < 3; ++i) {
        delta.balanced[i] = static_cast<int8_t>(
            glm::clamp(delta.value[i] * scale, -128.0f, 127.0f)
        );
    }
}

void AdaptiveMesh::packDeltaExtended(VertexDelta& delta) {
    const float scale = 5.0f; // 0.2cm precision
    for (int i = 0; i < 3; ++i) {
        delta.extended[i] = static_cast<int8_t>(
            glm::clamp(delta.value[i] * scale, -128.0f, 127.0f)
        );
    }
}

void AdaptiveMesh::packDeltaPacked(VertexDelta& delta) {
    const float scale = 1.0f; // 1cm precision
    int8_t packed = 0;
    
    // Pack two 4-bit values into one byte
    for (int i = 0; i < 2; ++i) {
        int value = static_cast<int>(
            glm::clamp(delta.value[i] * scale, -8.0f, 7.0f)
        );
        packed |= (value & 0x0F) << (i * 4);
    }
    
    delta.packed = packed;
}

void AdaptiveMesh::processAdaptiveMesh(const MeshData& mesh, const CompressionConfig& config) {
    if (config.proceduralPrediction.enabled) {
        setupComputeResources();
    }

    buildSubdivisionHierarchy(mesh);

    for (uint32_t level = 0; level < subdivLevels.size(); ++level) {
        computeVertexDeltas(level);

        if (shouldApplyPrediction(level, config)) {
            processGPUPrediction(level);
            blendPredictedDeltas(level);
        }

        optimizeDeltaStorage(config);
        updateProgressivePrecision(subdivLevels[level]);
    }

    if (config.enableWavelets) {
        applyWaveletCompression();
    }

    generateStreamingChunks();
    optimizeForGPU();
}

void AdaptiveMesh::processNode() {
    auto baseMesh = getInputValue<MeshData>("BaseMesh");
    auto config = getInputValue<CompressionConfig>("CompressionConfig");
    
    if (isDirty || !tryUseCache()) {
        processAdaptiveMesh(baseMesh, config);
        updateCache();
    }

    setOutputValue("ProcessedMesh", processedMesh);
    setOutputValue("SubdivisionLevels", subdivLevels);
    setOutputValue("CompressionStats", compressionStats);
}

} // namespace hd