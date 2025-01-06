# Classes
---

## EntityManager
---




# Variables
---

### `initialEntityCapacity`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxComponents`

- **Type:** `uint32_t`

- **Default Value:** `64`



### `enableComponentPools`

- **Type:** `bool`

- **Default Value:** `true`



### `poolMemoryBudget`

- **Type:** `size_t`

- **Default Value:** `64 * 1024 * 1024`



### `config`

- **Type:** `const EntityConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `EntityConfig`



### `m_Stats`

- **Type:** `EntityStats`




# Structs
---

### `EntityConfig`

- **Description:** 

- **Members:**

  - `1024`: `64 * 1024 *` - 




# Functions
---

## EntityManager



- **Parameters:** `const EntityConfig& config = {}`

- **Return:** `explicit`

---

## CreateEntity



- **Parameters:** ``

- **Return:** `EntityHandle`

---

## DestroyEntity



- **Parameters:** `EntityHandle entity`

- **Return:** `void`

---

## AddComponent



- **Parameters:** `EntityHandle entity`

- **Return:** `T*`

---

## RemoveComponent



- **Parameters:** `EntityHandle entity`

- **Return:** `void`

---

## GetComponent



- **Parameters:** `EntityHandle entity`

- **Return:** `T*`

---
