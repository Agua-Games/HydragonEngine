
## Classes

### RuntimeNavigationSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeNavigationConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### LoadNavMesh



**Parameters:** const std::string& path

---

### UnloadNavMesh



**Parameters:** NavMeshHandle handle

---

### BuildNavMesh



**Parameters:** const NavMeshBuildSettings& settings

---

### RequestPath



**Parameters:** const Vector3& start, const Vector3& end, const PathfindingParams& params = {}

---

### CancelPathRequest



**Parameters:** PathRequestHandle handle

---

### CreateCrowdAgent



**Parameters:** const CrowdAgentDesc& desc

---

### DestroyCrowdAgent



**Parameters:** CrowdAgentHandle handle

---

### SetAgentDestination



**Parameters:** CrowdAgentHandle handle, const Vector3& destination

---

### AddDynamicObstacle



**Parameters:** const Vector3& position, float radius

---

### RemoveDynamicObstacle



**Parameters:** ObstacleHandle handle

---

### AddAreaModifier



**Parameters:** const AreaModifierDesc& desc

---

### RemoveAreaModifier



**Parameters:** AreaModifierHandle handle

---

### UpdateAreaModifier



**Parameters:** AreaModifierHandle handle, float cost

---

### RegisterNavigationFilter



**Parameters:** const std::string& name, std::unique_ptr<INavigationFilter> filter

---

### UnregisterNavigationFilter



**Parameters:** const std::string& name

---
