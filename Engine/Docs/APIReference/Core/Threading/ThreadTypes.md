# Classes
---

## ThreadPriority
---




# Variables
---

### `ThreadHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `FiberHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `ThreadFunction`

- **Type:** `using`

- **Default Value:** `std::function<void()>`



### `FiberFunction`

- **Type:** `using`

- **Default Value:** `std::function<void()>`



### `name`

- **Type:** `string`



### `function`

- **Type:** `ThreadFunction`



### `priority`

- **Type:** `ThreadPriority`

- **Default Value:** `ThreadPriority::Normal`



### `coreMask`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `stackSize`

- **Type:** `size_t`

- **Default Value:** `0`



### `name`

- **Type:** `string`



### `function`

- **Type:** `FiberFunction`



### `stackSize`

- **Type:** `size_t`

- **Default Value:** `0`



### `pooled`

- **Type:** `bool`

- **Default Value:** `true`



### `activeThreads`

- **Type:** `uint32_t`



### `activeFibers`

- **Type:** `uint32_t`



### `pooledFibers`

- **Type:** `uint32_t`



### `contextSwitches`

- **Type:** `uint32_t`



### `averageUtilization`

- **Type:** `float`



### `totalStackMemory`

- **Type:** `size_t`




# Structs
---

### `ThreadDesc`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `function`: `ThreadFunction` - 



### `FiberDesc`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `function`: `FiberFunction` - 



### `ThreadStats`

- **Description:** 

- **Members:**

  - `activeThreads`: `uint32_t` - 

  - `activeFibers`: `uint32_t` - 

  - `pooledFibers`: `uint32_t` - 

  - `contextSwitches`: `uint32_t` - 

  - `averageUtilization`: `float` - 

  - `totalStackMemory`: `size_t` - 


