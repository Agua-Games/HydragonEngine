
## Classes

### RenderBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const RenderBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreatePipeline



**Parameters:** const PipelineDesc& desc

### DestroyPipeline



**Parameters:** PipelineHandle handle

### SetPipelineState



**Parameters:** PipelineHandle handle, const PipelineState& state

### CreateRenderTarget



**Parameters:** const RenderTargetDesc& desc

### DestroyRenderTarget



**Parameters:** RenderTargetHandle handle

### ResizeRenderTarget



**Parameters:** RenderTargetHandle handle, uint32_t width, uint32_t height

### AddPostEffect



**Parameters:** const std::string& name, const PostEffectDesc& desc

### RemovePostEffect



**Parameters:** const std::string& name

### SetPostEffectEnabled



**Parameters:** const std::string& name, bool enabled

### SetViewport



**Parameters:** const Viewport& viewport

### SetScissor



**Parameters:** const Scissor& scissor

### BindPipeline



**Parameters:** PipelineHandle handle

### BindRenderTarget



**Parameters:** RenderTargetHandle handle

### BindVertexBuffers



**Parameters:** const std::vector<BufferHandle>& buffers

### BindIndexBuffer



**Parameters:** BufferHandle buffer

### BindDescriptorSets



**Parameters:** const std::vector<DescriptorSetHandle>& sets

### Draw



**Parameters:** uint32_t vertexCount, uint32_t instanceCount = 1

### DrawIndexed



**Parameters:** uint32_t indexCount, uint32_t instanceCount = 1

### DrawIndirect



**Parameters:** BufferHandle buffer, uint32_t offset

### BeginProfileScope



**Parameters:** const std::string& name

### EndProfileScope



**Parameters:** 

### SetDebugMarker



**Parameters:** const std::string& name
