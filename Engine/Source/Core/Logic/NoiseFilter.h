/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NoiseFilter.h
 * @brief NoiseFilter represents a noise filter in the engine's node graph.
 * It is used to apply various filters to noise patterns, using strategies such as thresholding, frequency filtering, and others.
 * 
 * ARCHITECTURAL NOTES:
 * - NoiseFilter is a class that represents a noise filter in the engine's node graph.
 * - It supports different types of filters, such as low-pass, high-pass, band-pass, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include "Node.h"
#include "ProceduralTypes.h"
#include "Noise.h"
#include "ProceduralManager.h"

namespace hd {

struct NoiseFilterInfo : public NodeInfo {
    enum class FilterType {
        LowPass,
        HighPass,
        BandPass,
        BandReject,
        Notch,
        AllPass,
        Custom
    };

    NoiseFilterInfo() {
        NodeType = "Procedural/NoiseFilter";
        
        inputs = {
            "inputNoise",
            "threshold",
            "inputNoiseType",      // Perlin, Simplex, Worley, etc.
            "filterType",     // Low-pass, high-pass, band-pass, etc.
            "filterParams",   // Cutoff frequency, Q factor, etc.
        };
        
        outputs = {
            "filteredNoise",
            "filterMetrics"    // Statistical properties
        };
    }
};

class NoiseFilter : public Node {
public:
    // === Allocation, Initialization, Loading ===
    explicit NoiseFilter(const NoiseFilterInfo& info = NoiseFilterInfo())
        : Node(info) {}
    initialize() override {}
    load() override {}
    
    // === Processing ===
    void processNode() override {
        auto threshold = getInputValue<float>("threshold");
        auto noiseType = getInputValue<NoiseType>("inputNoiseType");
        uint32_t seed = getInputValue<uint32_t>("Seed");
        float scale = getInputValue<float>("Scale");
        int octaves = getInputValue<int>("Octaves");
        float persistence = getInputValue<float>("Persistence");
        float lacunarity = getInputValue<float>("Lacunarity");
        
    }
    
    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~NoiseFilter() = default;     // Default destructor
};

} // namespace hd
