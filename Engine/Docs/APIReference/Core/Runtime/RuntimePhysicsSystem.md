# Classes
---

## RuntimePhysicsSystem
---




# Variables
---

### `enableContinuousDetection`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMultithreading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSleeping`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebugDraw`

- **Type:** `bool`

- **Default Value:** `true`



### `solverIterations`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `maxSubsteps`

- **Type:** `uint32_t`

- **Default Value:** `3`



### `fixedTimestep`

- **Type:** `float`

- **Default Value:** `1.0f/60.0f`



### `defaultFriction`

- **Type:** `float`

- **Default Value:** `0.5f`



### `defaultRestitution`

- **Type:** `float`

- **Default Value:** `0.0f`



### `gravity`

- **Type:** `Vector3`

- **Default Value:** `{0.0f, -9.81f, 0.0f}`



### `config`

- **Type:** `const RuntimePhysicsConfig&`

- **Default Value:** `{})`



### `angular`

- **Type:** `const Vector3&`

- **Default Value:** `{})`



### `point`

- **Type:** `const Vector3&`

- **Default Value:** `{})`



### `point`

- **Type:** `const Vector3&`

- **Default Value:** `{})`



### `maxDistance`

- **Type:** `float`

- **Default Value:** `1000.0f)`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimePhysicsConfig`



### `m_Stats`

- **Type:** `RuntimePhysicsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimePhysicsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static RuntimePhysicsSystem&`

---

## Initialize



- **Parameters:** `const RuntimePhysicsConfig& config = {}`

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

## FixedUpdate



- **Parameters:** ``

- **Return:** `void`

---

## CreateRigidBody



- **Parameters:** `const RigidBodyDesc& desc`

- **Return:** `RigidBodyHandle`

---

## DestroyRigidBody



- **Parameters:** `RigidBodyHandle handle`

- **Return:** `void`

---

## CreateCollider



- **Parameters:** `const ColliderDesc& desc`

- **Return:** `ColliderHandle`

---

## DestroyCollider



- **Parameters:** `ColliderHandle handle`

- **Return:** `void`

---

## CreateJoint



- **Parameters:** `const JointDesc& desc`

- **Return:** `JointHandle`

---

## DestroyJoint



- **Parameters:** `JointHandle handle`

- **Return:** `void`

---

## SetBodyTransform



- **Parameters:** `RigidBodyHandle handle`, `const Transform& transform`

- **Return:** `void`

---

## SetBodyVelocity



- **Parameters:** `RigidBodyHandle handle`, `const Vector3& linear`, `const Vector3& angular = {}`

- **Return:** `void`

---

## ApplyForce



- **Parameters:** `RigidBodyHandle handle`, `const Vector3& force`, `const Vector3& point = {}`

- **Return:** `void`

---

## ApplyImpulse



- **Parameters:** `RigidBodyHandle handle`, `const Vector3& impulse`, `const Vector3& point = {}`

- **Return:** `void`

---

## SetCollisionFilter



- **Parameters:** `ColliderHandle handle`, `uint32_t category`, `uint32_t mask`

- **Return:** `void`

---

## SetCollisionGroup



- **Parameters:** `ColliderHandle handle`, `int32_t group`

- **Return:** `void`

---

## AddCollisionHandler



- **Parameters:** `const std::string& name`, `const CollisionCallback& callback`

- **Return:** `void`

---

## RemoveCollisionHandler



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## SetGravity



- **Parameters:** `const Vector3& gravity`

- **Return:** `void`

---

## SetTimestep



- **Parameters:** `float timestep`

- **Return:** `void`

---

## Raycast



- **Parameters:** `const Vector3& origin`, `const Vector3& direction`, `float maxDistance = 1000.0f`

- **Return:** `RaycastResult`

---
