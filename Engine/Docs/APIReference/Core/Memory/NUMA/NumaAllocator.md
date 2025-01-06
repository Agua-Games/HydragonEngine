
# Variables
---

### `preferLocalAllocation`

- **Type:** `bool`

- **Default Value:** `true`



### `strictNodeBinding`

- **Type:** `bool`

- **Default Value:** `false`



### `nodeReserveSize`

- **Type:** `size_t`

- **Default Value:** `64 * 1024 * 1024`



### `config`

- **Type:** `const NumaConfig&`

- **Default Value:** `{})`



### `preferredNode`

- **Type:** `uint32_t`

- **Default Value:** `info.numaNode`



### `preferredNode`

- **Type:** `Get the node of the current thread`

- **Default Value:** `Platform::GetCurrentNumaNode()`



### `ptr`

- **Type:** `void*`

- **Default Value:** `Platform::NumaAllocateOnNode(size, preferredNode)`



### `ptr`

- **Type:** `Fallback to any node if strict binding is not required`

- **Default Value:** `Platform::NumaAllocateAny(size)`



### `ptr`

- **Type:** `return`



### `m_Config`

- **Type:** `NumaConfig`




# Structs
---

### `NumaConfig`

- **Description:** 

- **Members:**

  - `1024`: `64 * 1024 *` - 




# Functions
---

## NumaAllocator



- **Parameters:** const NumaConfig& config = {}

- **Return:** `explicit`

---
