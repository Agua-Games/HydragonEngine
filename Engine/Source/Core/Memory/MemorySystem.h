/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory system for Hydragon
 */

#pragma once
#include <cstddef>
#include <memory>
#include <string>
#include <unordered_map>

// Third party includes
#include <fmt/format.h>

// Local includes
#include "Types/MemoryTypes.h"
#include "Management/IMemoryStrategy.h"
#include "CrazyStuff.h"

namespace Hydragon::Memory {

/**
 * Core memory system for managing allocations
 */
class MemorySystem {
public:
    /**
     * Get singleton instance of memory system
     * @return Reference to memory system
     */
    static MemorySystem& get();
    
    /**
     * Initialize memory system with configuration
     * @param config Configuration settings
     */
    void initialize(const MemoryConfig& config = {});
    /**
     * Shuts down the memory system and cleans up resources
     */
    void shutdown();
    
    /**
     * Allocates memory with specified parameters
     * @param size Size in bytes to allocate
     * @param info Additional allocation parameters
     * @return Pointer to allocated memory or nullptr on failure
     * @throws std::bad_alloc if allocation fails
     */
    void* allocate(size_t size, const AllocationInfo& info = {});
    /**
     * Deallocates previously allocated memory
     * @param ptr Pointer to memory to deallocate
     */
    void deallocate(void* ptr);
    
    // Memory management
    void addStrategy(const std::string& name, IMemoryStrategy* strategy);
    void setDefaultStrategy(const std::string& name);
    
    // System state
    MemoryStats getSystemStats() const;
    bool isInitialized() const { return m_Initialized; }

private:
    MemorySystem() = default;
    
    bool m_Initialized = false;
    std::unordered_map<std::string, std::unique_ptr<IMemoryStrategy>> m_Strategies;
    IMemoryStrategy* m_DefaultStrategy = nullptr;
};

} // namespace Hydragon::Memory 