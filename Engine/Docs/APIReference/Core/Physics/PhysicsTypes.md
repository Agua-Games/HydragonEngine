# Classes
---

## BodyType
---



## ColliderType
---




# Variables
---

### `RigidBodyHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `ColliderHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `ConstraintHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `type`

- **Type:** `BodyType`

- **Default Value:** `BodyType::Dynamic`



### `mass`

- **Type:** `float`

- **Default Value:** `1.0f`



### `position`

- **Type:** `Vector3`



### `rotation`

- **Type:** `Quaternion`



### `enableCCD`

- **Type:** `bool`

- **Default Value:** `false`



### `friction`

- **Type:** `float`

- **Default Value:** `0.5f`



### `restitution`

- **Type:** `float`

- **Default Value:** `0.0f`



### `collisionGroup`

- **Type:** `uint32_t`

- **Default Value:** `1`



### `collisionMask`

- **Type:** `uint32_t`

- **Default Value:** `0xFFFFFFFF`



### `type`

- **Type:** `ColliderType`



### `size`

- **Type:** `Vector3`



### `density`

- **Type:** `float`

- **Default Value:** `1.0f`



### `isTrigger`

- **Type:** `bool`

- **Default Value:** `false`



### `isExclusive`

- **Type:** `bool`

- **Default Value:** `false`



### `bodyA`

- **Type:** `RigidBodyHandle`



### `bodyB`

- **Type:** `RigidBodyHandle`



### `pivotA`

- **Type:** `Vector3`



### `pivotB`

- **Type:** `Vector3`



### `enableCollision`

- **Type:** `bool`

- **Default Value:** `true`



### `breakingForce`

- **Type:** `float`

- **Default Value:** `FLT_MAX`



### `origin`

- **Type:** `Vector3`



### `direction`

- **Type:** `Vector3`



### `maxDistance`

- **Type:** `float`

- **Default Value:** `FLT_MAX`



### `RayCastCallback`

- **Type:** `using`

- **Default Value:** `std::function<bool(const RayCastHit&)>`



### `activeBodies`

- **Type:** `uint32_t`



### `activeColliders`

- **Type:** `uint32_t`



### `activeConstraints`

- **Type:** `uint32_t`



### `activeContacts`

- **Type:** `uint32_t`



### `simulationTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`




# Structs
---

### `RigidBodyDesc`

- **Description:** 

- **Members:**

  - `position`: `Vector3` - 

  - `rotation`: `Quaternion` - 



### `ColliderDesc`

- **Description:** 

- **Members:**

  - `type`: `ColliderType` - 

  - `size`: `Vector3` - 



### `ConstraintDesc`

- **Description:** 

- **Members:**

  - `bodyA`: `RigidBodyHandle` - 

  - `bodyB`: `RigidBodyHandle` - 

  - `pivotA`: `Vector3` - 

  - `pivotB`: `Vector3` - 



### `Ray`

- **Description:** 

- **Members:**

  - `origin`: `Vector3` - 

  - `direction`: `Vector3` - 



### `PhysicsStats`

- **Description:** 

- **Members:**

  - `activeBodies`: `uint32_t` - 

  - `activeColliders`: `uint32_t` - 

  - `activeConstraints`: `uint32_t` - 

  - `activeContacts`: `uint32_t` - 

  - `simulationTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 


