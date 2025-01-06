
# Variables
---

### `size`

- **Type:** `double`



### `label`

- **Type:** `QString`



### `m_MinBlockSize`

- **Type:** `size_t`

- **Default Value:** `1024`



### `m_MaxBlockSize`

- **Type:** `size_t`

- **Default Value:** `1024*1024`




# Structs
---

### `BlockPosition`

- **Description:** 

- **Members:**

  - `size`: `double` - 

  - `label`: `QString` - 




# Functions
---

## mapToPosition



- **Parameters:** `const Memory::MemoryBlock& block`

- **Return:** `BlockPosition`

---

## mapGroupToPosition



- **Parameters:** `const BlockGroup& group`

- **Return:** `BlockPosition`

---

## calculateVisualSize



- **Parameters:** `size_t memorySize`

- **Return:** `double`

---

## findFreePosition



- **Parameters:** `double size`

- **Return:** `QPointF`

---
