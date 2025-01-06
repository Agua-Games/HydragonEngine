# Classes
---

## PipelineSystem
---




# Variables
---

### `maxPipelines`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxShaders`

- **Type:** `uint32_t`

- **Default Value:** `5000`



### `enableShaderHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePipelineCache`

- **Type:** `bool`

- **Default Value:** `true`



### `enableValidation`

- **Type:** `bool`

- **Default Value:** `true`



### `pipelineCacheSize`

- **Type:** `size_t`

- **Default Value:** `256 * 1024 * 1024`



### `config`

- **Type:** `const PipelineConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `PipelineConfig`



### `m_Stats`

- **Type:** `PipelineStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `PipelineConfig`

- **Description:** 

- **Members:**

  - `1024`: `256 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static PipelineSystem&`

---

## Initialize



- **Parameters:** `const PipelineConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## CreatePipeline



- **Parameters:** `const PipelineDesc& desc`

- **Return:** `PipelineHandle`

---

## DestroyPipeline



- **Parameters:** `PipelineHandle handle`

- **Return:** `void`

---

## CreateShader



- **Parameters:** `const ShaderDesc& desc`

- **Return:** `ShaderHandle`

---

## DestroyShader



- **Parameters:** `ShaderHandle handle`

- **Return:** `void`

---

## BindPipeline



- **Parameters:** `PipelineHandle handle`

- **Return:** `void`

---

## SetPipelineState



- **Parameters:** `PipelineHandle handle`, `const PipelineState& state`

- **Return:** `void`

---
