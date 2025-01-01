#pragma once
#include "Management/IMemoryStrategy.h"
#include <atomic>
#include <array>

namespace Hydragon {
namespace Memory {

// Thread-local cache for fast allocations
class ThreadLocalCache {
public:
    static constexpr size_t CACHE_BUCKETS = 32;
    static constexpr size_t BUCKET_SIZE = 256;  // Objects per bucket

    struct Bucket {
        std::atomic<void*> head{nullptr};
        std::atomic<size_t> count{0};
        size_t blockSize;
        char padding[64 - sizeof(std::atomic<void*>) - sizeof(std::atomic<size_t>) - sizeof(size_t)];  // Prevent false sharing
    };

    std::array<Bucket, CACHE_BUCKETS> buckets;
    thread_local static ThreadLocalCache* current;
};

// Lock-free allocator for small allocations
class ConcurrentAllocator : public IMemoryStrategy {
public:
    struct Config {
        size_t maxThreads = 64;
        size_t cacheSize = 1024 * 1024;  // 1MB per thread
        bool enableThreadCache = true;
    };

    explicit ConcurrentAllocator(const Config& config = {});

    void* allocate(size_t size, const AllocationInfo& info) override;
    void deallocate(void* ptr) override;
    bool owns(void* ptr) const override;

private:
    static constexpr size_t CENTRAL_BUFFER_SIZE = 1024 * 1024 * 64;  // 64MB
    
    struct alignas(64) CentralBuffer {  // Align to prevent false sharing
        std::atomic<void*> head;
        std::atomic<size_t> count;
    };

    std::vector<CentralBuffer> m_CentralBuffers;
    std::vector<ThreadLocalCache> m_ThreadCaches;
    Config m_Config;

    void* allocateFromCache(size_t size);
    void* allocateFromCentral(size_t size);
    bool returnToCache(void* ptr, size_t size);
};

}} // namespace Hydragon::Memory 