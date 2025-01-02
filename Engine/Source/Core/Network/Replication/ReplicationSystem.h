/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Entity replication and state synchronization system
 */

#pragma once
#include "../NetworkTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Network {

class ReplicationSystem {
public:
    struct ReplicationConfig {
        uint32_t maxEntities = 10000;
        uint32_t maxPropertiesPerEntity = 100;
        bool enableDeltaCompression = true;
        bool enableInterpolation = true;
        bool enableExtrapolation = true;
        float updateFrequency = 20.0f;  // 20Hz
    };

    static ReplicationSystem& Get();
    
    void Initialize(const ReplicationConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterEntity(EntityHandle entity, const EntityDesc& desc);
    void UnregisterEntity(EntityHandle entity);
    
    void SetReplicationState(EntityHandle entity, const EntityState& state);
    EntityState GetReplicationState(EntityHandle entity) const;
    
    void SetInterpolationConfig(EntityHandle entity, const InterpolationConfig& config);
    void EnablePrediction(EntityHandle entity, bool enable);
    
    const ReplicationStats& GetStats() const { return m_Stats; }

private:
    ReplicationSystem() = default;
    
    ReplicationConfig m_Config;
    ReplicationStats m_Stats;
    std::unique_ptr<IReplicationProvider> m_Provider;
    std::vector<ReplicatedEntity> m_Entities;
    bool m_Initialized = false;
};

} // namespace Hydragon::Network 