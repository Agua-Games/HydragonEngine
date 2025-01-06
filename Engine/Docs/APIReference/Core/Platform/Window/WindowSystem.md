# Classes
---

## WindowSystem
---




# Variables
---

### `enableTransparency`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDragDrop`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMultiWindow`

- **Type:** `bool`

- **Default Value:** `true`



### `maxWindows`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `defaultWidth`

- **Type:** `uint32_t`

- **Default Value:** `1280`



### `defaultHeight`

- **Type:** `uint32_t`

- **Default Value:** `720`



### `config`

- **Type:** `const WindowConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `WindowConfig`



### `m_Stats`

- **Type:** `WindowStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `WindowConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static WindowSystem&`

---

## Initialize



- **Parameters:** const WindowConfig& config = {}

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

## SetWindowPosition



- **Parameters:** WindowHandle handle, int x, int y

- **Return:** `void`

---

## SetWindowSize



- **Parameters:** WindowHandle handle, uint32_t width, uint32_t height

- **Return:** `void`

---

## SetWindowTitle



- **Parameters:** WindowHandle handle, const std::string& title

- **Return:** `void`

---

## SetWindowIcon



- **Parameters:** WindowHandle handle, const std::string& iconPath

- **Return:** `void`

---

## ShowWindow



- **Parameters:** WindowHandle handle

- **Return:** `void`

---

## HideWindow



- **Parameters:** WindowHandle handle

- **Return:** `void`

---

## MinimizeWindow



- **Parameters:** WindowHandle handle

- **Return:** `void`

---

## MaximizeWindow



- **Parameters:** WindowHandle handle

- **Return:** `void`

---
