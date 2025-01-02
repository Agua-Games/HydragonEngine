/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene management system
 */

#pragma once
#include "SceneTypes.h"
#include "Core/ECS/EntityManager.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <string>

namespace Hydragon::Scene {

class SceneSystem {
public:
    struct SceneConfig {
        uint32_t maxScenes = 8;
        uint32_t maxEntitiesPerScene = 100000;
        bool enableSceneStreaming = true;
        bool enableAsyncLoading = true;
        size_t sceneMemoryBudget = 1024 * 1024 * 1024;  // 1GB
    };

    static SceneSystem& Get();
    
    void Initialize(const SceneConfig& config = {});
    void Shutdown();

    SceneHandle CreateScene(const SceneDesc& desc);
    void DestroyScene(SceneHandle handle);
    
    void LoadScene(const std::string& path, const LoadSceneParams& params = {});
    void UnloadScene(SceneHandle handle);
    
    ECS::EntityManager* GetEntityManager(SceneHandle handle);
    const SceneStats& GetStats() const { return m_Stats; }

private:
    SceneSystem() = default;
    
    SceneConfig m_Config;
    SceneStats m_Stats;
    std::unordered_map<SceneHandle, std::unique_ptr<ECS::EntityManager>> m_Scenes;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scene 