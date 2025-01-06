
# Variables
---

### `uiElementPoolSize`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024`



### `commandBufferSize`

- **Type:** `size_t`

- **Default Value:** `512 * 1024`



### `enableDefragmentation`

- **Type:** `bool`

- **Default Value:** `true`



### `defragInterval`

- **Type:** `uint32_t`

- **Default Value:** `5000`



### `config`

- **Type:** `const Config&`

- **Default Value:** `{})`



### `isLongLived`

- **Type:** `bool`



### `isFrequentlyUsed`

- **Type:** `bool`



### `needsQuickAccess`

- **Type:** `bool`



### `memory`

- **Type:** `void*`



### `size`

- **Type:** `size_t`



### `used`

- **Type:** `size_t`



### `isLongLived`

- **Type:** `bool`



### `buffer`

- **Type:** `void*`



### `capacity`

- **Type:** `size_t`



### `head`

- **Type:** `size_t`



### `tail`

- **Type:** `size_t`



### `m_LastDefragTime`

- **Type:** `uint64_t`




# Structs
---

### `Config`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 

  - `1024`: `512 *` - 



### `UIAllocationHint`

- **Description:** 

- **Members:**

  - `isLongLived`: `bool` - 

  - `isFrequentlyUsed`: `bool` - 

  - `needsQuickAccess`: `bool` - 



### `UIPool`

- **Description:** 

- **Members:**

  - `memory`: `void*` - 

  - `size`: `size_t` - 

  - `used`: `size_t` - 

  - `isLongLived`: `bool` - 



### `CommandBuffer`

- **Description:** 

- **Members:**

  - `buffer`: `void*` - 

  - `capacity`: `size_t` - 

  - `head`: `size_t` - 

  - `tail`: `size_t` - 




# Functions
---

## EditorMemoryStrategy



- **Parameters:** `const Config& config = {}`

- **Return:** `explicit`

---

## defragmentIfNeeded



- **Parameters:** ``

- **Return:** `void`

---
