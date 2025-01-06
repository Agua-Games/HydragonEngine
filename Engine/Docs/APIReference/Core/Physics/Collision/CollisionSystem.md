
## Classes

### CollisionSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const CollisionConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### AddCollider



**Parameters:** ColliderHandle handle, const ColliderDesc& desc

---

### RemoveCollider



**Parameters:** ColliderHandle handle

---

### UpdateCollider



**Parameters:** ColliderHandle handle, const Transform& transform

---

### SetCollisionFilter



**Parameters:** ColliderHandle handle, uint32_t group, uint32_t mask

---

### QueryAABB



**Parameters:** const AABB& aabb, QueryCallback callback

---

### QuerySphere



**Parameters:** const Sphere& sphere, QueryCallback callback

---
