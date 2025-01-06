# Classes
---

## TerrainMeshletSystem
---




# Variables
---

### `INVALID_MESHLET`

- **Type:** `static constexpr uint32_t`

- **Default Value:** `~0u`



### `maxVertices`

- **Type:** `uint32_t`

- **Default Value:** `64`



### `maxPrimitives`

- **Type:** `uint32_t`

- **Default Value:** `126`



### `maxMeshlets`

- **Type:** `uint32_t`

- **Default Value:** `32768`



### `enableCulling`

- **Type:** `bool`

- **Default Value:** `true`



### `lodThreshold`

- **Type:** `float`

- **Default Value:** `0.75f`



### `operator`

- **Type:** `TerrainMeshletSystem&`

- **Default Value:** `(const TerrainMeshletSystem&) = delete`



### `vertexOffset`

- **Type:** `uint32_t`



### `primitiveOffset`

- **Type:** `uint32_t`



### `vertexCount`

- **Type:** `uint32_t`



### `primitiveCount`

- **Type:** `uint32_t`



### `lodDistance`

- **Type:** `float`



### `bounds`

- **Type:** `BoundingBox`



### `isVisible`

- **Type:** `bool`

- **Default Value:** `true`



### `m_Device`

- **Type:** `IRenderDevice*`



### `m_Config`

- **Type:** `MeshletConfig`



### `m_MeshletBuffer`

- **Type:** `BufferHandle`

- **Default Value:** `nullptr`



### `m_VertexBuffer`

- **Type:** `BufferHandle`

- **Default Value:** `nullptr`



### `m_PrimitiveBuffer`

- **Type:** `BufferHandle`

- **Default Value:** `nullptr`



### `m_CurrentVertexOffset`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `m_CurrentPrimitiveOffset`

- **Type:** `uint32_t`

- **Default Value:** `0`




# Structs
---

### `MeshletConfig`

- **Description:** 

- **Members:**



### `TerrainMeshlet`

- **Description:** 

- **Members:**

  - `vertexOffset`: `uint32_t` - 

  - `primitiveOffset`: `uint32_t` - 

  - `vertexCount`: `uint32_t` - 

  - `primitiveCount`: `uint32_t` - 

  - `lodDistance`: `float` - 

  - `bounds`: `BoundingBox` - 




# Functions
---

## updateLODs



- **Parameters:** `const Camera& camera`

- **Return:** `void`

---

## drawMeshlets



- **Parameters:** `CommandBuffer* cmd`

- **Return:** `void`

---

## streamMeshlets



- **Parameters:** `const StreamingRegion& region`

- **Return:** `void`

---

## unloadMeshlets



- **Parameters:** `const StreamingRegion& region`

- **Return:** `void`

---

## drawMeshletBounds



- **Parameters:** `CommandBuffer* cmd`

- **Return:** `void`

---

## drawLODTransitions



- **Parameters:** `CommandBuffer* cmd`

- **Return:** `void`

---

## updateMeshletVisibility



- **Parameters:** `const Camera& camera`

- **Return:** `void`

---

## calculateBounds



- **Parameters:** `const std::vector<TerrainVertex>& vertices`

- **Return:** `BoundingBox`

---

## cleanupResources



- **Parameters:** ``

- **Return:** `void`

---
