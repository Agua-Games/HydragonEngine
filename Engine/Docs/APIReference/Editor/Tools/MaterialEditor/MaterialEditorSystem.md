# Classes
---

## MaterialEditorSystem
---




# Variables
---

### `enableLivePreview`

- **Type:** `bool`

- **Default Value:** `true`



### `enableNodePreview`

- **Type:** `bool`

- **Default Value:** `true`



### `enableShaderDebug`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMaterialInstancing`

- **Type:** `bool`

- **Default Value:** `true`



### `maxPreviewResolution`

- **Type:** `uint32_t`

- **Default Value:** `512`



### `maxNodeCount`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `previewMemoryBudget`

- **Type:** `size_t`

- **Default Value:** `128 * 1024 * 1024`



### `config`

- **Type:** `const MaterialEditorConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `MaterialEditorConfig`



### `m_Stats`

- **Type:** `MaterialEditorStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `MaterialEditorConfig`

- **Description:** 

- **Members:**

  - `1024`: `128 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static MaterialEditorSystem&`

---

## Initialize



- **Parameters:** `const MaterialEditorConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## CreateMaterial



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## LoadMaterial



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## SaveMaterial



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## AddNode



- **Parameters:** `const NodeDesc& desc`

- **Return:** `void`

---

## RemoveNode



- **Parameters:** `NodeHandle handle`

- **Return:** `void`

---

## ConnectNodes



- **Parameters:** `NodeHandle output`, `NodeHandle input`, `uint32_t slot`

- **Return:** `void`

---

## SetParameter



- **Parameters:** `const std::string& name`, `const MaterialParameter& value`

- **Return:** `void`

---

## UpdatePreview



- **Parameters:** ``

- **Return:** `void`

---

## CompileShader



- **Parameters:** ``

- **Return:** `void`

---

## ValidateGraph



- **Parameters:** ``

- **Return:** `void`

---
