# Classes
---

## GPUMemoryDefrag
---




# Variables
---

### `minBlockSize`

- **Type:** `size_t`



### `fragmentationThreshold`

- **Type:** `float`



### `enableLiveDefrag`

- **Type:** `bool`



### `source`

- **Type:** `GPUAllocation`



### `destination`

- **Type:** `GPUAllocation`



### `size`

- **Type:** `size_t`



### `inProgress`

- **Type:** `bool`



### `m_Config`

- **Type:** `DefragConfig`




# Structs
---

### `DefragConfig`

- **Description:** 

- **Members:**

  - `minBlockSize`: `size_t` - 

  - `fragmentationThreshold`: `float` - 

  - `enableLiveDefrag`: `bool` - 



### `DefragOperation`

- **Description:** 

- **Members:**

  - `source`: `GPUAllocation` - 

  - `destination`: `GPUAllocation` - 

  - `size`: `size_t` - 

  - `inProgress`: `bool` - 




# Functions
---

## BeginDefrag



- **Parameters:** const DefragConfig& config

- **Return:** `void`

---

## UpdateDefrag



- **Parameters:** 

- **Return:** `void`

---
