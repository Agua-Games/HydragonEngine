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