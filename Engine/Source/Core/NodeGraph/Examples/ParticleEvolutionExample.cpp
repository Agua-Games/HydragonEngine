#include "Core/Particles/ParticleSystem.h"
#include "Core/Procedural/ProceduralOrchestrator.h"

namespace hd {

class ParticleEvolutionExample {
public:
    void evolveParticleSystem() {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        
        // Create base particle system
        auto particleSystem = std::make_shared<ParticleSystem>();
        
        // Setup evolution parameters
        OctaveParams evolutionParams;
        evolutionParams.complexity = 0.7f;
        evolutionParams.coherence = 0.8f;
        evolutionParams.variation = 0.5f;

        // Register for procedural evolution
        auto pattern = orchestrator.registerPattern(particleSystem);
        
        // Evolve the system
        pattern->setIntent("VisualComplexity", evolutionParams);
        
        // Process evolution
        orchestrator.ProcessNodeGraph();
        
        // The particle system will now evolve based on:
        // 1. System harmony requirements
        // 2. Performance constraints
        // 3. Visual complexity targets
        // 4. Global evolution parameters
    }

    void handleEvolutionFeedback() {
        auto& orchestrator = ProceduralOrchestrator::GetInstance();
        
        // Get evolution metrics
        auto metrics = orchestrator.GetOutputValue<EvolutionMetrics>("EvolutionMetrics");
        
        // Adjust based on feedback
        if (metrics.complexityScore > 0.8f) {
            // Reduce complexity
            OctaveParams newParams;
            newParams.complexity = 0.6f;
            orchestrator.ModulateSystem(SystemDomain::Particles, newParams);
        }
    }
};

} // namespace hd