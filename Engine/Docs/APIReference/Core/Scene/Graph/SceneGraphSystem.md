# Classes
---

## SceneGraphSystem
---




# Variables
---

### `enableDynamicUpdates`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSpatialHashing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableFrustumCulling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableOcclusionCulling`

- **Type:** `bool`

- **Default Value:** `true`



### `maxNodes`

- **Type:** `uint32_t`

- **Default Value:** `100000`



### `maxLayers`

- **Type:** `uint32_t`

- **Default Value:** `32`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `256 * 1024 * 1024`



### `config`

- **Type:** `const GraphConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `GraphConfig`



### `m_Stats`

- **Type:** `GraphStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `GraphConfig`

- **Description:** 

- **Members:**

  - `1024`: `256 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static SceneGraphSystem&`

---

## Initialize



- **Parameters:** const GraphConfig& config = {}

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

## CreateNode



- **Parameters:** const NodeDesc& desc

- **Return:** `NodeHandle`

---

## DestroyNode



- **Parameters:** NodeHandle handle

- **Return:** `void`

---

## SetParent



- **Parameters:** NodeHandle node, NodeHandle parent

- **Return:** `void`

---

## RemoveParent



- **Parameters:** NodeHandle node

- **Return:** `void`

---

## SetTransform



- **Parameters:** NodeHandle node, const Transform& transform

- **Return:** `void`

---

## SetBounds



- **Parameters:** NodeHandle node, const BoundingVolume& bounds

- **Return:** `void`

---

## SetLayer



- **Parameters:** NodeHandle node, uint32_t layer

- **Return:** `void`

---

## SetVisibility



- **Parameters:** NodeHandle node, bool visible

- **Return:** `void`

---
