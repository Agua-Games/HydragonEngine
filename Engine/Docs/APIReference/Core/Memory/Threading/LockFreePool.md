# Classes
---

## LockFreePool
---




# Variables
---

### `memory`

- **Type:** `char*`

- **Default Value:** `static_cast<char*>(Platform::AlignedAlloc(BlockSize * BlockCount, HD_CACHE_LINE))`



### `i`

- **Type:** `size_t`

- **Default Value:** `0`



### `current`

- **Type:** `void*`

- **Default Value:** `memory + (i * BlockSize)`



### `next`

- **Type:** `void*`

- **Default Value:** `memory + ((i + 1) * BlockSize)`



### `ptr`

- **Type:** `void*`



### `expected`

- **Type:** `void*`

- **Default Value:** `m_Head.load(std::memory_order_acquire)`



### `nullptr`

- **Type:** `return`



### `ptr`

- **Type:** `Pool is empty`

- **Default Value:** `expected`



### `next`

- **Type:** `void*`

- **Default Value:** `*static_cast<void**>(ptr)`



### `ptr`

- **Type:** `return`



### `expected`

- **Type:** `void*`

- **Default Value:** `m_Head.load(std::memory_order_acquire)`


