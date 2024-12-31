#pragma once
#include "IMemoryStrategy.h"

namespace Hydragon {

class DefaultMemoryStrategy : public IMemoryStrategy {
public:
    void* Allocate(size_t size, size_t alignment) override;
    void Deallocate(void* ptr) override;
    
    void OnLowMemory() override;
    void OnDefragmentRequest() override;
};

} // namespace Hydragon 