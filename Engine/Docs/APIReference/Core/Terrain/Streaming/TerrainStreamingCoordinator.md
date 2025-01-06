
## Classes

### TerrainStreamingCoordinator




## Functions

### update



**Parameters:** const Camera& camera

---

### setStreamingPriority



**Parameters:** const Vector3& position, float priority

---

### pauseStreaming



**Parameters:** 

---

### resumeStreaming



**Parameters:** 

---

### drawDebugInfo



**Parameters:** CommandBuffer* cmd

---

### handleAssetReload



**Parameters:** const AssetReloadEvent& event

---

### registerStreamingAssets



**Parameters:** AssetSystem& assetSystem

---

### handleCollaboratorUpdate



**Parameters:** const CollaboratorUpdate& update

---

### broadcastStreamingState



**Parameters:** 

---

### registerAttributes



**Parameters:** AttributeSystem& attributes

---

### setStreamingBudget



**Parameters:** size_t budgetMB

---

### setPrioritizedRegion



**Parameters:** const BoundingBox& region, float priority

---

### setQualityLevel



**Parameters:** StreamingQuality quality

---

### updateStreamingTasks



**Parameters:** const Camera& camera

---

### processStreamingTasks



**Parameters:** 

---

### synchronizeSystems



**Parameters:** 

---

### handleStreamingFailure



**Parameters:** const StreamingError& error

---

### updateStreamingStats



**Parameters:** 

---

### notifyCollaborators



**Parameters:** const StreamingEvent& event

---

### handleAssetDependencies



**Parameters:** uint32_t regionId

---

### calculateRegionPriority



**Parameters:** const BoundingBox& bounds, float distance

---
