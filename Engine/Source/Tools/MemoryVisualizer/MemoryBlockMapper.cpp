#include "MemoryBlockMapper.h"
#include <cmath>
#include <algorithm>

namespace Hydragon {
namespace Tools {

BlockPosition MemoryBlockMapper::mapToPosition(const Memory::MemoryBlock& block) {
    BlockPosition pos;
    
    // Calculate visual size based on memory size
    pos.size = calculateVisualSize(block.size);
    
    // Find optimal position in grid
    auto point = findFreePosition(pos.size);
    pos.x = point.x();
    pos.y = point.y();
    
    // Enhance with block metadata
    pos.label = formatBlockLabel(block);
    pos.type = determineBlockType(block);  // For future streaming/resource types
    pos.flags = calculateBlockFlags(block); // For future memory attributes
    
    return pos;
}

BlockPosition MemoryBlockMapper::mapGroupToPosition(const BlockGroup& group) {
    BlockPosition pos;
    pos.size = calculateGroupSize(group);
    
    // Use different positioning strategy for groups
    auto point = findGroupPosition(group);
    pos.x = point.x();
    pos.y = point.y();
    
    // Add group-specific metadata
    pos.groupInfo = createGroupInfo(group);
    return pos;
}

QPointF MemoryBlockMapper::findFreePosition(double size) {
    // Quadtree-based position finding for efficient space usage
    if (!m_PositionTree) {
        m_PositionTree = std::make_unique<BlockQuadTree>(QRectF(0, 0, 100, 100));
    }
    
    return m_PositionTree->findSpace(size);
}

BlockQuadTree::Node* MemoryBlockMapper::reserveSpace(const QRectF& rect) {
    // Reserve space in quadtree and handle overlaps
    auto node = m_PositionTree->insert(rect);
    if (node) {
        m_OccupiedSpaces.push_back(node);
        emit spaceReserved(rect);
    }
    return node;
}

// Future-proofing: Support for different block types
BlockType MemoryBlockMapper::determineBlockType(const Memory::MemoryBlock& block) {
    if (block.flags & Memory::BlockFlags::Streaming) {
        return BlockType::Streaming;
    } else if (block.flags & Memory::BlockFlags::Resource) {
        return BlockType::Resource;
    } else if (block.flags & Memory::BlockFlags::System) {
        return BlockType::System;
    }
    return BlockType::Default;
}

} // namespace Tools
} // namespace Hydragon 