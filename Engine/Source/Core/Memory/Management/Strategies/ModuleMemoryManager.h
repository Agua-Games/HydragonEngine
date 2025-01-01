/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Module memory manager for Hydragon
 */

#pragma once
#include "Management/IModuleMemoryManager.h"
#include "Types/MemoryTypes.h"

namespace Hydragon {
namespace Memory {

class ModuleMemoryManager : public IModuleMemoryManager {
public:
    ModuleMemoryManager(const std::string& moduleName, 
                       IMemoryStrategy* strategy);

    // IModuleMemoryManager interface
    void setMemoryLimit(size_t limit) override;
    void setPriority(ModulePriority priority) override;
    void* allocate(size_t size, const AllocationInfo& info) override;
    void deallocate(void* ptr) override;
    size_t getCurrentUsage() const override;
    bool isUnderPressure() const override;

private:
    std::string m_ModuleName;
    IMemoryStrategy* m_Strategy;
    size_t m_MemoryLimit;
    size_t m_CurrentUsage;
    ModulePriority m_Priority;
    
    void checkMemoryPressure();
    bool handleAllocationFailure(size_t size);
};

}} // namespace Hydragon::Memory 