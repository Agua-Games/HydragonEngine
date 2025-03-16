// Clean, fluent style for node creation
auto scene = graph.create<SceneNode>("mainScene")
    .skybox("sky/hdri.exr")
    .ambient({0.1f, 0.1f, 0.1f})
    .fog(true)
    .fogDensity(0.01f);

// Add child with transform
auto camera = scene.add<CameraNode>("mainCam")
    .position({0, 2, -10})
    .target({0, 0, 0})
    .fov(75.0f)
    .near(0.1f)
    .far(1000.0f);

// Complex setup using setters where needed
auto physics = scene.add<PhysicsNode>("dynamic")
    .position({0, 5, 0})
    .mass(1.0f)
    .setCollider<SphereShape>(0.5f)      // Complex setup
    .setMaterial("bouncy")               // Resource loading
    .onCollision([](auto& other) {       // Event handler
        // Handle collision
    });

// The same node can be accessed traditionally elsewhere
if (physics.getMass() > 10.0f) {
    physics.setCollider<BoxShape>({1,1,1});
}