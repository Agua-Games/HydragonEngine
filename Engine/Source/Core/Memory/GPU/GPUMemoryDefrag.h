/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * GPU memory defragmentation for Hydragon
 */

#pragma once
#include "GPUMemoryManager.h"

namespace Hydragon {
namespace Memory {

class GPUMemoryDefrag {
public:
    struct DefragConfig {
        size_t minBlockSize;
        float fragmentationThreshold;
        bool enableLiveDefrag;
    };
    
    void BeginDefrag(const DefragConfig& config);
    void UpdateDefrag();
    bool IsDefragNeeded() const;
    
private:
    struct DefragOperation {
        GPUAllocation source;
        GPUAllocation destination;
        size_t size;
        bool inProgress;
    };
    
    std::vector<DefragOperation> m_PendingOps;
    DefragConfig m_Config;
};

}} // namespace Hydragon::Memory 