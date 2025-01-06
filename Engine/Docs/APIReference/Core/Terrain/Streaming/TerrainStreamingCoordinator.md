# Classes
---

## TerrainStreamingCoordinator
---




# Variables
---

### `streamingConfig`

- **Type:** `StreamingConfig`



### `meshletConfig`

- **Type:** `MeshletConfig`



### `textureConfig`

- **Type:** `VirtualTextureConfig`



### `meshletStreamingDistance`

- **Type:** `float`

- **Default Value:** `1000.0f`



### `textureStreamingDistance`

- **Type:** `float`

- **Default Value:** `2000.0f`



### `maxConcurrentOperations`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `averageLoadTime`

- **Type:** `float`



### `peakMemoryUsage`

- **Type:** `size_t`



### `activeStreamingTasks`

- **Type:** `uint32_t`



### `pendingRequests`

- **Type:** `uint32_t`



### `bandwidthUtilization`

- **Type:** `float`



### `m_Stats`

- **Type:** `return`



### `regionId`

- **Type:** `uint32_t`



### `needsMeshlets`

- **Type:** `bool`



### `needsTextures`

- **Type:** `bool`



### `priority`

- **Type:** `float`



### `m_StreamingSystem`

- **Type:** `TerrainStreamingSystem`



### `m_MeshletSystem`

- **Type:** `TerrainMeshletSystem`



### `m_TextureSystem`

- **Type:** `TerrainVirtualTexturing`



### `m_IsPaused`

- **Type:** `bool`

- **Default Value:** `false`



### `m_AssetSystem`

- **Type:** `AssetSystem*`

- **Default Value:** `nullptr`



### `m_CollabManager`

- **Type:** `CollaborationManager*`

- **Default Value:** `nullptr`



### `m_AttributeSystem`

- **Type:** `AttributeSystem*`

- **Default Value:** `nullptr`



### `m_Stats`

- **Type:** `StreamingStats`



### `m_QualityLevel`

- **Type:** `StreamingQuality`

- **Default Value:** `StreamingQuality::High`



### `m_PrioritizedRegion`

- **Type:** `BoundingBox`



### `m_RegionPriority`

- **Type:** `float`

- **Default Value:** `1.0f`




# Structs
---

### `CoordinatorConfig`

- **Description:** 

- **Members:**

  - `streamingConfig`: `StreamingConfig` - 

  - `meshletConfig`: `MeshletConfig` - 

  - `textureConfig`: `VirtualTextureConfig` - 



### `StreamingStats`

- **Description:** 

- **Members:**

  - `averageLoadTime`: `float` - 

  - `peakMemoryUsage`: `size_t` - 

  - `activeStreamingTasks`: `uint32_t` - 

  - `pendingRequests`: `uint32_t` - 

  - `bandwidthUtilization`: `float` - 



### `StreamingTask`

- **Description:** 

- **Members:**

  - `regionId`: `uint32_t` - 

  - `needsMeshlets`: `bool` - 

  - `needsTextures`: `bool` - 

  - `priority`: `float` - 




# Functions
---

## update



- **Parameters:** `const Camera& camera`

- **Return:** `void`

---

## setStreamingPriority



- **Parameters:** `const Vector3& position`, `float priority`

- **Return:** `void`

---

## pauseStreaming



- **Parameters:** ``

- **Return:** `void`

---

## resumeStreaming



- **Parameters:** ``

- **Return:** `void`

---

## drawDebugInfo



- **Parameters:** `CommandBuffer* cmd`

- **Return:** `void`

---

## handleAssetReload



- **Parameters:** `const AssetReloadEvent& event`

- **Return:** `void`

---

## registerStreamingAssets



- **Parameters:** `AssetSystem& assetSystem`

- **Return:** `void`

---

## handleCollaboratorUpdate



- **Parameters:** `const CollaboratorUpdate& update`

- **Return:** `void`

---

## broadcastStreamingState



- **Parameters:** ``

- **Return:** `void`

---

## registerAttributes



- **Parameters:** `AttributeSystem& attributes`

- **Return:** `void`

---

## setStreamingBudget



- **Parameters:** `size_t budgetMB`

- **Return:** `void`

---

## setPrioritizedRegion



- **Parameters:** `const BoundingBox& region`, `float priority`

- **Return:** `void`

---

## setQualityLevel



- **Parameters:** `StreamingQuality quality`

- **Return:** `void`

---

## updateStreamingTasks



- **Parameters:** `const Camera& camera`

- **Return:** `void`

---

## processStreamingTasks



- **Parameters:** ``

- **Return:** `void`

---

## synchronizeSystems



- **Parameters:** ``

- **Return:** `void`

---

## handleStreamingFailure



- **Parameters:** `const StreamingError& error`

- **Return:** `void`

---

## updateStreamingStats



- **Parameters:** ``

- **Return:** `void`

---

## notifyCollaborators



- **Parameters:** `const StreamingEvent& event`

- **Return:** `void`

---

## handleAssetDependencies



- **Parameters:** `uint32_t regionId`

- **Return:** `void`

---

## calculateRegionPriority



- **Parameters:** `const BoundingBox& bounds`, `float distance`

- **Return:** `float`

---
