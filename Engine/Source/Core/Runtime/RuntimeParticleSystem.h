/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime particle effects and simulation management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeParticleConfig {
    bool enableGPUSimulation = true;
    bool enableInstancing = true;
    bool enableLOD = true;
    bool enableCollision = true;
    uint32_t maxActiveSystems = 1000;
    uint32_t maxParticlesPerSystem = 10000;
    uint32_t updateBatchSize = 1024;
    float cullingDistance = 100.0f;
    std::string effectCachePath = "Cache/Particles";
};

class RuntimeParticleSystem {
public:
    static RuntimeParticleSystem& Get();
    
    void Initialize(const RuntimeParticleConfig& config = {});
    void Shutdown();
    
    void Update();
    
    ParticleSystemHandle CreateSystem(const ParticleSystemDesc& desc);
    void DestroySystem(ParticleSystemHandle handle);
    
    void EmitParticles(ParticleSystemHandle handle, uint32_t count = 1);
    void StopEmission(ParticleSystemHandle handle);
    void KillParticles(ParticleSystemHandle handle);
    
    void SetEmissionRate(ParticleSystemHandle handle, float particlesPerSecond);
    void SetLifetime(ParticleSystemHandle handle, float minLife, float maxLife);
    void SetVelocity(ParticleSystemHandle handle, const Vector3& velocity);
    
    void AddForceField(const std::string& name, const ForceFieldDesc& desc);
    void RemoveForceField(const std::string& name);
    void UpdateForceField(const std::string& name, const ForceFieldParams& params);
    
    void SetSystemTransform(ParticleSystemHandle handle, const Transform& transform);
    void SetSimulationSpace(ParticleSystemHandle handle, SimulationSpace space);
    
    void RegisterCollisionHandler(ParticleSystemHandle handle, const CollisionHandler& handler);
    void UnregisterCollisionHandler(ParticleSystemHandle handle);
    
    ParticleSystemState GetSystemState(ParticleSystemHandle handle) const;
    uint32_t GetActiveParticleCount(ParticleSystemHandle handle) const;
    const RuntimeParticleStats& GetStats() const { return m_Stats; }

private:
    RuntimeParticleSystem() = default;
    
    RuntimeParticleConfig m_Config;
    RuntimeParticleStats m_Stats;
    std::unordered_map<ParticleSystemHandle, ParticleSystemInstance> m_Systems;
    std::unordered_map<std::string, ForceField> m_ForceFields;
    std::unordered_map<ParticleSystemHandle, CollisionHandler> m_CollisionHandlers;
    std::unique_ptr<IParticleSimulator> m_Simulator;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 