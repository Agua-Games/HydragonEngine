/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 * 
 * These examples demonstrate:
 * 
 *  - Particle system chain
 *  - Audio processing chain
 *  - Post-processing chain
 */
#if 0
#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"
#include "ParticleEmitter.h"
#include "ParticleModifier.h"
#include "AcousticSource.h"
#include "Reverb.h"
#include "Compressor.h"
#include "Bloom.h"
#include "ColorGrading.h"

using namespace hd;

// Example 1: Particle system chain
auto particles = NodeGraph::create("effects")
    .add<ParticleEmitter>("mainEmitter")
        .emissionRate(50.0f, runtime)
        .initialVelocity({0.0f, 1.0f, 0.0f})
        .particleLifetime(2.0f, runtime)
        .connect("output", "modifier1.input")
    .add<ParticleModifier>("modifier1")
        .mode(ParticleModifierMode::Scale)
        .scaleCurve({
            {0.0f, 1.0f},
            {1.0f, 0.0f}
        })
        .connect("output", "renderer.input");

// Example 2: Audio processing chain
auto audio = NodeGraph::create("audio")
    .add<AcousticSource>("musicTrack")
        .volume(0.8f, runtime)
        .pitch(1.0f, runtime)
        .connect("output", "reverb.input")
    .add<Reverb>("reverb")
        .roomSize(0.7f, runtime)
        .damping(0.3f, runtime)
        .connect("output", "compressor.input")
    .add<Compressor>("compressor")
        .threshold(-12.0f, runtime)
        .ratio(4.0f, runtime);

// Example 3: Post-processing chain
auto post = NodeGraph::create("post")
    .add<Bloom>("bloom")
        .intensity(1.0f, runtime)
        .threshold(1.0f, runtime)
        .connect("output", "colorGrade.input")
    .add<ColorGrading>("colorGrade")
        .contrast({1.0f, 1.0f, 1.0f}, runtime)
        .saturation(1.0f, runtime);

#endif