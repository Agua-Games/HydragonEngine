struct HD_EmitterInfo : public HD_NodeInfo {
    HD_EmitterInfo() {
        NodeType = "Particles/Emitter";
        
        Inputs = {
            "Shape",            // Emission shape
            "Rate",            // Emission rate
            "InitialState",    // Initial conditions
            "Variation",       // Property variation
            "Lifetime",        // Particle lifetime
            "Seed"            // Random seed
        };
        
        Outputs = {
            "ParticleStream",   // Emitted particles
            "EmissionEvents",   // Emission timing
            "SystemState",      // Emitter state
            "Statistics"        // Performance stats
        };
    }
};