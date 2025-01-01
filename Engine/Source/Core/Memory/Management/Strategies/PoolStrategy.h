#pragma once
#include "Management/IMemoryStrategy.h"

namespace Hydragon {
namespace Memory {

class PoolStrategy : public IMemoryStrategy {
public:
    struct PoolConfig {
        size_t smallPoolSize = 64;
        size_t mediumPoolSize = 256;
        size_t poolCount = 16;
    };
    
    explicit PoolStrategy(const PoolConfig& config = {});
    
    void* allocate(size_t size) override;
    void deallocate(void* ptr) override;
    
private:
    struct Pool {
        void* memory;
        size_t blockSize;
        size_t blockCount;
        std::vector<bool> used;
    };
    
    std::vector<Pool> m_Pools;
    PoolConfig m_Config;
};

}} // namespace Hydragon::Memory 