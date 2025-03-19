#include "MiniPhysicsSolver.h"

namespace hd {

void MiniPhysicsSolver::step(float deltaTime) {
    // 1. Update wave functions
    for (auto& wave : m_waveFunctions) {
        // Transform continuous wave representation to discrete particles
        wave.updateFromField(m_currentField);
    }
    
    // 2. Propagate energy through field
    propagateWaves();
    
    // 3. Update adaptive grid based on energy density
    updateAdaptiveGrid();
    
    // 4. Solve constraints in reduced dimensions where possible
    solveConstraints();
    
    // 5. Compress and quantize results
    if (m_config.useWaveletCompression) {
        compressWavelets();
    }
    quantizeValues();
}

void MiniPhysicsSolver::propagateWaves() {
    // Use exponential encoding for efficient energy propagation
    // Base-10 exponential encoding allows for huge range with minimal bits
    for (size_t i = 0; i < m_adaptiveGrid->getCellCount(); ++i) {
        auto& cell = m_adaptiveGrid->getCell(i);
        
        // Reduce 3D propagation to 2D where possible
        if (cell.energyGradient.isMainlyPlanar()) {
            propagateInPlane(cell);
        } else {
            propagateIn3D(cell);
        }
    }
}

void MiniPhysicsSolver::updateAdaptiveGrid() {
    // Refine grid based on energy density and gradient
    m_adaptiveGrid->refine([](const VoxelCell& cell) {
        return cell.energyDensity > ENERGY_THRESHOLD &&
               cell.gradientMagnitude > GRADIENT_THRESHOLD;
    });
}

vec3 MiniPhysicsSolver::projectToSurface(const vec3& point, const vec3& normal) {
    // Project point onto surface defined by normal
    // Use local space for better precision
    LocalSpace space;
    space.normal = normal;
    space.computeBasis();  // Generates tangent and bitangent
    
    vec3 localPos = space.toLocal(point);
    localPos.z = 0.0f;  // Project to surface
    return space.toWorld(localPos);
}

} // namespace hd