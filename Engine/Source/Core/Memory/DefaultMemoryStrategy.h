/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Default memory strategy for Hydragon
 */

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