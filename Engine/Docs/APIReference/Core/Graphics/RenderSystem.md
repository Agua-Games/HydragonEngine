# Classes
---

## RenderSystem
---




# Variables
---

### `maxDrawCalls`

- **Type:** `uint32_t`

- **Default Value:** `100000`



### `maxMaterials`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `maxTextures`

- **Type:** `uint32_t`

- **Default Value:** `5000`



### `enableInstancing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMultithreading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableGPUCulling`

- **Type:** `bool`

- **Default Value:** `true`



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

## Submit



- **Parameters:** const RenderCommand& command

- **Return:** `void`

---

## SetViewport



- **Parameters:** const Viewport& viewport

- **Return:** `void`

---

## SetRenderTarget



- **Parameters:** RenderTargetHandle target

- **Return:** `void`

---

## CreateMaterial



- **Parameters:** const MaterialDesc& desc

- **Return:** `MaterialHandle`

---

## DestroyMaterial



- **Parameters:** MaterialHandle handle

- **Return:** `void`

---

## CreateTexture



- **Parameters:** const TextureDesc& desc

- **Return:** `TextureHandle`

---

## DestroyTexture



- **Parameters:** TextureHandle handle

- **Return:** `void`

---

## UpdateBuffer



- **Parameters:** BufferHandle handle, const void* data, size_t size

- **Return:** `void`

---

## SetShaderParameter



- **Parameters:** MaterialHandle material, const char* name, const void* data

- **Return:** `void`

---
