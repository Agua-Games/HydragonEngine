/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Scene {

using SceneHandle = uint64_t;

struct SceneDesc {
    std::string name;
    bool isPersistent = false;
    bool isEditorScene = false;
    std::vector<std::string> requiredSystems;
};

struct LoadSceneParams {
    bool asyncLoad = true;
    bool additive = false;
    bool keepLoadedInMemory = false;
    uint32_t priority = 0;
};

struct SceneStats {
    uint32_t activeScenes;
    uint32_t totalEntities;
    uint32_t totalComponents;
    size_t memoryUsed;
    float averageUpdateTimeMs;
};

} // namespace Hydragon::Scene 