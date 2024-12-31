#pragma once

namespace Hydragon {

class IMemoryStrategy {
public:
    virtual ~IMemoryStrategy() = default;
    
    virtual void* Allocate(size_t size, size_t alignment) = 0;
    virtual void Deallocate(void* ptr) = 0;
    
    // Future expansion hooks
    virtual void OnLowMemory() {}
    virtual void OnDefragmentRequest() {}
};

} // namespace Hydragon 