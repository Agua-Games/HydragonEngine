/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * This is still a sketch, on how to design the data structures and interfaces to allow one method for mesh animation optimization (compression, encoding, etc) 
 * using lattices - for performance gains while keeping quality. This file will change a lot, probably becoming one or more classes.
 * 
 * ARCHITECTURAL NOTES:
 * - Rules should be data-driven, loaded from config
 * - Support runtime format switching based on distance
 * - Enable easy debugging/visualization of encoding
 * - Maintain original data for editor/tools
 * 
 * TODO:
 *  - Refactor content to match the latest Object and Node design.
 */
#pragma once

namespace hd {
class LatticeAnimEncoder : public AnimationEncoder {
public:
    struct CompressionStats {
        // Memory usage per LOD level
        struct LODStats {
            uint32_t baseMemory;      // Base lattice
            uint32_t deltaMemory;     // Delta storage
            uint32_t keyframeCount;   // Active keyframes
            float compressionRatio;   // Achieved compression
        };
        
        std::vector<LODStats> lodStats;
    };

    void CompressAnimation(AdaptiveLatticeDeformer& lattice) {
        for(uint32_t lod = 0; lod < MAX_LODS; lod++) {
            // Progressive bit depth per LOD:
            // LOD0: 4-bit deltas (-6.4 to 6.4cm)
            // LOD1: 3-bit deltas (-3.2 to 3.2cm)
            // LOD2: 2-bit deltas (-1.6 to 1.6cm)
            // LOD3+: 1-bit deltas (±0.8cm)
            
            ConfigureLODCompression(lod);
            CompressLODLevel(lattice, lod);
        }
    }

    void CompressLODLevel(AdaptiveLatticeDeformer& lattice, uint32_t lod) {
        // Reuse AdaptiveMesh's DeltaEncoding and DeltaCompressionScheme
        auto& scheme = lattice.GetCompressionScheme(lod);
        
        for(auto& keyframe : lattice.GetKeyframes()) {
            // Apply progressive encoding
            for(auto& delta : keyframe.deltas) {
                if(lod == 0) {
                    CompressLevel1Delta(delta, scheme);
                }
                else if(lod == 1) {
                    CompressLevel2Delta(delta, scheme);
                }
                else {
                    CompressLevel3PlusDelta(delta, scheme);
                }
            }
        }
    }
};

} // namespace hd