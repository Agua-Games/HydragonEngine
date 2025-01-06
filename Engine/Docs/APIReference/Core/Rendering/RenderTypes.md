# Classes
---

## ResourceType
---



## ShaderStage
---




# Variables
---

### `ResourceHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `PipelineHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `BufferHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `TextureHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `type`

- **Type:** `ResourceType`



### `width`

- **Type:** `uint32_t`

- **Default Value:** `1`



### `height`

- **Type:** `uint32_t`

- **Default Value:** `1`



### `depth`

- **Type:** `uint32_t`

- **Default Value:** `1`



### `mipLevels`

- **Type:** `uint32_t`

- **Default Value:** `1`



### `arraySize`

- **Type:** `uint32_t`

- **Default Value:** `1`



### `format`

- **Type:** `Format`

- **Default Value:** `Format::Unknown`



### `flags`

- **Type:** `ResourceFlags`

- **Default Value:** `ResourceFlags::None`



### `vertexLayout`

- **Type:** `VertexLayoutDesc`



### `rasterState`

- **Type:** `RasterStateDesc`



### `depthStencil`

- **Type:** `DepthStencilDesc`



### `blendState`

- **Type:** `BlendStateDesc`



### `topology`

- **Type:** `PrimitiveTopology`



### `drawCalls`

- **Type:** `uint32_t`



### `triangles`

- **Type:** `uint32_t`



### `vertices`

- **Type:** `uint32_t`



### `shaderBindings`

- **Type:** `uint32_t`



### `resourceUpdates`

- **Type:** `uint32_t`



### `gpuMemoryUsed`

- **Type:** `size_t`



### `frameTimeMs`

- **Type:** `float`




# Structs
---

### `ResourceDesc`

- **Description:** 

- **Members:**

  - `type`: `ResourceType` - 



### `PipelineDesc`

- **Description:** 

- **Members:**

  - `vertexLayout`: `VertexLayoutDesc` - 

  - `rasterState`: `RasterStateDesc` - 

  - `depthStencil`: `DepthStencilDesc` - 

  - `blendState`: `BlendStateDesc` - 

  - `topology`: `PrimitiveTopology` - 



### `RenderStats`

- **Description:** 

- **Members:**

  - `drawCalls`: `uint32_t` - 

  - `triangles`: `uint32_t` - 

  - `vertices`: `uint32_t` - 

  - `shaderBindings`: `uint32_t` - 

  - `resourceUpdates`: `uint32_t` - 

  - `gpuMemoryUsed`: `size_t` - 

  - `frameTimeMs`: `float` - 


