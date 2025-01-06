
## Classes

### RuntimePhysicsSystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimePhysicsConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### FixedUpdate



**Parameters:** 

---

### CreateRigidBody



**Parameters:** const RigidBodyDesc& desc

---

### DestroyRigidBody



**Parameters:** RigidBodyHandle handle

---

### CreateCollider



**Parameters:** const ColliderDesc& desc

---

### DestroyCollider



**Parameters:** ColliderHandle handle

---

### CreateJoint



**Parameters:** const JointDesc& desc

---

### DestroyJoint



**Parameters:** JointHandle handle

---

### SetBodyTransform



**Parameters:** RigidBodyHandle handle, const Transform& transform

---

### SetBodyVelocity



**Parameters:** RigidBodyHandle handle, const Vector3& linear, const Vector3& angular = {}

---

### ApplyForce



**Parameters:** RigidBodyHandle handle, const Vector3& force, const Vector3& point = {}

---

### ApplyImpulse



**Parameters:** RigidBodyHandle handle, const Vector3& impulse, const Vector3& point = {}

---

### SetCollisionFilter



**Parameters:** ColliderHandle handle, uint32_t category, uint32_t mask

---

### SetCollisionGroup



**Parameters:** ColliderHandle handle, int32_t group

---

### AddCollisionHandler



**Parameters:** const std::string& name, const CollisionCallback& callback

---

### RemoveCollisionHandler



**Parameters:** const std::string& name

---

### SetGravity



**Parameters:** const Vector3& gravity

---

### SetTimestep



**Parameters:** float timestep

---

### Raycast



**Parameters:** const Vector3& origin, const Vector3& direction, float maxDistance = 1000.0f

---
