# Classes
---

## AnimationSystem
---




# Variables
---

### `maxAnimations`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxSkeletons`

- **Type:** `uint32_t`

- **Default Value:** `500`



### `maxBones`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `enableGPUSkinning`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCompression`

- **Type:** `bool`

- **Default Value:** `true`



### `enableBlending`

- **Type:** `bool`

- **Default Value:** `true`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `128 * 1024 * 1024`



### `config`

- **Type:** `const AnimationConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AnimationConfig`



### `m_Stats`

- **Type:** `AnimationStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AnimationConfig`

- **Description:** 

- **Members:**

  - `1024`: `128 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static AnimationSystem&`

---

## Initialize



- **Parameters:** `const AnimationConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** `float deltaTime`

- **Return:** `void`

---

## CreateAnimation



- **Parameters:** `const AnimationDesc& desc`

- **Return:** `AnimationHandle`

---

## DestroyAnimation



- **Parameters:** `AnimationHandle handle`

- **Return:** `void`

---

## CreateSkeleton



- **Parameters:** `const SkeletonDesc& desc`

- **Return:** `SkeletonHandle`

---

## DestroySkeleton



- **Parameters:** `SkeletonHandle handle`

- **Return:** `void`

---

## PlayAnimation



- **Parameters:** `AnimationHandle anim`, `SkeletonHandle skeleton`

- **Return:** `void`

---

## StopAnimation



- **Parameters:** `AnimationHandle anim`, `SkeletonHandle skeleton`

- **Return:** `void`

---

## BlendAnimation



- **Parameters:** `AnimationHandle from`, `AnimationHandle to`, `float blendTime`

- **Return:** `void`

---

## SetAnimationTime



- **Parameters:** `AnimationHandle handle`, `float time`

- **Return:** `void`

---

## SetAnimationSpeed



- **Parameters:** `AnimationHandle handle`, `float speed`

- **Return:** `void`

---
