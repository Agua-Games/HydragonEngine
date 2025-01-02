/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Entity Component System manager
 */

#pragma once
#include "EntityTypes.h"
#include "ComponentPool.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <typeindex>

namespace Hydragon::ECS {

class EntityManager {
public:
    struct EntityConfig {
        uint32_t initialEntityCapacity = 1000;
        uint32_t maxComponents = 64;
        bool enableComponentPools = true;
        size_t poolMemoryBudget = 64 * 1024 * 1024;  // 64MB
    };

    explicit EntityManager(const EntityConfig& config = {});
    ~EntityManager();

    EntityHandle CreateEntity();
    void DestroyEntity(EntityHandle entity);
    
    template<typename T>
    T* AddComponent(EntityHandle entity);
    
    template<typename T>
    void RemoveComponent(EntityHandle entity);
    
    template<typename T>
    T* GetComponent(EntityHandle entity);
    
    const EntityStats& GetStats() const { return m_Stats; }

private:
    EntityConfig m_Config;
    EntityStats m_Stats;
    std::unordered_map<std::type_index, std::unique_ptr<IComponentPool>> m_ComponentPools;
    std::vector<EntityData> m_Entities;
    std::vector<EntityHandle> m_FreeList;
};

} // namespace Hydragon::ECS 