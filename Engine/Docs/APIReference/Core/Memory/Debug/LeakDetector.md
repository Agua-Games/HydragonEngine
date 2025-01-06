# Classes
---

## LeakDetector
---




# Variables
---

### `ptr`

- **Type:** `void*`



### `size`

- **Type:** `size_t`



### `file`

- **Type:** `const char*`



### `line`

- **Type:** `int`



### `timestamp`

- **Type:** `uint64_t`



### `callstack`

- **Type:** `Callstack`



### `threadId`

- **Type:** `id`



### `record`

- **Type:** `AllocationRecord`



### `totalLeaks`

- **Type:** `size_t`

- **Default Value:** `0`



### `totalBytes`

- **Type:** `size_t`

- **Default Value:** `0`



### `m_Mutex`

- **Type:** `mutex`




# Structs
---

### `AllocationRecord`

- **Description:** 

- **Members:**

  - `ptr`: `void*` - 

  - `size`: `size_t` - 

  - `file`: `const char*` - 

  - `line`: `int` - 

  - `timestamp`: `uint64_t` - 

  - `callstack`: `Callstack` - 

  - `threadId`: `id` - 




# Functions
---

## report



- **Parameters:** filename

- **Return:** `ofstream`

---
