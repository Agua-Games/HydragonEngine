#pragma once
#include "MemoryBlock.h"
#include <vector>

namespace Hydragon {
namespace Memory {

class Defragmenter {
public:
    struct DefragStats {
        size_t fragmentedBytes;
        size_t recoveredBytes;
        size_t movedBlocks;
        double fragmentationRatio;
    };

    DefragStats analyze() {
        DefragStats stats{};
        
        // Calculate fragmentation
        size_t totalGaps = 0;
        size_t maxContiguous = 0;
        
        for (size_t i = 1; i < m_Blocks.size(); ++i) {
            size_t gap = getGapSize(m_Blocks[i-1], m_Blocks[i]);
            totalGaps += gap;
            maxContiguous = std::max(maxContiguous, 
                                   getContiguousSize(m_Blocks[i]));
        }
        
        stats.fragmentedBytes = totalGaps;
        stats.fragmentationRatio = static_cast<double>(totalGaps) / 
                                 m_TotalMemory;
        return stats;
    }

    void defragment() {
        if (!needsDefragmentation()) return;

        // Sort blocks by address
        std::sort(m_Blocks.begin(), m_Blocks.end(),
                 [](const MemoryBlock& a, const MemoryBlock& b) {
                     return a.ptr < b.ptr;
                 });

        // Compact blocks
        void* target = m_HeapStart;
        for (auto& block : m_Blocks) {
            if (block.ptr != target) {
                moveBlock(block, target);
            }
            target = static_cast<char*>(target) + block.size;
        }
    }

private:
    bool needsDefragmentation() {
        auto stats = analyze();
        return stats.fragmentationRatio > 0.2;  // 20% fragmentation threshold
    }

    void moveBlock(MemoryBlock& block, void* newLocation) {
        // Update pointers if needed
        if (block.hasPointers) {
            updatePointers(block, newLocation);
        }
        
        // Move memory
        std::memmove(newLocation, block.ptr, block.size);
        block.ptr = newLocation;
    }
};

}} // namespace Hydragon::Memory 