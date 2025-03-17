/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */
#include "Core/Engine.h"
#include "Core/NodeGraph/Node.h"

using namespace hd;

// Post-processing chain
auto post = graph.create<PostProcessNode>("mainPost")
    .connect<BloomNode>("bloom")
        .threshold(1.0f)
        .intensity(0.8f)
        .scatter(0.7f)
    .connect<TonemapNode>("tonemap")
        .operator(TonemapOperator::ACES)
        .exposure(1.0f)
    .connect<SSAONode>("ssao")
        .radius(0.5f)
        .bias(0.025f)
        .intensity(1.0f)
        .quality(SSAOQuality::High)
    .connect<DOFNode>("dof")
        .focalDistance(10.0f)
        .focalRange(5.0f)
        .maxBlur(1.0f);