# Classes
---

## TaskPriority
---




# Variables
---

### `TaskHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `TaskFunction`

- **Type:** `using`

- **Default Value:** `std::function<void()>`



### `name`

- **Type:** `string`



### `function`

- **Type:** `TaskFunction`



### `priority`

- **Type:** `TaskPriority`

- **Default Value:** `TaskPriority::Normal`



### `isAsync`

- **Type:** `bool`

- **Default Value:** `false`



### `weight`

- **Type:** `float`

- **Default Value:** `1.0f`



### `activeTasks`

- **Type:** `uint32_t`



### `completedTasks`

- **Type:** `uint32_t`



### `totalDependencies`

- **Type:** `uint32_t`



### `averageLatencyMs`

- **Type:** `float`



### `averageExecutionTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`




# Structs
---

### `TaskDesc`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `function`: `TaskFunction` - 



### `TaskStats`

- **Description:** 

- **Members:**

  - `activeTasks`: `uint32_t` - 

  - `completedTasks`: `uint32_t` - 

  - `totalDependencies`: `uint32_t` - 

  - `averageLatencyMs`: `float` - 

  - `averageExecutionTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 


