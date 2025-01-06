# Classes
---

## ThreadSystem
---




# Variables
---

### `maxThreads`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `maxFibers`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `enableWorkStealing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableFiberPooling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableThreadAffinity`

- **Type:** `bool`

- **Default Value:** `true`



### `stackSize`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024`



### `fiberStackSize`

- **Type:** `size_t`

- **Default Value:** `64 * 1024`



### `config`

- **Type:** `const ThreadConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ThreadConfig`



### `m_Stats`

- **Type:** `ThreadStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ThreadConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 

  - `1024`: `64 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static ThreadSystem&`

---

## Initialize



- **Parameters:** const ThreadConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## CreateThread



- **Parameters:** const ThreadDesc& desc

- **Return:** `ThreadHandle`

---

## DestroyThread



- **Parameters:** ThreadHandle handle

- **Return:** `void`

---

## CreateFiber



- **Parameters:** const FiberDesc& desc

- **Return:** `FiberHandle`

---

## DestroyFiber



- **Parameters:** FiberHandle handle

- **Return:** `void`

---

## SetThreadAffinity



- **Parameters:** ThreadHandle thread, uint32_t coreMask

- **Return:** `void`

---

## SetThreadPriority



- **Parameters:** ThreadHandle thread, ThreadPriority priority

- **Return:** `void`

---

## SwitchToFiber



- **Parameters:** FiberHandle fiber

- **Return:** `void`

---

## YieldFiber



- **Parameters:** 

- **Return:** `void`

---
