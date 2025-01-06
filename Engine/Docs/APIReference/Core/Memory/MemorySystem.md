# Classes
---

## MemorySystem
---




# Variables
---

### `config`

- **Type:** `const MemoryConfig&`

- **Default Value:** `{})`



### `info`

- **Type:** `const AllocationInfo&`

- **Default Value:** `{})`



### `m_Initialized`

- **Type:** `return`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`



### `m_DefaultStrategy`

- **Type:** `IMemoryStrategy*`

- **Default Value:** `nullptr`




# Functions
---

## get



- **Parameters:** 

- **Return:** `static MemorySystem&`

---

## initialize



- **Parameters:** const MemoryConfig& config = {}

- **Return:** `void`

---

## shutdown



- **Parameters:** 

- **Return:** `void`

---

## allocate



- **Parameters:** size_t size, const AllocationInfo& info = {}

- **Return:** `void*`

---

## deallocate



- **Parameters:** void* ptr

- **Return:** `void`

---

## addStrategy



- **Parameters:** const std::string& name, IMemoryStrategy* strategy

- **Return:** `void`

---

## setDefaultStrategy



- **Parameters:** const std::string& name

- **Return:** `void`

---
