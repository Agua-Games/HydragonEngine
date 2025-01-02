/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Scene graph traversal system
 */

#pragma once
#include "../SceneGraphTypes.h"
#include <functional>
#include <vector>

namespace Hydragon::Scene {

enum class TraversalOrder {
    PreOrder,
    PostOrder,
    BreadthFirst
};

struct TraversalOptions {
    TraversalOrder order = TraversalOrder::PreOrder;
    bool skipInvisible = true;
    bool skipCulled = true;
    uint32_t maxDepth = UINT32_MAX;
    std::function<bool(NodeHandle)> filter;
};

class GraphTraversal {
public:
    using VisitorFunc = std::function<void(NodeHandle)>;
    
    static void TraverseSubtree(NodeHandle root, const VisitorFunc& visitor, 
                               const TraversalOptions& options = {});
                               
    static void TraverseAncestors(NodeHandle node, const VisitorFunc& visitor);
    static void TraverseSiblings(NodeHandle node, const VisitorFunc& visitor);
    
    static std::vector<NodeHandle> CollectSubtree(NodeHandle root, 
                                                 const TraversalOptions& options = {});
    
    static NodeHandle FindCommonAncestor(NodeHandle a, NodeHandle b);
    static uint32_t GetDepth(NodeHandle node);

private:
    static void TraversePreOrder(NodeHandle node, const VisitorFunc& visitor, 
                                const TraversalOptions& options, uint32_t depth);
    static void TraversePostOrder(NodeHandle node, const VisitorFunc& visitor,
                                 const TraversalOptions& options, uint32_t depth);
    static void TraverseBreadthFirst(NodeHandle root, const VisitorFunc& visitor,
                                    const TraversalOptions& options);
};

} // namespace Hydragon::Scene 