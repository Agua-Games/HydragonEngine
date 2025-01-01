/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory block mapper for Hydragon
 */

#pragma once
#include <QObject>
#include "Core/Memory/MemoryBlock.h"

namespace Hydragon {
namespace Tools {

class MemoryBlockMapper : public QObject {
    Q_OBJECT
public:
    struct BlockPosition {
        double x, y;    // Normalized coordinates (0-100)
        double size;    // Visual size
        QString label;
    };
    
    BlockPosition mapToPosition(const Memory::MemoryBlock& block);
    BlockPosition mapGroupToPosition(const BlockGroup& group);
    
    void setMinBlockSize(size_t size) { m_MinBlockSize = size; }
    void setMaxBlockSize(size_t size) { m_MaxBlockSize = size; }

private:
    size_t m_MinBlockSize = 1024;      // 1KB
    size_t m_MaxBlockSize = 1024*1024;  // 1MB
    
    double calculateVisualSize(size_t memorySize);
    QPointF findFreePosition(double size);
};

}} // namespace Hydragon::Tools 