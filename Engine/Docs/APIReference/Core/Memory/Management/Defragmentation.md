# Classes
---

## Defragmenter
---




# Variables
---

### `fragmentedBytes`

- **Type:** `size_t`



### `recoveredBytes`

- **Type:** `size_t`



### `movedBlocks`

- **Type:** `size_t`



### `fragmentationRatio`

- **Type:** `double`



### `totalGaps`

- **Type:** `size_t`

- **Default Value:** `0`



### `maxContiguous`

- **Type:** `size_t`

- **Default Value:** `0`



### `i`

- **Type:** `size_t`

- **Default Value:** `1`



### `gap`

- **Type:** `size_t`

- **Default Value:** `getGapSize(m_Blocks[i-1], m_Blocks[i])`



### `stats`

- **Type:** `return`



### `target`

- **Type:** `void*`

- **Default Value:** `m_HeapStart`



### `stats`

- **Type:** `auto`

- **Default Value:** `analyze()`




# Structs
---

### `DefragStats`

- **Description:** 

- **Members:**

  - `fragmentedBytes`: `size_t` - 

  - `recoveredBytes`: `size_t` - 

  - `movedBlocks`: `size_t` - 

  - `fragmentationRatio`: `double` - 


