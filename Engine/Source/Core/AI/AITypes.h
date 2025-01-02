/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * AI system type definitions
 */

#pragma once
#include "Core/Math/Vector3.h"
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::AI {

using AgentHandle = uint32_t;
using BehaviorHandle = uint32_t;

enum class AgentType {
    Character,
    Vehicle,
    Flying,
    Custom
};

struct AgentDesc {
    AgentType type;
    float radius = 0.5f;
    float height = 2.0f;
    float maxSpeed = 5.0f;
    float maxAcceleration = 10.0f;
    bool avoidance = true;
    std::string behaviorPath;
};

struct PerceptionEvent {
    enum class Type {
        Visual,
        Audio,
        Damage,
        Custom
    };
    
    Type type;
    Vector3 position;
    float intensity;
    float duration;
};

struct AIStats {
    uint32_t activeAgents;
    uint32_t activeBehaviors;
    uint32_t pathRequestsPerFrame;
    float averageUpdateTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::AI 