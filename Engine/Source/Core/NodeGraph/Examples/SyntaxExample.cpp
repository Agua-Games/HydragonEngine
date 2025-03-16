auto node = graph.create<TransformNode>("worldRoot")
    .position({0, 5, -10})
    .rotation({0, 45, 0})
    .scale(2.0f)
    .visible(true);

auto material = graph.create<MaterialNode>("metal")
    .baseColor({0.8f, 0.8f, 0.8f})
    .roughness(0.5f)
    .metallic(1.0f)
    .albedoMap("textures/metal_albedo.png")
    .normalMap("textures/metal_normal.png");