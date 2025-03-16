using namespace hd;

// Example 1: Particle system chain
auto particles = NodeGraph::Create("effects")
    .add<ParticleEmitterNode>("mainEmitter")
        .emissionRate(50.0f, runtime)
        .initialVelocity({0.0f, 1.0f, 0.0f})
        .particleLifetime(2.0f, runtime)
        .connect("output", "modifier1.input")
    .add<ParticleModifierNode>("modifier1")
        .mode(ParticleModifierMode::Scale)
        .scaleCurve({
            {0.0f, 1.0f},
            {1.0f, 0.0f}
        })
        .connect("output", "renderer.input");

// Example 2: Audio processing chain
auto audio = NodeGraph::Create("audio")
    .add<AudioSourceNode>("musicTrack")
        .volume(0.8f, runtime)
        .pitch(1.0f, runtime)
        .connect("output", "reverb.input")
    .add<ReverbNode>("reverb")
        .roomSize(0.7f, runtime)
        .damping(0.3f, runtime)
        .connect("output", "compressor.input")
    .add<CompressorNode>("compressor")
        .threshold(-12.0f, runtime)
        .ratio(4.0f, runtime);

// Example 3: Post-processing chain
auto post = NodeGraph::Create("post")
    .add<BloomNode>("bloom")
        .intensity(1.0f, runtime)
        .threshold(1.0f, runtime)
        .connect("output", "colorGrade.input")
    .add<ColorGradingNode>("colorGrade")
        .contrast({1.0f, 1.0f, 1.0f}, runtime)
        .saturation(1.0f, runtime);