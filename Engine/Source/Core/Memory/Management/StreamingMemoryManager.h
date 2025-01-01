#pragma once
#include "IMemoryStrategy.h"
#include "Core/Streaming/IStreamingSystem.h"

namespace Hydragon {
namespace Memory {

class StreamingMemoryManager {
public:
    struct StreamingConfig {
        size_t reservedStreamingMemory;
        size_t maxStreamingBlocks;
        bool enablePrioritization;
        float evictionThreshold;
    };

    // Integration with core systems
    void registerStreamingSystem(IStreamingSystem* system);
    void setMemoryStrategy(IMemoryStrategy* strategy);

    // Cross-module coordination
    void registerModule(const std::string& moduleName, size_t reservedMemory);
    void setModulePriority(const std::string& moduleName, StreamingPriority priority);

    // Memory operations with streaming awareness
    void* allocateStreamingMemory(size_t size, const StreamingAllocationInfo& info);
    void deallocateStreamingMemory(void* ptr);

    // Streaming state management
    void updateStreamingState();
    bool needsEviction() const;
    
private:
    struct ModuleInfo {
        size_t reservedMemory;
        size_t currentUsage;
        StreamingPriority priority;
        std::vector<StreamingBlock*> blocks;
    };

    std::unordered_map<std::string, ModuleInfo> m_Modules;
    IStreamingSystem* m_StreamingSystem;
    IMemoryStrategy* m_Strategy;
};

}} // namespace Hydragon::Memory 