#include "EditorMemoryStrategy.h"
#include "NUMA/NumaAwareAllocator.h"

namespace Hydragon {
namespace Memory {

class EditorMemoryStrategy::Impl {
public:
    Impl(const Config& config) 
        : m_NumaAllocator(std::make_unique<NumaAwareAllocator>())
        , m_Config(config) {
        
        initializePools();
    }

    void* allocateUIElement(size_t size, const UIAllocationHint& hint) {
        NumaAwareAllocator::EditorNumaHint numaHint{
            .workload = NumaAwareAllocator::EditorNumaHint::WorkloadType::UI,
            .isPersistent = hint.isLongLived,
            .isHotPath = hint.needsQuickAccess
        };

        if (hint.isFrequentlyUsed) {
            // Use thread-local cache for frequently used UI elements
            return allocateFromCache(size);
        }

        return m_NumaAllocator->allocateForEditor(size, numaHint);
    }

    void* allocateAssetData(size_t size, bool isStreaming) {
        NumaAwareAllocator::EditorNumaHint numaHint{
            .workload = NumaAwareAllocator::EditorNumaHint::WorkloadType::AssetProcessing,
            .isPersistent = !isStreaming,
            .isHotPath = false
        };

        return m_NumaAllocator->allocateForEditor(size, numaHint);
    }

private:
    std::unique_ptr<NumaAwareAllocator> m_NumaAllocator;
    Config m_Config;
    
    // Thread-local UI element cache
    struct UICache {
        static constexpr size_t CACHE_SIZE = 1024 * 1024;  // 1MB per thread
        char* memory;
        size_t used;
    };
    
    thread_local static UICache* t_Cache;
};

}} // namespace Hydragon::Memory 