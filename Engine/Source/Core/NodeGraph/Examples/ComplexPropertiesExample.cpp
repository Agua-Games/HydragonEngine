auto physics = graph.create<PhysicsNode>("rigidBody")
    .mass(1.0f)                          // Simple property
    .friction(0.5f)                      // Simple property
    .setCollider<BoxShape>({1,1,1})      // Complex setup needs setter
    .setMaterial("metal")                // Needs resource loading
    .addForce({0, -9.81f, 0});          // Action method