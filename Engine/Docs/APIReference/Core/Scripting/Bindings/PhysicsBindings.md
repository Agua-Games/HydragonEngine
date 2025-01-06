
## Classes

### PhysicsBindings




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const PhysicsBindingsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateRigidBody



**Parameters:** const RigidBodyDesc& desc

---

### DestroyRigidBody



**Parameters:** RigidBodyHandle handle

---

### SetRigidBodyProperties



**Parameters:** RigidBodyHandle handle, const RigidBodyProperties& props

---

### CreateCollider



**Parameters:** const ColliderDesc& desc

---

### DestroyCollider



**Parameters:** ColliderHandle handle

---

### SetColliderProperties



**Parameters:** ColliderHandle handle, const ColliderProperties& props

---

### CreateConstraint



**Parameters:** const ConstraintDesc& desc

---

### DestroyConstraint



**Parameters:** ConstraintHandle handle

---

### SetConstraintProperties



**Parameters:** ConstraintHandle handle, const ConstraintProperties& props

---

### RayCast



**Parameters:** const Vector3& start, const Vector3& end, const RaycastOptions& options

---

### ShapeCast



**Parameters:** const ColliderDesc& shape, const Transform& start, const Transform& end

---

### OverlapTest



**Parameters:** const ColliderDesc& shape, const Transform& pose

---

### RegisterCollisionCallback



**Parameters:** const std::string& name, const CollisionCallback& callback

---

### UnregisterCollisionCallback



**Parameters:** const std::string& name

---

### RegisterTriggerCallback



**Parameters:** const std::string& name, const TriggerCallback& callback

---

### UnregisterTriggerCallback



**Parameters:** const std::string& name

---

### EnableDebugDrawing



**Parameters:** bool enable

---

### SetDebugDrawMode



**Parameters:** DebugDrawMode mode

---
