# Classes
---

## NodeType
---



## ParameterType
---




# Variables
---

### `NodeHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `type`

- **Type:** `ParameterType`



### `isLazy`

- **Type:** `bool`

- **Default Value:** `false`



### `type`

- **Type:** `NodeType`



### `name`

- **Type:** `string`



### `isProcedural`

- **Type:** `bool`

- **Default Value:** `false`



### `isStatic`

- **Type:** `bool`

- **Default Value:** `false`



### `fromPort`

- **Type:** `string`



### `toPort`

- **Type:** `string`



### `isLazy`

- **Type:** `bool`

- **Default Value:** `false`



### `totalNodes`

- **Type:** `uint32_t`



### `totalConnections`

- **Type:** `uint32_t`



### `evaluatedNodes`

- **Type:** `uint32_t`



### `cachedResults`

- **Type:** `uint32_t`



### `memoryUsed`

- **Type:** `size_t`



### `evaluationTimeMs`

- **Type:** `float`




# Structs
---

### `Parameter`

- **Description:** 

- **Members:**

  - `type`: `ParameterType` - 



### `NodeDesc`

- **Description:** 

- **Members:**

  - `type`: `NodeType` - 

  - `name`: `string` - 



### `ConnectionDesc`

- **Description:** 

- **Members:**

  - `fromPort`: `string` - 

  - `toPort`: `string` - 



### `DescriptionStats`

- **Description:** 

- **Members:**

  - `totalNodes`: `uint32_t` - 

  - `totalConnections`: `uint32_t` - 

  - `evaluatedNodes`: `uint32_t` - 

  - `cachedResults`: `uint32_t` - 

  - `memoryUsed`: `size_t` - 

  - `evaluationTimeMs`: `float` - 


