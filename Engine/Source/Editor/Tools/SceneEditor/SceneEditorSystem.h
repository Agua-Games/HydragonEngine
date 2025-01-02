/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene editing and manipulation system
 */

#pragma once
#include "SceneTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Editor {

struct SceneEditorConfig {
    bool enableGizmos = true;
    bool enableSnapping = true;
    bool enableLayers = true;
    bool enablePrefabs = true;
    uint32_t maxUndoOperations = 100;
    float gridSize = 1.0f;
    Vector3 snapIncrement = {0.5f, 0.5f, 0.5f};
};

class SceneEditorSystem {
public:
    static SceneEditorSystem& Get();
    
    void Initialize(const SceneEditorConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void CreateEntity(const EntityDesc& desc);
    void DeleteEntity(EntityHandle handle);
    void DuplicateEntity(EntityHandle handle);
    
    void SetSelection(const std::vector<EntityHandle>& entities);
    void ClearSelection();
    
    void BeginTransform(TransformMode mode);
    void UpdateTransform(const Transform& delta);
    void EndTransform();
    
    void CreatePrefab(const std::string& name, EntityHandle root);
    void InstantiatePrefab(const std::string& name, const Transform& transform);
    
    const SceneEditorStats& GetStats() const { return m_Stats; }

private:
    SceneEditorSystem() = default;
    
    SceneEditorConfig m_Config;
    SceneEditorStats m_Stats;
    std::vector<EntityHandle> m_Selection;
    std::unique_ptr<ISceneManipulator> m_Manipulator;
    bool m_Initialized = false;
};

} // namespace Hydragon::Editor 