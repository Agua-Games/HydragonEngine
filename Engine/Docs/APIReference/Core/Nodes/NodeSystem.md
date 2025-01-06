
## Classes

### NodeSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const NodeConfig& config = {}

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

### Connect



**Parameters:** GraphHandle graph, NodeHandle output, NodeHandle input, 
                const std::string& pinName

### Disconnect



**Parameters:** GraphHandle graph, NodeHandle output, NodeHandle input

### Execute



**Parameters:** GraphHandle graph
