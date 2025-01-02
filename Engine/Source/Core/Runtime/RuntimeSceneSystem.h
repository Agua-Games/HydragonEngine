/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime scene management and streaming
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeSceneConfig {
    bool enableAsyncLoading = true;
    bool enableStreamingLevels = true;
    bool enableSceneTransitions = true;
    bool enablePersistentObjects = true;
    uint32_t maxActiveScenes = 4;
    uint32_t streamingBudgetMB = 512;
    float unloadDelay = 5.0f;
    std::string defaultScene = "MainMenu";
};

class RuntimeSceneSystem {
public:
    static RuntimeSceneSystem& Get();
    
    void Initialize(const RuntimeSceneConfig& config = {});
    void Shutdown();
    
    void Update();
    
    SceneHandle LoadScene(const std::string& name, const SceneLoadParams& params = {});
    void UnloadScene(SceneHandle handle);
    
    void StreamLevel(const std::string& name, const Vector3& origin);
    void UnstreamLevel(const std::string& name);
    
    void SetActiveScene(SceneHandle handle);
    void MergeScenes(SceneHandle target, SceneHandle source);
    
    void RegisterPersistentObject(EntityHandle entity);
    void UnregisterPersistentObject(EntityHandle entity);
    
    SceneState GetSceneState(SceneHandle handle) const;
    const RuntimeSceneStats& GetStats() const { return m_Stats; }

private:
    RuntimeSceneSystem() = default;
    
    RuntimeSceneConfig m_Config;
    RuntimeSceneStats m_Stats;
    std::unordered_map<SceneHandle, SceneInstance> m_Scenes;
    std::vector<EntityHandle> m_PersistentObjects;
    std::unique_ptr<ISceneLoader> m_Loader;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 