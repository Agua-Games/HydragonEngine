# Classes
---

## DialogManagerSystem
---




# Variables
---

### `enableCustomDialogs`

- **Type:** `bool`

- **Default Value:** `true`



### `enableModalStack`

- **Type:** `bool`

- **Default Value:** `true`



### `enableAnimations`

- **Type:** `bool`

- **Default Value:** `true`



### `enableBackdrop`

- **Type:** `bool`

- **Default Value:** `true`



### `maxModalDepth`

- **Type:** `uint32_t`

- **Default Value:** `5`



### `fadeTime`

- **Type:** `float`

- **Default Value:** `0.2f`



### `defaultStyle`

- **Type:** `string`

- **Default Value:** `"Modern"`



### `config`

- **Type:** `const DialogManagerConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `DialogManagerConfig`



### `m_Stats`

- **Type:** `DialogManagerStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `DialogManagerConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static DialogManagerSystem&`

---

## Initialize



- **Parameters:** const DialogManagerConfig& config = {}

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

## ShowDialog



- **Parameters:** const std::string& type, const DialogDesc& desc

- **Return:** `DialogHandle`

---

## CloseDialog



- **Parameters:** DialogHandle handle

- **Return:** `void`

---

## CloseAllDialogs



- **Parameters:** 

- **Return:** `void`

---

## ShowModal



- **Parameters:** const std::string& type, const DialogDesc& desc

- **Return:** `DialogHandle`

---

## CloseModal



- **Parameters:** DialogHandle handle

- **Return:** `void`

---

## RegisterDialogType



- **Parameters:** const std::string& type, std::unique_ptr<IDialog> dialog

- **Return:** `void`

---

## UnregisterDialogType



- **Parameters:** const std::string& type

- **Return:** `void`

---
