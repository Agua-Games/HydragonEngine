/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core procedural generation system
 */

#pragma once
#include "ProceduralTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::Procedural {

class ProceduralSystem {
public:
    struct ProceduralConfig {
        bool enableMultithreading = true;
        bool enableCaching = true;
        bool enableProgressiveGeneration = true;
        uint32_t maxGenerators = 100;
        uint32_t maxConcurrentJobs = 8;
        size_t memoryBudget = 256 * 1024 * 1024;  // 256MB
        uint32_t seedOffset = 0;
    };

    static ProceduralSystem& Get();
    
    void Initialize(const ProceduralConfig& config = {});
    void Shutdown();
    
    void Update();
    
    GeneratorHandle CreateGenerator(const GeneratorDesc& desc);
    void DestroyGenerator(GeneratorHandle handle);
    
    void SetSeed(GeneratorHandle handle, uint64_t seed);
    void SetParameters(GeneratorHandle handle, const GeneratorParams& params);
    
    void Generate(GeneratorHandle handle, const GenerationContext& context);
    void CancelGeneration(GeneratorHandle handle);
    
    bool IsGenerating(GeneratorHandle handle) const;
    float GetProgress(GeneratorHandle handle) const;
    
    void RegisterCallback(GeneratorHandle handle, const GenerationCallback& callback);
    void UnregisterCallback(GeneratorHandle handle);
    
    const ProceduralStats& GetStats() const { return m_Stats; }

private:
    ProceduralSystem() = default;
    
    ProceduralConfig m_Config;
    ProceduralStats m_Stats;
    std::vector<Generator> m_Generators;
    std::unique_ptr<IJobScheduler> m_Scheduler;
    bool m_Initialized = false;
};

} // namespace Hydragon::Procedural 