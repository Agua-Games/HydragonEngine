# Classes
---

## WindowManagerSystem
---




# Variables
---

### `enableDocking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTabbing`

- **Type:** `bool`

- **Default Value:** `true`



### `enableSnapping`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAutoHide`

- **Type:** `bool`

- **Default Value:** `true`



### `maxDockedWindows`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `maxTabbedGroups`

- **Type:** `uint32_t`

- **Default Value:** `20`



### `dockingThreshold`

- **Type:** `float`

- **Default Value:** `20.0f`



### `config`

- **Type:** `const WindowManagerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `WindowManagerConfig`



### `m_Stats`

- **Type:** `WindowManagerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `WindowManagerConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static WindowManagerSystem&`

---

## Initialize



- **Parameters:** `const WindowManagerConfig& config = {}`

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

## CreateDockSpace



- **Parameters:** `const std::string& name`, `const DockSpaceDesc& desc`

- **Return:** `DockID`

---

## DestroyDockSpace



- **Parameters:** `DockID id`

- **Return:** `void`

---

## DockWindow



- **Parameters:** `const std::string& window`, `DockID dock`, `DockSlot slot`

- **Return:** `void`

---

## UndockWindow



- **Parameters:** `const std::string& window`

- **Return:** `void`

---

## GroupWindows



- **Parameters:** `const std::vector<std::string>& windows`

- **Return:** `void`

---

## UngroupWindows



- **Parameters:** `const std::vector<std::string>& windows`

- **Return:** `void`

---

## SetWindowVisibility



- **Parameters:** `const std::string& window`, `bool visible`

- **Return:** `void`

---
