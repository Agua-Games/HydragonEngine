# Classes
---

## WidgetManagerSystem
---




# Variables
---

### `enableCustomWidgets`

- **Type:** `bool`

- **Default Value:** `true`



### `enableWidgetCache`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDragDrop`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTooltips`

- **Type:** `bool`

- **Default Value:** `true`



### `maxCustomWidgets`

- **Type:** `uint32_t`

- **Default Value:** `500`



### `cacheSize`

- **Type:** `uint32_t`

- **Default Value:** `1024 * 1024`



### `tooltipDelay`

- **Type:** `float`

- **Default Value:** `0.5f`



### `config`

- **Type:** `const WidgetManagerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `WidgetManagerConfig`



### `m_Stats`

- **Type:** `WidgetManagerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `WidgetManagerConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static WidgetManagerSystem&`

---

## Initialize



- **Parameters:** const WidgetManagerConfig& config = {}

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

## RegisterWidget



- **Parameters:** const std::string& type, std::unique_ptr<IWidget> widget

- **Return:** `void`

---

## UnregisterWidget



- **Parameters:** const std::string& type

- **Return:** `void`

---

## CreateWidget



- **Parameters:** const std::string& type, const WidgetDesc& desc

- **Return:** `WidgetHandle`

---

## DestroyWidget



- **Parameters:** WidgetHandle handle

- **Return:** `void`

---

## SetWidgetProperty



- **Parameters:** WidgetHandle handle, const std::string& name, 
                          const PropertyValue& value

- **Return:** `void`

---

## EnableDragDrop



- **Parameters:** WidgetHandle handle, const DragDropConfig& config

- **Return:** `void`

---

## DisableDragDrop



- **Parameters:** WidgetHandle handle

- **Return:** `void`

---
