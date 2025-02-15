struct HD_ParticleCollisionInfo : public HD_NodeInfo {
    HD_ParticleCollisionInfo() {
        NodeType = "Particles/Collision";
        
        Inputs = {
            "Particles",        // From ForcesNode
            "CollisionMesh",    // Scene geometry
            "Response",         // Collision response
            "Friction",         // Surface friction
            "Damping",         // Energy loss
            "SubSteps"         // Simulation quality
        };
        
        Outputs = {
            "FinalParticles",   // Post-collision state
            "Impacts",          // Collision events
            "Fragments",        // Break-up particles
            "SurfaceEffects"    // Surface interaction
        };
    }
};