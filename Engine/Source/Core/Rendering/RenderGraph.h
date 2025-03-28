/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RenderGraph.h
 * @brief A node graph for rendering.
 * 
 * ARCHITECTURAL NOTES:
 * - This file contains a node graph for rendering.
 * - It is used to define and manage rendering passes.
 * 
 * @todo This is a placeholder for the actual implementation.
 */
#pragma once
#include <string>
#include <unordered_map>
#include "Node.h"
#include "PrismaRenderer.h"

using namespace hd;

Engine::get()
    .renderer()
        .renderPath(PrismaRenderer::RenderPath::DEFERRED)
        .connect<RenderPass>("gBuffer")          // Need to decide if we risk the overhead of graphics-level objects being nodes, instead of using PrismaRenderer methods
        .connect<RenderPass>("lighting")
        .connect<RenderPass>("shadows")
        .connect<RenderPass>("multiSampling")
        .connect<RenderPass>("ssao")
        .connect<RenderPass>("ssr")
        .connect<RenderPass>("bloom")
        .connect<RenderPass>("taa")
        .connect<RenderPass>("fxaa")
        .connect<RenderPass>("volumetricLighting")
        .connect<RenderPass>("main")
        .connect<BloomEffect>()     // This was an assistant suggestion. Look like connecting a PostProcess (single node). Could connect a PostProcessChain, also.
            .threshold(1.0f)
            .intensity(0.5f)
        .connect<SSAOEffect>()
            .radius(0.5f)
            .samples(16)
        .connect<TonemapEffect>()
            .exposure(1.0f)
            .connect<ColorGrading>()
                .temperature(6500.0f)
                .contrast(1.1f)
        .connect<FXAAEffect>();