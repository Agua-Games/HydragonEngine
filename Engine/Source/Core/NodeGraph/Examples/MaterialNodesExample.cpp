/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */

// Material nodes showing texture/shader workflow
auto metallic = graph.create<MaterialNode>("brushedMetal")
    .baseColor({0.9f, 0.9f, 0.9f})
    .roughness(0.4f)
    .metallic(1.0f)
    .normal(1.0f)
    .connect<TextureNode>("albedo")
        .path("textures/metal/albedo.png")
        .sRGB(true)
        .filter(TextureFilter::Anisotropic)
        .wrap(TextureWrap::Repeat)
    .connect<TextureNode>("roughness")
        .path("textures/metal/roughness.png")
        .channel(Channel::Red)
    .connect<NormalMapNode>("normal")
        .path("textures/metal/normal.png")
        .strength(0.8f);

// Procedural material using math nodes
auto marble = graph.create<MaterialNode>("marble")
    .connect<NoiseNode>("noise")
        .scale(0.5f)
        .octaves(4)
        .persistence(0.5f)
    .connect<ColorRampNode>("ramp")
        .addStop(0.0f, {0.8f, 0.8f, 0.8f})
        .addStop(0.5f, {0.9f, 0.9f, 0.9f})
        .addStop(1.0f, {1.0f, 1.0f, 1.0f});