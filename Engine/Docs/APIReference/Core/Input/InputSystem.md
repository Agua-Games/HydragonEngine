# Classes
---

## InputSystem
---




# Variables
---

### `maxDevices`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `maxBindings`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxActions`

- **Type:** `uint32_t`

- **Default Value:** `500`



### `enableRawInput`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHaptics`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHotPlugging`

- **Type:** `bool`

- **Default Value:** `true`



### `deadZone`

- **Type:** `float`

- **Default Value:** `0.15f`



### `config`

- **Type:** `const InputConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `InputConfig`



### `m_Stats`

- **Type:** `InputStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `InputConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static InputSystem&`

---

## Initialize



- **Parameters:** const InputConfig& config = {}

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

## RegisterDevice



- **Parameters:** const DeviceDesc& desc

- **Return:** `void`

---

## UnregisterDevice



- **Parameters:** DeviceHandle handle

- **Return:** `void`

---

## CreateAction



- **Parameters:** const ActionDesc& desc

- **Return:** `ActionHandle`

---

## DestroyAction



- **Parameters:** ActionHandle handle

- **Return:** `void`

---

## BindAction



- **Parameters:** ActionHandle action, const ActionBinding& binding

- **Return:** `void`

---

## UnbindAction



- **Parameters:** ActionHandle action, const ActionBinding& binding

- **Return:** `void`

---

## AddActionCallback



- **Parameters:** ActionHandle action, const ActionCallback& callback

- **Return:** `void`

---

## RemoveActionCallback



- **Parameters:** ActionHandle action, const ActionCallback& callback

- **Return:** `void`

---

## SetHapticFeedback



- **Parameters:** DeviceHandle device, const HapticEffect& effect

- **Return:** `void`

---
