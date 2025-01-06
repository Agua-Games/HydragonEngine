# Classes
---

## NodeFlags
---




# Variables
---

### `NodeHandle`

- **Type:** `using`

- **Default Value:** `uint64_t`



### `name`

- **Type:** `string`



### `localTransform`

- **Type:** `Transform`



### `bounds`

- **Type:** `BoundingVolume`



### `layer`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `flags`

- **Type:** `NodeFlags`

- **Default Value:** `NodeFlags::Visible`



### `desc`

- **Type:** `NodeDesc`



### `parent`

- **Type:** `NodeHandle`



### `worldTransform`

- **Type:** `Transform`



### `worldBounds`

- **Type:** `BoundingVolume`



### `dirty`

- **Type:** `bool`

- **Default Value:** `false`



### `totalNodes`

- **Type:** `uint32_t`



### `visibleNodes`

- **Type:** `uint32_t`



### `culledNodes`

- **Type:** `uint32_t`



### `dynamicNodes`

- **Type:** `uint32_t`



### `staticNodes`

- **Type:** `uint32_t`



### `updateTimeMs`

- **Type:** `float`



### `cullingTimeMs`

- **Type:** `float`




# Structs
---

### `NodeDesc`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `localTransform`: `Transform` - 

  - `bounds`: `BoundingVolume` - 



### `Node`

- **Description:** 

- **Members:**

  - `desc`: `NodeDesc` - 

  - `parent`: `NodeHandle` - 

  - `worldTransform`: `Transform` - 

  - `worldBounds`: `BoundingVolume` - 



### `GraphStats`

- **Description:** 

- **Members:**

  - `totalNodes`: `uint32_t` - 

  - `visibleNodes`: `uint32_t` - 

  - `culledNodes`: `uint32_t` - 

  - `dynamicNodes`: `uint32_t` - 

  - `staticNodes`: `uint32_t` - 

  - `updateTimeMs`: `float` - 

  - `cullingTimeMs`: `float` - 


