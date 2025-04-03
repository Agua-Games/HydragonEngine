/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file NodeGraph_UsageExample_02.cpp
 * @brief This file contains examples of how to setup a node graph in code.
 * The syntax in these examples is a bit outdated. We'll fix it or remove this example altogether.
 * 
 * These examples demonstrate:
 * 
 *  - Complete game level setup
 *  - Character controller with animation system
 *  - Procedural weapon generation system
 *  - Post-processing chain
 *  - AI behavior tree using nodes
 *  - Particle system setup
 *  - Shader graph setup
 */
#if 0
using namespace hd;

// Example 1: Complete game level setup
auto& level = Scene::current()
    .add<DynamicEnvLighting>("main_level")
        .skysphere("textures/sky_day.hdr", 1.0f)
        .ambientColor({0.1f, 0.1f, 0.12f})
        .ambientIntensity(0.2f)
        .fog()
            .color({0.5f, 0.6f, 0.7f})
            .density(0.01f)
    .add<Terrain>()
        .heightmap("terrain/height_01.png")
        .connect<TerrainLayer>()
            .density(0.3f)
        .connect<Solid>()
            .shapeType(ShapeType::Sphere)
            .radius(100.0f)
        .connect<Vegetation>()
            .density(100)
            .types({"grass", "trees", "rocks"});

// Example 2: Character controller with animation system
auto& player = Scene::current()
    .add<Transform>("player")
        .position({0, 0, 0})
    .add<Character>()
        .speed(5.0f)
        .jumpForce(10.0f)
        .connect<AnimationStateMachine>()
            .addState("idle", "anims/idle.fbx")
            .addState("walk", "anims/walk.fbx")
            .addState("run", "anims/run.fbx")
            .connect<AnimationBlend>()
                .addTransition("idle", "walk", 0.2f)
                .addTransition("walk", "run", 0.1f)
        .connect<InputHandler>()
            .mapKey(Key::W, "move_forward")
            .mapKey(Key::Space, "jump")
        .connect<AcousticSource>()
            .addAudioBank("footsteps", "sfx/footsteps.wav")
            .addAudioBank("jump", "sfx/jump.wav");

// Example 3: Procedural weapon generation system
auto& weaponGen = Factory::create<WeaponMechanics>()
    .connect<Equipment>()
        .addVariant("pistol", "weapons/pistol.usd")         // Just a syntax sketch, as others here
        .addVariant("shotgun", "weapons/shotgun.usd")
        .addVariant("rifle", "weapons/rifle.usd")
        .material("metal")
    .connect<Stats>()
        .damageRange({10.0f, 20.0f})
        .rateOfFire({0.5f, 2.0f});

// Example 4: Post-processing chain
auto& postProcess = Renderer::get()
    .add<RenderPass>("main")
        .connect<BloomEffect>()
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

// Example 5: AI behavior tree using nodes
auto& npc = Scene::current()
    .add<ControllerAI>("guard")
        .add<BehaviorTree>()            // We'll convert the Behavior tree setup to use a normal node graph, instead of this
            .sequence("patrol")
                .add<MoveToPoint>()
                    .position({10, 0, 10})
                .add<Wait>()
                    .duration(3.0f)
                .add<MoveToPoint>()
                    .position({-10, 0, 10})
                .add<Wait>()
                    .duration(3.0f)
            .selector("combat")
                .add<CheckHealth>()
                    .threshold(0.3f)
                .add<Flee>()
                    .speed(8.0f)
                .add<Attack>()
                    .range(5.0f)
                    .damage(10.0f);

// Example 6: Particle system setup
auto& particles = Scene::current()
    .add<ParticleSystem>("magic_effect")
        .maxParticles(1000)
        .emission()
            .rate(50)
            .burst(100)
        .shape()
            .sphere(1.0f)
        .connect<ColorOverLife>()
            .addKey(0.0f, {1, 0, 0, 1})
            .addKey(0.5f, {0, 1, 0, 1})
            .addKey(1.0f, {0, 0, 1, 0})
        .connect<SizeOverLife>()
            .addKey(0.0f, 0.1f)
            .addKey(1.0f, 0.0f);

// Example 7: Shader graph setup
auto& shader = ShaderGraph::create("custom_pbr")
    .add<TextureInput>("albedo")
        .default("textures/default_albedo.png")
    .add<TextureInput>("normal")
        .default("textures/default_normal.png")
    .add<FloatInput>("metallic")
        .range(0.0f, 1.0f)
        .default(0.5f)
    .connect<NormalMapping>()
    .connect<PBRCalculation>()
        .connect<LightingModel>()
            .type(LightingType::Physical)
    .connect<OutputColor>();

#endif