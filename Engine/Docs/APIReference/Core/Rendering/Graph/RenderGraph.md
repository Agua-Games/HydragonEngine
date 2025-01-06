
## Classes

### RenderGraph




## Functions

### CreateResource



**Parameters:** const ResourceDesc& desc

### AddPass



**Parameters:** const PassDesc& desc

### SetInput



**Parameters:** PassHandle pass, const std::string& name, ResourceHandle resource

### SetOutput



**Parameters:** PassHandle pass, const std::string& name, ResourceHandle resource

### AddDependency



**Parameters:** PassHandle dependent, PassHandle dependency

### RemoveDependency



**Parameters:** PassHandle dependent, PassHandle dependency

### Compile



**Parameters:** 

### Execute



**Parameters:** CommandBuffer& cmd

### Clear



**Parameters:** 
