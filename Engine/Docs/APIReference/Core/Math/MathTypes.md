
# Variables
---

### `position`

- **Type:** `Vector3`



### `rotation`

- **Type:** `Quaternion`



### `scale`

- **Type:** `Vector3`



### `ComputeCallback`

- **Type:** `using`

- **Default Value:** `std::function<void(void*, uint32_t)>`



### `data`

- **Type:** `void*`



### `elementCount`

- **Type:** `uint32_t`



### `elementSize`

- **Type:** `uint32_t`



### `callback`

- **Type:** `ComputeCallback`



### `parallel`

- **Type:** `bool`

- **Default Value:** `true`



### `matrixOps`

- **Type:** `uint64_t`



### `vectorOps`

- **Type:** `uint64_t`



### `quaternionOps`

- **Type:** `uint64_t`



### `batchTasks`

- **Type:** `uint32_t`



### `activeThreads`

- **Type:** `uint32_t`



### `computeTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`




# Structs
---

### `Vector2`

- **Description:** 

- **Members:**



### `Vector3`

- **Description:** 

- **Members:**



### `Vector4`

- **Description:** 

- **Members:**



### `Quaternion`

- **Description:** 

- **Members:**



### `Matrix3x3`

- **Description:** 

- **Members:**



### `Matrix4x4`

- **Description:** 

- **Members:**



### `Transform`

- **Description:** 

- **Members:**

  - `position`: `Vector3` - 

  - `rotation`: `Quaternion` - 

  - `scale`: `Vector3` - 



### `ComputeTask`

- **Description:** 

- **Members:**

  - `data`: `void*` - 

  - `elementCount`: `uint32_t` - 

  - `elementSize`: `uint32_t` - 

  - `callback`: `ComputeCallback` - 



### `MathStats`

- **Description:** 

- **Members:**

  - `matrixOps`: `uint64_t` - 

  - `vectorOps`: `uint64_t` - 

  - `quaternionOps`: `uint64_t` - 

  - `batchTasks`: `uint32_t` - 

  - `activeThreads`: `uint32_t` - 

  - `computeTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 




# Functions
---

## Identity



- **Parameters:** ``

- **Return:** `void`

---

## Identity



- **Parameters:** ``

- **Return:** `void`

---
