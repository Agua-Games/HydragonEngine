# Classes
---

## TooltipManagerSystem
---




# Variables
---

### `enableRichTooltips`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDelayedShow`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAnimation`

- **Type:** `bool`

- **Default Value:** `true`



### `enableKeyboardTips`

- **Type:** `bool`

- **Default Value:** `true`



### `showDelay`

- **Type:** `float`

- **Default Value:** `0.5f`



### `hideDelay`

- **Type:** `float`

- **Default Value:** `0.1f`



### `offset`

- **Type:** `Vector2`

- **Default Value:** `{10.0f, 10.0f}`



### `styleSheet`

- **Type:** `string`

- **Default Value:** `"Editor/Styles/Tooltips.css"`



### `config`

- **Type:** `const TooltipManagerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `TooltipManagerConfig`



### `m_Stats`

- **Type:** `TooltipManagerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `TooltipManagerConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static TooltipManagerSystem&`

---

## Initialize



- **Parameters:** `const TooltipManagerConfig& config = {}`

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

## RegisterTooltip



- **Parameters:** `const std::string& id`, `const TooltipDesc& desc`

- **Return:** `void`

---

## UnregisterTooltip



- **Parameters:** `const std::string& id`

- **Return:** `void`

---

## ShowTooltip



- **Parameters:** `const std::string& id`, `const Vector2& position`

- **Return:** `void`

---

## HideTooltip



- **Parameters:** `const std::string& id`

- **Return:** `void`

---

## HideAllTooltips



- **Parameters:** ``

- **Return:** `void`

---

## SetTooltipContent



- **Parameters:** `const std::string& id`, `const std::string& content`

- **Return:** `void`

---

## SetTooltipStyle



- **Parameters:** `const std::string& id`, `const TooltipStyle& style`

- **Return:** `void`

---
