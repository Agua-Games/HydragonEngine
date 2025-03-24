/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file WaveletSkinDeformerNode.h
 * @brief WaveletSkinDeformerNode is a deformer that uses wavelet-based skinning.
 * 
 * ARCHITECTURAL NOTES:
 * - Extends SkeletonDeformer to support wavelet-based skinning
 * - Uses wavelet transforms to capture skin details and apply deformation effects like skin bunching, slide, stretch, wrinkling, etc.
 * - Supports various wavelet bases (Haar, Daubechies, etc.) for different use cases
 * - Enables progressive loading through LOD hierarchy
 * - Integrates with existing SkeletonDeformer system
 * - Provides fine-grained control over skinning parameters
 * - Includes runtime optimization capabilities
 * - Supports caching and node graph features
 * 
 */
#pragma once
#include "SkeletonDeformer.h"

namespace hd {

class WaveletSkinDeformerNode : public SkeletonDeformer {
public:
    // === Structure Definitions ===
    struct SpringWavelet {
        float restLength;
        vec3 normal;
        float dampening;
        
        // Wavelet parameters
        struct WaveletParams {
            float frequency;
            float amplitude;
            float phase;
            InterpolationType interpolation; // LOG, EXP, etc.
        } wavelet;
        
        // Bunching control
        struct BunchingParams {
            float compressionThreshold;
            float curvatureWeight;
            float normalAlignment;
        } bunching;
    };

    // === Processing ===
    void computeDeformation() override {
        // 1. Sample vertices at stride intervals
        for(uint32_t i = 0; i < vertices.size(); i += config.stride) {
            auto& spring = springs[i];
            
            // 2. Compute compression from skinning
            float compression = computeCompression(vertices[i]);
            
            if(compression > spring.bunching.compressionThreshold) {
                // 3. Generate wavelet pattern
                auto wavelet = generateBunchingWavelet(
                    compression, 
                    spring.normal,
                    spring.wavelet
                );
                
                // 4. Apply bunching deformation
                applyWaveletBunching(
                    vertices[i], 
                    wavelet,
                    spring.bunching
                );
            }
        }
    }

private:
    // === Processing ===
    void applyWaveletBunching(
        Vertex& vertex, 
        const WaveletPattern& wavelet,
        const BunchingParams& params
    ) {
        // Find closest wavelet plot point
        vec3 targetPos = findClosestWaveletPoint(
            vertex.position, 
            wavelet
        );
        
        // Apply curvature-based weighting
        float curvatureWeight = computeCurvatureWeight(
            vertex,
            params.curvatureWeight
        );
        
        // Interpolate position using configured method
        vertex.position = interpolatePosition(
            vertex.position,
            targetPos,
            wavelet.interpolation,
            curvatureWeight
        );
    }
};

} // namespace hd