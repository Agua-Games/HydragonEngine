# Classes
---

## WindowBindings
---




# Variables
---

### `enableMultiWindow`

- **Type:** `bool`

- **Default Value:** `true`



### `enableFullscreen`

- **Type:** `bool`

- **Default Value:** `true`



### `enableResizing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDragAndDrop`

- **Type:** `bool`

- **Default Value:** `true`



### `maxWindows`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `maxMonitors`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `maxDropFiles`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `windowConfigPath`

- **Type:** `string`

- **Default Value:** `"Config/Window"`



### `config`

- **Type:** `const WindowBindingsConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `WindowBindingsConfig`



### `m_Stats`

- **Type:** `WindowBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `WindowBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static WindowBindings&`

---

## Initialize



- **Parameters:** `const WindowBindingsConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## CreateWindow



- **Parameters:** `const WindowDesc& desc`

- **Return:** `WindowHandle`

---

## DestroyWindow



- **Parameters:** `WindowHandle handle`

- **Return:** `void`

---

## ShowWindow



- **Parameters:** `WindowHandle handle`

- **Return:** `void`

---

## HideWindow



- **Parameters:** `WindowHandle handle`

- **Return:** `void`

---

## SetTitle



- **Parameters:** `WindowHandle handle`, `const std::string& title`

- **Return:** `void`

---

## SetSize



- **Parameters:** `WindowHandle handle`, `uint32_t width`, `uint32_t height`

- **Return:** `void`

---

## SetPosition



- **Parameters:** `WindowHandle handle`, `int32_t x`, `int32_t y`

- **Return:** `void`

---

## SetFullscreen



- **Parameters:** `WindowHandle handle`, `bool fullscreen`

- **Return:** `void`

---

## SetBorderless



- **Parameters:** `WindowHandle handle`, `bool borderless`

- **Return:** `void`

---

## SetResizable



- **Parameters:** `WindowHandle handle`, `bool resizable`

- **Return:** `void`

---

## SetMinSize



- **Parameters:** `WindowHandle handle`, `uint32_t width`, `uint32_t height`

- **Return:** `void`

---

## SetMaxSize



- **Parameters:** `WindowHandle handle`, `uint32_t width`, `uint32_t height`

- **Return:** `void`

---

## RegisterWindowCallback



- **Parameters:** `WindowHandle handle`, `const WindowCallback& callback`

- **Return:** `void`

---

## UnregisterWindowCallback



- **Parameters:** `WindowHandle handle`

- **Return:** `void`

---

## RegisterDropCallback



- **Parameters:** `WindowHandle handle`, `const DropCallback& callback`

- **Return:** `void`

---

## UnregisterDropCallback



- **Parameters:** `WindowHandle handle`

- **Return:** `void`

---

## ShowCursor



- **Parameters:** `WindowHandle handle`, `bool show`

- **Return:** `void`

---

## SetCursorPosition



- **Parameters:** `WindowHandle handle`, `int32_t x`, `int32_t y`

- **Return:** `void`

---

## SetCursorMode



- **Parameters:** `WindowHandle handle`, `CursorMode mode`

- **Return:** `void`

---
