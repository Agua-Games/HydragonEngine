# Classes
---

## GraphicsBindings
---




# Variables
---

### `enableMaterialScripting`

- **Type:** `bool`

- **Default Value:** `true`



### `enableShaderHotReload`

- **Type:** `bool`

- **Default Value:** `true`



### `enableRenderPassScripting`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebugVisualization`

- **Type:** `bool`

- **Default Value:** `true`



### `maxCustomMaterials`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxCustomShaders`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `maxRenderCallbacks`

- **Type:** `uint32_t`

- **Default Value:** `20`



### `shaderCachePath`

- **Type:** `string`

- **Default Value:** `"Cache/Shaders"`



### `config`

- **Type:** `const GraphicsBindingsConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `GraphicsBindingsConfig`



### `m_Stats`

- **Type:** `GraphicsBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `GraphicsBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static GraphicsBindings&`

---

## Initialize



- **Parameters:** const GraphicsBindingsConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

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

## UpdateMaterial



- **Parameters:** MaterialHandle handle, const MaterialProperties& props

- **Return:** `void`

---

## CreateShader



- **Parameters:** const ShaderDesc& desc

- **Return:** `ShaderHandle`

---

## DestroyShader



- **Parameters:** ShaderHandle handle

- **Return:** `void`

---

## ReloadShader



- **Parameters:** ShaderHandle handle

- **Return:** `void`

---

## RegisterRenderPass



- **Parameters:** const std::string& name, const RenderPassDesc& desc

- **Return:** `void`

---

## UnregisterRenderPass



- **Parameters:** const std::string& name

- **Return:** `void`

---

## SetRenderPassEnabled



- **Parameters:** const std::string& name, bool enabled

- **Return:** `void`

---

## DrawDebugLine



- **Parameters:** const Vector3& start, const Vector3& end, const Color& color

- **Return:** `void`

---

## DrawDebugSphere



- **Parameters:** const Vector3& center, float radius, const Color& color

- **Return:** `void`

---

## DrawDebugText



- **Parameters:** const Vector3& position, const std::string& text, const Color& color

- **Return:** `void`

---

## RegisterRenderCallback



- **Parameters:** const std::string& name, const RenderCallback& callback

- **Return:** `void`

---

## UnregisterRenderCallback



- **Parameters:** const std::string& name

- **Return:** `void`

---
