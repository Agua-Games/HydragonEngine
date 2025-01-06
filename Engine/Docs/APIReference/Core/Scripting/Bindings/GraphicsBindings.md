
## Classes

### GraphicsBindings




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const GraphicsBindingsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** 

### CreateMaterial



**Parameters:** const MaterialDesc& desc

### DestroyMaterial



**Parameters:** MaterialHandle handle

### UpdateMaterial



**Parameters:** MaterialHandle handle, const MaterialProperties& props

### CreateShader



**Parameters:** const ShaderDesc& desc

### DestroyShader



**Parameters:** ShaderHandle handle

### ReloadShader



**Parameters:** ShaderHandle handle

### RegisterRenderPass



**Parameters:** const std::string& name, const RenderPassDesc& desc

### UnregisterRenderPass



**Parameters:** const std::string& name

### SetRenderPassEnabled



**Parameters:** const std::string& name, bool enabled

### DrawDebugLine



**Parameters:** const Vector3& start, const Vector3& end, const Color& color

### DrawDebugSphere



**Parameters:** const Vector3& center, float radius, const Color& color

### DrawDebugText



**Parameters:** const Vector3& position, const std::string& text, const Color& color

### RegisterRenderCallback



**Parameters:** const std::string& name, const RenderCallback& callback

### UnregisterRenderCallback



**Parameters:** const std::string& name
