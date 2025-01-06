# Classes
---

## MathSystem
---




# Variables
---

### `enableSIMD`

- **Type:** `bool`

- **Default Value:** `true`



### `enableGPUAcceleration`

- **Type:** `bool`

- **Default Value:** `true`



### `maxThreads`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `workGroupSize`

- **Type:** `size_t`

- **Default Value:** `256`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `64 * 1024 * 1024`



### `config`

- **Type:** `const MathConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `MathConfig`



### `m_Stats`

- **Type:** `MathStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `MathConfig`

- **Description:** 

- **Members:**

  - `1024`: `64 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static MathSystem&`

---

## Initialize



- **Parameters:** `const MathConfig& config = {}`

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

## MultiplyMatrices



- **Parameters:** `const Matrix4x4* matrices`, `uint32_t count`, `Matrix4x4* results`

- **Return:** `void`

---

## InvertMatrices



- **Parameters:** `const Matrix4x4* matrices`, `uint32_t count`, `Matrix4x4* results`

- **Return:** `void`

---

## TransformVectors



- **Parameters:** `const Vector4* vectors`, `const Matrix4x4& transform`, `uint32_t count`, `Vector4* results`

- **Return:** `void`

---

## NormalizeVectors



- **Parameters:** `const Vector4* vectors`, `uint32_t count`, `Vector4* results`

- **Return:** `void`

---

## SlerpQuaternions



- **Parameters:** `const Quaternion* start`, `const Quaternion* end`, `float* t`, `uint32_t count`, `Quaternion* results`

- **Return:** `void`

---

## BatchProcess



- **Parameters:** `const ComputeTask& task`

- **Return:** `void`

---

## ScheduleTask



- **Parameters:** `const ComputeTask& task`

- **Return:** `void`

---
