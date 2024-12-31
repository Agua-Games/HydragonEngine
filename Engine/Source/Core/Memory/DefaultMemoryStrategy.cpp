#include "DefaultMemoryStrategy.h"
#include <cstdlib>

namespace Hydragon {

void* DefaultMemoryStrategy::Allocate(size_t size, size_t alignment) {
    void* ptr = nullptr;
    
    #if defined(_MSC_VER)
        ptr = _aligned_malloc(size, alignment);
    #else
        int result = posix_memalign(&ptr, alignment, size);
        if (result != 0) ptr = nullptr;
    #endif
    
    return ptr;
}

void DefaultMemoryStrategy::Deallocate(void* ptr) {
    #if defined(_MSC_VER)
        _aligned_free(ptr);
    #else
        free(ptr);
    #endif
}

void DefaultMemoryStrategy::OnLowMemory() {
    // Future: Implement memory pressure handling
}

void DefaultMemoryStrategy::OnDefragmentRequest() {
    // Future: Implement memory defragmentation
}

} // namespace Hydragon 