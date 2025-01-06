
## Classes

### StreamingMemoryManager




## Functions

### registerStreamingSystem



**Parameters:** IStreamingSystem* system

### setMemoryStrategy



**Parameters:** IMemoryStrategy* strategy

### registerModule



**Parameters:** const std::string& moduleName, size_t reservedMemory

### setModulePriority



**Parameters:** const std::string& moduleName, StreamingPriority priority

### allocateStreamingMemory



**Parameters:** size_t size, const StreamingAllocationInfo& info

### deallocateStreamingMemory



**Parameters:** void* ptr

### updateStreamingState



**Parameters:** 
