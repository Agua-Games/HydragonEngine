/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @brief This file contains examples of how to setup a node graph in code.
 */

#if 0
using namespace hd;

// Example 1: Material creation with precise typing
auto material = ResourceManager::get().create<Material>("metal")
    .setProperty<Vector3>("baseColor", {0.8f, 0.8f, 0.8f})
    .setProperty<float>("roughness", 0.5f)
    .setProperty<float>("metallic", 1.0f)
    .setTexture("albedoMap", "textures/metal_albedo.png")
    .setTexture("normalMap", "textures/metal_normal.png")
    .setTexture("roughnessMap", "textures/metal_roughness.png");

// Example 2: Physics setup with explicit types
auto& rigidBody = entity.add<RigidBody>()
    .setMass(1.0f)
    .setFriction(0.5f)
    .setRestitution(0.3f);

auto& collider = rigidBody.connect<BoxCollider>()
    .setSize(Vector3{1.0f, 1.0f, 1.0f})
    .setOffset(Vector3{0.0f, 0.5f, 0.0f})
    .setTrigger(false);

// Example 3: Audio mixer with strongly typed parameters
auto& audioMixer = Engine::get().audio().createMixer("sfx")
    .addTrack<AudioClip>("music")
        .setVolume(0.8f)
        .setFadeTime(2.0f)
        .addEffect<ReverbEffect>()
            .setRoomSize(0.7f)
            .setDamping(0.3f)
    .addTrack<AudioClip>("ambient")
        .setVolume(0.4f)
        .addEffect<LowPassFilter>()
            .setCutoff(1000.0f);

// Example 4: Shader pipeline with type checking
auto& shader = ShaderCompiler::create("terrain")
    .addStage<VertexShader>("shaders/terrain.vert")
        .define("MAX_LIGHTS", "4")
        .include("shaders/common.glsl")
    .addStage<FragmentShader>("shaders/terrain.frag")
        .define("USE_PBR", "1")
    .addUniform<Vector4>("u_LightPos")
    .addUniform<Matrix4>("u_ViewProj")
    .compile();

// Example 5: UI layout with strong typing and constraints
auto& ui = UIManager::get().createWindow("inventory")
    .setSize<Pixels>(800, 600)
    .add<UIPanel>("background")
        .setColor(Color{0.2f, 0.2f, 0.2f, 0.9f})
        .add<UIGrid>("items")
            .setColumns(5)
            .setSpacing<Pixels>(10)
            .setItemSize<Pixels>(64, 64)
        .add<UIText>("title")
            .setText("Inventory")
            .setFont("fonts/roboto.ttf")
            .setFontSize<Pixels>(24)
            .setAlignment(TextAlign::Center);

// Example 6: Animation state machine with type-safe transitions
auto& animator = entity.add<Animator>()
    .addState<AnimationState>("idle")
        .setClip("animations/idle.anim")
        .setSpeed(1.0f)
        .setLoop(true)
    .addState<AnimationState>("walk")
        .setClip("animations/walk.anim")
        .setSpeed(1.2f)
        .setLoop(true)
    .addTransition<BlendTransition>("idle", "walk")
        .setDuration(0.2f)
        .setCondition([](const AnimationContext& ctx) {
            return ctx.getSpeed() > 0.1f;
        });

// Example 7: Particle system with templated emitters and modifiers
auto& particles = Scene::current().create<ParticleSystem>()
    .setMaxParticles(1000)
    .addEmitter<SphereEmitter>()
        .setRadius(1.0f)
        .setEmissionRate(50.0f)
        .setInitialVelocity(2.0f)
    .addModifier<ColorOverLife>()
        .addKeyframe(0.0f, Color{1, 0, 0, 1})
        .addKeyframe(1.0f, Color{1, 0, 0, 0})
    .addModifier<SizeOverLife>()
        .addKeyframe(0.0f, 1.0f)
        .addKeyframe(1.0f, 0.0f)
    .addModifier<VelocityOverLife>()
        .setGravity(Vector3{0, -9.81f, 0});

// Example 8: Network replication with type safety
auto& netObj = Scene::current().create<NetworkObject>("player")
    .addReplicatedVar<Vector3>("position")
        .setUpdateRate(60)
        .setInterpolation(InterpolationType::Linear)
    .addReplicatedVar<Quaternion>("rotation")
        .setUpdateRate(20)
        .setInterpolation(InterpolationType::Spherical)
    .addRPC<void(const Vector3&)>("OnPositionChanged")
        .setReliable(true)
        .setHandler([](const Vector3& newPos) {
            // Handle position change
        });

#endif