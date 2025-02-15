struct HD_ParticleForcesInfo : public HD_NodeInfo {
    HD_ParticleForcesInfo() {
        NodeType = "Particles/Forces";
        
        Inputs = {
            "Particles",        // From EmitterNode
            "Gravity",         // Gravity force
            "Wind",            // Wind force
            "Turbulence",      // Turbulent forces
            "Attractors",      // Point attractors
            "Fields"           // Force fields
        };
        
        Outputs = {
            "ModifiedParticles", // Updated particles
            "ForceMetrics",      // Force magnitudes
            "EnergyState",       // System energy
            "Trajectories"       // Motion paths
        };
    }
};