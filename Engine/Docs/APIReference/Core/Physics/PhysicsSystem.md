
## Classes

### PhysicsSystem




## Functions

### Get



**Parameters:** 

### Initialize



**Parameters:** const PhysicsConfig& config = {}

### Shutdown



**Parameters:** 

### Update



**Parameters:** float deltaTime

### StepSimulation



**Parameters:** float timeStep

### CreateRigidBody



**Parameters:** const RigidBodyDesc& desc

### DestroyRigidBody



**Parameters:** RigidBodyHandle handle

### CreateCollider



**Parameters:** const ColliderDesc& desc

### DestroyCollider



**Parameters:** ColliderHandle handle

### CreateConstraint



**Parameters:** const ConstraintDesc& desc

### DestroyConstraint



**Parameters:** ConstraintHandle handle

### SetGravity



**Parameters:** const Vector3& gravity

### RayCast



**Parameters:** const Ray& ray, RayCastCallback callback
