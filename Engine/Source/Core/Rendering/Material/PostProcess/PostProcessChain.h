/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file PostProcessChain.h
 * @brief Header file for the PostProcessChain class.
 * 
 * ARCHITECTURAL NOTES:
 * - PostProcessChain is a class that represents a chain of post-processing effects in the engine's node graph.
 * - It is used to apply a sequence of post-processing effects to rendered images.
 * 
 */
#pragma once
#include <string>
#include <unordered_map>
#include <memory>
#include "PostProcess.h"

namespace hd {

struct PostProcessChainInfo : public PostProcessInfo {
    PostProcessChainInfo() {
        NodeType = "Rendering/PostProcess/PostProcessChain";
        inputs = {
            "RenderedImage",                   // Rendered image to apply post-processing chain to
            "BloomIntensity",                  // Bloom intensity
            "PostProcessChainParams"           // Post-processing chain parameters
        };
        outputs = {
            "ProcessedChainImage",             // Post-processed image after the chain
            "PostProcessChainMetrics"          // Post-processing chain performance metrics
        };
    }
};

class PostProcessChain : public PostProcess {
public:
    // === Allocation, Initialization, Loading ===
    explicit PostProcessChain(const PostProcessChainInfo& info = PostProcessChainInfo())
        : PostProcess(info) {}   
    initialize() override {}
    load() override {}

    std::unordered_map<std::string, std::shared_ptr<PostProcess>> passes;
    std::unordered_map<std::string, std::any> PostProcessChainParams;

    // === Processing ===
    void addPass(const std::string& name, const std::shared_ptr<PostProcess>& pass);
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~PostProcessChain() = default;     // Default destructor
};

} // namespace hd
