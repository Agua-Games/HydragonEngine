
## Classes

### GraphSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const GraphConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateGraph



**Parameters:** const GraphDesc& desc

### DestroyGraph



**Parameters:** GraphHandle handle

### AddNode



**Parameters:** GraphHandle graph, const NodeDesc& desc

### RemoveNode



**Parameters:** GraphHandle graph, NodeHandle node

### AddEdge



**Parameters:** GraphHandle graph, NodeHandle from, NodeHandle to, const EdgeDesc& desc

### RemoveEdge



**Parameters:** GraphHandle graph, EdgeHandle edge

### TraverseGraph



**Parameters:** GraphHandle graph, const TraversalParams& params

### OptimizeGraph



**Parameters:** GraphHandle graph, const OptimizationParams& params
