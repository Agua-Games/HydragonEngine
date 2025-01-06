
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

### Submit



**Parameters:** const RenderCommand& command

### SetViewport



**Parameters:** const Viewport& viewport

### SetRenderTarget



**Parameters:** RenderTargetHandle target

### CreateMaterial



**Parameters:** const MaterialDesc& desc

### DestroyMaterial



**Parameters:** MaterialHandle handle

### CreateTexture



**Parameters:** const TextureDesc& desc

### DestroyTexture



**Parameters:** TextureHandle handle

### UpdateBuffer



**Parameters:** BufferHandle handle, const void* data, size_t size

### SetShaderParameter



**Parameters:** MaterialHandle material, const char* name, const void* data
