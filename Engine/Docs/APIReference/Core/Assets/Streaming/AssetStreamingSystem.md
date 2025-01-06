# Classes
---

## AssetStreamingSystem
---




# Variables
---

### `streamingBudget`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024 * 1024`



### `maxConcurrentStreams`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `enablePrioritization`

- **Type:** `bool`

- **Default Value:** `true`



### `streamingLatencyMs`

- **Type:** `float`

- **Default Value:** `16.67f`



### `config`

- **Type:** `const StreamingConfig&`

- **Default Value:** `{})`



### `handle`

- **Type:** `AssetHandle`



### `params`

- **Type:** `LoadParams`



### `priority`

- **Type:** `float`



### `m_Config`

- **Type:** `StreamingConfig`




# Structs
---

### `StreamingConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 * 1024 *` - 



### `StreamRequest`

- **Description:** 

- **Members:**

  - `handle`: `AssetHandle` - 

  - `params`: `LoadParams` - 

  - `priority`: `float` - 




# Functions
---

## Initialize



- **Parameters:** const StreamingConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## RequestStream



- **Parameters:** AssetHandle handle, const LoadParams& params

- **Return:** `void`

---

## CancelStream



- **Parameters:** AssetHandle handle

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---
