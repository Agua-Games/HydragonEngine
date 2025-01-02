/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Replication system type definitions
 */

#pragma once
#include "Core/Math/Transform.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Network {

struct EntityDesc {
    std::string name;
    uint32_t classId;
    bool reliable = true;
    bool predictable = false;
    float priority = 1.0f;
};

struct EntityState {
    Transform transform;
    std::vector<uint8_t> customData;
    uint32_t sequence;
    float timestamp;
};

struct InterpolationConfig {
    float delay = 0.1f;
    float smoothing = 0.5f;
    bool extrapolate = true;
    uint32_t maxExtrapolationTicks = 10;
};

struct ReplicationStats {
    uint32_t replicatedEntities;
    uint32_t stateUpdates;
    uint32_t bytesReplicated;
    float updateTimeMs;
    float interpolationDelay;
    size_t memoryUsed;
};

} // namespace Hydragon::Network 