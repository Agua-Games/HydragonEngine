
## Classes

### TraversalOrder



### GraphTraversal




## Functions

### TraverseSubtree



**Parameters:** NodeHandle root, const VisitorFunc& visitor, 
                               const TraversalOptions& options = {}

### TraverseAncestors



**Parameters:** NodeHandle node, const VisitorFunc& visitor

### TraverseSiblings



**Parameters:** NodeHandle node, const VisitorFunc& visitor

### FindCommonAncestor



**Parameters:** NodeHandle a, NodeHandle b

### GetDepth



**Parameters:** NodeHandle node

### TraversePreOrder



**Parameters:** NodeHandle node, const VisitorFunc& visitor, 
                                const TraversalOptions& options, uint32_t depth

### TraversePostOrder



**Parameters:** NodeHandle node, const VisitorFunc& visitor,
                                 const TraversalOptions& options, uint32_t depth

### TraverseBreadthFirst



**Parameters:** NodeHandle root, const VisitorFunc& visitor,
                                    const TraversalOptions& options
