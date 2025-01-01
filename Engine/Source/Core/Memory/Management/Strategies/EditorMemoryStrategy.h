#pragma once
#include "IMemoryStrategy.h"
#include <array>

namespace Hydragon {
namespace Memory {

// Specialized allocator for Editor UI components
class EditorMemoryStrategy : public IMemoryStrategy {
public:
    struct Config {
        size_t uiElementPoolSize = 1024 * 1024;  // 1MB for UI elements
        size_t commandBufferSize = 512 * 1024;   // 512KB for undo/redo
        bool enableDefragmentation = true;
        uint32_t defragInterval = 5000;          // ms between defrag passes
    };

    explicit EditorMemoryStrategy(const Config& config = {});

    // Specialized allocation for UI elements
    struct UIAllocationHint {
        bool isLongLived;        // Widgets that stay alive
        bool isFrequentlyUsed;   // Elements accessed often
        bool needsQuickAccess;   // For responsive UI
    };

private:
    // Separate pools for different UI element lifetimes
    struct UIPool {
        void* memory;
        size_t size;
        size_t used;
        bool isLongLived;
    };

    std::array<UIPool, 3> m_UIPools;  // Short, medium, long-lived
    
    // Circular buffer for undo/redo operations
    struct CommandBuffer {
        void* buffer;
        size_t capacity;
        size_t head;
        size_t tail;
    } m_CommandBuffer;

    // Defragmentation support
    void defragmentIfNeeded();
    uint64_t m_LastDefragTime;
};

}} // namespace Hydragon::Memory 