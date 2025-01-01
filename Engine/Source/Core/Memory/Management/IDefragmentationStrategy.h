#pragma once
#include "MemoryBlock.h"
#include <vector>

namespace Hydragon {
namespace Memory {

struct DefragmentationStats {
    size_t fragmentedBlocks;
    size_t movedBlocks;
    size_t totalMoves;
    double fragmentationRatio;
};

class IDefragmentationStrategy {
public:
    virtual ~IDefragmentationStrategy() = default;
    
    // Core operations any defrag strategy will need
    virtual bool BeginDefragmentation() = 0;
    virtual bool DefragmentStep() = 0;  // For incremental defrag
    virtual void EndDefragmentation() = 0;
    
    // Analysis
    virtual DefragmentationStats GetStats() const = 0;
    virtual bool NeedsDefragmentation() const = 0;
    
protected:
    // Common utilities for derived strategies
    bool IsBlockMovable(const MemoryBlock& block) const;
    void NotifyBlockMove(void* oldPtr, void* newPtr, size_t size);
};

}} // namespace Hydragon::Memory 