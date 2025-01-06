# Classes
---

## PhysicsSystem
---




# Variables
---

### `fixedTimeStep`

- **Type:** `float`

- **Default Value:** `1.0f/60.0f`



### `maxSubSteps`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `maxBodies`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `maxConstraints`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `enableCCD`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMultithreading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDebugDraw`

- **Type:** `bool`

- **Default Value:** `true`



### `gravity`

- **Type:** `Vector3`

- **Default Value:** `{0.0f, -9.81f, 0.0f}`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `256 * 1024 * 1024`



### `config`

- **Type:** `const PhysicsConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `PhysicsConfig`



### `m_Stats`

- **Type:** `PhysicsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `PhysicsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static PhysicsSystem&`

---

## Initialize



- **Parameters:** const PhysicsConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** float deltaTime

- **Return:** `void`

---

## StepSimulation



- **Parameters:** float timeStep

- **Return:** `void`

---

## CreateRigidBody



- **Parameters:** const RigidBodyDesc& desc

- **Return:** `RigidBodyHandle`

---

## DestroyRigidBody



- **Parameters:** RigidBodyHandle handle

- **Return:** `void`

---

## CreateCollider



- **Parameters:** const ColliderDesc& desc

- **Return:** `ColliderHandle`

---

## DestroyCollider



- **Parameters:** ColliderHandle handle

- **Return:** `void`

---

## CreateConstraint



- **Parameters:** const ConstraintDesc& desc

- **Return:** `ConstraintHandle`

---

## DestroyConstraint



- **Parameters:** ConstraintHandle handle

- **Return:** `void`

---

## SetGravity



- **Parameters:** const Vector3& gravity

- **Return:** `void`

---

## RayCast



- **Parameters:** const Ray& ray, RayCastCallback callback

- **Return:** `void`

---
