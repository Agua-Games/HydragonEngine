/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Entity Component System type definitions
 */

#pragma once
#include <cstdint>
#include <bitset>
#include <vector>

namespace Hydragon::ECS {

using EntityHandle = uint32_t;
using ComponentMask = std::bitset<64>;

struct EntityData {
    ComponentMask components;
    bool active = true;
    uint32_t version = 0;
};

struct EntityStats {
    uint32_t activeEntities;
    uint32_t totalComponents;
    uint32_t componentTypes;
    size_t memoryUsed;
    float averageIterationTimeMs;
};

struct ComponentTypeInfo {
    size_t size;
    size_t alignment;
    std::string name;
    bool isPooled;
};

} // namespace Hydragon::ECS 