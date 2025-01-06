# Classes
---

## UIBindings
---




# Variables
---

### `enableAnimations`

- **Type:** `bool`

- **Default Value:** `true`



### `enableLayoutCache`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEventBubbling`

- **Type:** `bool`

- **Default Value:** `true`



### `enableStyleSheets`

- **Type:** `bool`

- **Default Value:** `true`



### `maxWidgets`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxAnimations`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxEventHandlers`

- **Type:** `uint32_t`

- **Default Value:** `500`



### `uiResourcePath`

- **Type:** `string`

- **Default Value:** `"Data/UI"`



### `config`

- **Type:** `const UIBindingsConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `UIBindingsConfig`



### `m_Stats`

- **Type:** `UIBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `UIBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static UIBindings&`

---

## Initialize



- **Parameters:** const UIBindingsConfig& config = {}

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

## CreateWidget



- **Parameters:** const std::string& type, const WidgetDesc& desc

- **Return:** `WidgetHandle`

---

## DestroyWidget



- **Parameters:** WidgetHandle handle

- **Return:** `void`

---

## SetParent



- **Parameters:** WidgetHandle child, WidgetHandle parent

- **Return:** `void`

---

## SetPosition



- **Parameters:** WidgetHandle handle, const Vector2& position

- **Return:** `void`

---

## SetSize



- **Parameters:** WidgetHandle handle, const Vector2& size

- **Return:** `void`

---

## SetScale



- **Parameters:** WidgetHandle handle, const Vector2& scale

- **Return:** `void`

---

## SetRotation



- **Parameters:** WidgetHandle handle, float rotation

- **Return:** `void`

---

## SetVisible



- **Parameters:** WidgetHandle handle, bool visible

- **Return:** `void`

---

## SetOpacity



- **Parameters:** WidgetHandle handle, float opacity

- **Return:** `void`

---

## SetStyle



- **Parameters:** WidgetHandle handle, const std::string& style

- **Return:** `void`

---

## SetStyleSheet



- **Parameters:** const std::string& path

- **Return:** `void`

---

## RegisterEventHandler



- **Parameters:** WidgetHandle handle, const std::string& event, const EventHandler& handler

- **Return:** `void`

---

## UnregisterEventHandler



- **Parameters:** WidgetHandle handle, const std::string& event

- **Return:** `void`

---

## TriggerEvent



- **Parameters:** WidgetHandle handle, const UIEvent& event

- **Return:** `void`

---

## CreateAnimation



- **Parameters:** const AnimationDesc& desc

- **Return:** `AnimationHandle`

---

## DestroyAnimation



- **Parameters:** AnimationHandle handle

- **Return:** `void`

---

## PlayAnimation



- **Parameters:** WidgetHandle widget, AnimationHandle animation

- **Return:** `void`

---

## StopAnimation



- **Parameters:** WidgetHandle widget, AnimationHandle animation

- **Return:** `void`

---

## InvalidateLayout



- **Parameters:** WidgetHandle handle

- **Return:** `void`

---

## UpdateLayout



- **Parameters:** 

- **Return:** `void`

---

## ClearLayoutCache



- **Parameters:** 

- **Return:** `void`

---
