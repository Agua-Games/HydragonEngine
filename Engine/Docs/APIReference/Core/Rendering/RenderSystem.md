
## Classes

### RenderSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RenderConfig& config = {}

### Shutdown



**Parameters:** 

### BeginFrame



**Parameters:** 

### EndFrame



**Parameters:** 

### ExecuteRenderGraph



**Parameters:** const RenderGraph& graph

### SubmitCommandBuffer



**Parameters:** const CommandBuffer& buffer

### CreateResource



**Parameters:** const ResourceDesc& desc

### DestroyResource



**Parameters:** ResourceHandle handle

### CreatePipeline



**Parameters:** const PipelineDesc& desc

### DestroyPipeline



**Parameters:** PipelineHandle handle

### UpdateBuffer



**Parameters:** BufferHandle handle, const void* data, size_t size

### UpdateTexture



**Parameters:** TextureHandle handle, const void* data, const TextureRegion& region

### SetViewport



**Parameters:** const Viewport& viewport

### SetScissor



**Parameters:** const Scissor& scissor

### BeginRenderPass



**Parameters:** const RenderPassDesc& desc

### EndRenderPass



**Parameters:** 
