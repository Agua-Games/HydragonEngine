
# Variables
---

### `streamingPoolSize`

- **Type:** `size_t`

- **Default Value:** `256 * 1024 * 1024`



### `maxStreamingRequests`

- **Type:** `size_t`

- **Default Value:** `64`



### `enablePrioritization`

- **Type:** `bool`

- **Default Value:** `true`



### `memory`

- **Type:** `void*`



### `size`

- **Type:** `size_t`



### `priority`

- **Type:** `float`



### `lastAccess`

- **Type:** `uint64_t`




# Structs
---

### `StreamingConfig`

- **Description:** 

- **Members:**

  - `1024`: `256 * 1024 *` - 



### `StreamingBlock`

- **Description:** 

- **Members:**

  - `memory`: `void*` - 

  - `size`: `size_t` - 

  - `priority`: `float` - 

  - `lastAccess`: `uint64_t` - 




# Functions
---

## allocateWithHints



- **Parameters:** `{
            .isStreamable = true`, `.priority = request.priority`, `.expectedLifetime = request.expectedLifetime
        }`

- **Return:** `return`

---
