# Classes
---

## PlatformSystem
---




# Variables
---

### `enableHighDPI`

- **Type:** `bool`

- **Default Value:** `true`



### `enableRawInput`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMultitouch`

- **Type:** `bool`

- **Default Value:** `true`



### `enableGamepad`

- **Type:** `bool`

- **Default Value:** `true`



### `enableClipboard`

- **Type:** `bool`

- **Default Value:** `true`



### `catchExceptions`

- **Type:** `bool`

- **Default Value:** `true`



### `maxWindows`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `32 * 1024 * 1024`



### `config`

- **Type:** `const PlatformConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `PlatformConfig`



### `m_Stats`

- **Type:** `PlatformStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `PlatformConfig`

- **Description:** 

- **Members:**

  - `1024`: `32 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static PlatformSystem&`

---

## Initialize



- **Parameters:** const PlatformConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## CreateWindow



- **Parameters:** const WindowDesc& desc

- **Return:** `WindowHandle`

---

## DestroyWindow



- **Parameters:** WindowHandle handle

- **Return:** `void`

---

## SetWindowTitle



- **Parameters:** WindowHandle handle, const std::string& title

- **Return:** `void`

---

## SetWindowSize



- **Parameters:** WindowHandle handle, uint32_t width, uint32_t height

- **Return:** `void`

---

## PollEvents



- **Parameters:** 

- **Return:** `void`

---

## CreateDirectory



- **Parameters:** const std::string& path

- **Return:** `bool`

---
