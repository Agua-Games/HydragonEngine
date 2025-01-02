/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Block quad tree for Hydragon
 */

#include "BlockQuadTree.h"

namespace Hydragon {
namespace Tools {

Node* BlockQuadTree::findSpace(double size) {
    return findSpaceInNode(m_Root.get(), size, 0);
}

Node* BlockQuadTree::findSpaceInNode(Node* node, double size, int depth) {
    if (!node || node->occupied || depth >= m_MaxDepth) {
        return nullptr;
    }
    
    // If node is too small, reject
    if (node->bounds.width() < size || node->bounds.height() < size) {
        return nullptr;
    }
    
    // If node is empty and just right, use it
    if (!node->children[0] && node->bounds.width() <= size * 1.5) {
        return node;
    }
    
    // Subdivide if needed
    if (!node->children[0]) {
        subdivide(node);
    }
    
    // Try to find space in children
    for (auto& child : node->children) {
        if (Node* found = findSpaceInNode(child.get(), size, depth + 1)) {
            return found;
        }
    }
    
    return nullptr;
} 