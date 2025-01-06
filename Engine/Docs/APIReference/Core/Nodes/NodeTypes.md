# Classes
---

## NodeCategory
---




# Variables
---

### `GraphHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `NodeHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `name`

- **Type:** `string`



### `isPersistent`

- **Type:** `bool`

- **Default Value:** `false`



### `autoExecute`

- **Type:** `bool`

- **Default Value:** `false`



### `updateRate`

- **Type:** `float`

- **Default Value:** `60.0f`



### `category`

- **Type:** `NodeCategory`



### `type`

- **Type:** `string`



### `defaultValue`

- **Type:** `any`



### `activeGraphs`

- **Type:** `uint32_t`



### `totalNodes`

- **Type:** `uint32_t`



### `totalConnections`

- **Type:** `uint32_t`



### `averageExecutionTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`




# Structs
---

### `GraphDesc`

- **Description:** 

- **Members:**

  - `name`: `string` - 



### `NodeDesc`

- **Description:** 

- **Members:**

  - `category`: `NodeCategory` - 

  - `type`: `string` - 

  - `defaultValue`: `any` - 



### `NodeStats`

- **Description:** 

- **Members:**

  - `activeGraphs`: `uint32_t` - 

  - `totalNodes`: `uint32_t` - 

  - `totalConnections`: `uint32_t` - 

  - `averageExecutionTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 


