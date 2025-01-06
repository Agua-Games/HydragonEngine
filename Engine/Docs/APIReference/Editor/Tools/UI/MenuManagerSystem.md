# Classes
---

## MenuManagerSystem
---




# Variables
---

### `enableCustomMenus`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHotkeys`

- **Type:** `bool`

- **Default Value:** `true`



### `enableContextMenus`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDynamicMenus`

- **Type:** `bool`

- **Default Value:** `true`



### `maxMenuDepth`

- **Type:** `uint32_t`

- **Default Value:** `5`



### `maxRecentItems`

- **Type:** `uint32_t`

- **Default Value:** `10`



### `menuConfigPath`

- **Type:** `string`

- **Default Value:** `"Editor/Config/Menus"`



### `config`

- **Type:** `const MenuManagerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `MenuManagerConfig`



### `m_Stats`

- **Type:** `MenuManagerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `MenuManagerConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static MenuManagerSystem&`

---

## Initialize



- **Parameters:** const MenuManagerConfig& config = {}

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

## CreateMenu



- **Parameters:** const std::string& path, const MenuDesc& desc

- **Return:** `MenuHandle`

---

## DestroyMenu



- **Parameters:** MenuHandle handle

- **Return:** `void`

---

## AddMenuItem



- **Parameters:** MenuHandle menu, const MenuItemDesc& desc

- **Return:** `void`

---

## RemoveMenuItem



- **Parameters:** MenuHandle menu, const std::string& id

- **Return:** `void`

---

## RegisterMenuAction



- **Parameters:** const std::string& id, const MenuActionCallback& callback

- **Return:** `void`

---

## UnregisterMenuAction



- **Parameters:** const std::string& id

- **Return:** `void`

---

## ShowContextMenu



- **Parameters:** const std::string& type, const Vector2& position

- **Return:** `void`

---

## HideContextMenu



- **Parameters:** 

- **Return:** `void`

---
