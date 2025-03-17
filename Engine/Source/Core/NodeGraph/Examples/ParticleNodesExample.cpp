/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */
#include "Core/Engine.h"
#include "ParticleSystemNode.h"
#include "ProcOrchestrator.h"
#include "ProceduralTypes.h"
#include "ProcEvolutionNode.h"
#include "PatternNode.h"

using namespace hd;
// Particle system
auto particles = graph.create<ParticleSystemNode>("fire")
    .maxParticles(1000)
    .simulation(SimulationSpace::World)
    .connect<EmitterNode>("emitter")
        .rate(50.0f)
        .shape(EmitterShape::Cone)
        .angle(30.0f)
        .speed({1.0f, 2.0f})
        .size({0.1f, 0.2f})
        .life({1.0f, 2.0f})
    .connect<ForceFieldNode>("wind")
        .direction({1.0f, 0.2f, 0.0f})
        .strength(0.5f)
    .connect<ColorOverLifeNode>("color")
        .addKey(0.0f, {1.0f, 0.5f, 0.0f, 1.0f})
        .addKey(0.5f, {1.0f, 0.2f, 0.0f, 0.8f})
        .addKey(1.0f, {0.5f, 0.1f, 0.0f, 0.0f})
    .connect<SizeOverLifeNode>("size")
        .addKey(0.0f, 1.0f)
        .addKey(1.0f, 0.0f);