# Classes
---

## RenderSystem
---




# Variables
---

### `enableMultithreading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableRayTracing`

- **Type:** `bool`

- **Default Value:** `false`



### `enableVRS`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMeshShaders`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAsyncCompute`

- **Type:** `bool`

- **Default Value:** `true`



### `maxDrawCalls`

- **Type:** `uint32_t`

- **Default Value:** `100000`



### `maxMaterials`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `maxTextures`

- **Type:** `uint32_t`

- **Default Value:** `5000`



### `gpuMemoryBudget`

- **Type:** `size_t`

- **Default Value:** `4 * 1024 * 1024 * 1024ull`



### `config`

- **Type:** `const RenderConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RenderConfig`



### `m_Stats`

- **Type:** `RenderStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RenderConfig`

- **Description:** 

- **Members:**

  - `1024ull`: `4 * 1024 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static RenderSystem&`

---

## Initialize



- **Parameters:** const RenderConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## BeginFrame



- **Parameters:** 

- **Return:** `void`

---

## EndFrame



- **Parameters:** 

- **Return:** `void`

---

## ExecuteRenderGraph



- **Parameters:** const RenderGraph& graph

- **Return:** `void`

---

## SubmitCommandBuffer



- **Parameters:** const CommandBuffer& buffer

- **Return:** `void`

---

## CreateResource



- **Parameters:** const ResourceDesc& desc

- **Return:** `ResourceHandle`

---

## DestroyResource



- **Parameters:** ResourceHandle handle

- **Return:** `void`

---

## CreatePipeline



- **Parameters:** const PipelineDesc& desc

- **Return:** `PipelineHandle`

---

## DestroyPipeline



- **Parameters:** PipelineHandle handle

- **Return:** `void`

---

## UpdateBuffer



- **Parameters:** BufferHandle handle, const void* data, size_t size

- **Return:** `void`

---

## UpdateTexture



- **Parameters:** TextureHandle handle, const void* data, const TextureRegion& region

- **Return:** `void`

---

## SetViewport



- **Parameters:** const Viewport& viewport

- **Return:** `void`

---

## SetScissor



- **Parameters:** const Scissor& scissor

- **Return:** `void`

---

## BeginRenderPass



- **Parameters:** const RenderPassDesc& desc

- **Return:** `void`

---

## EndRenderPass



- **Parameters:** 

- **Return:** `void`

---
