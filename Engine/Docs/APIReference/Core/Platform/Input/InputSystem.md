# Classes
---

## InputSystem
---




# Variables
---

### `enableRawInput`

- **Type:** `bool`

- **Default Value:** `true`



### `enableGamepad`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMultitouch`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHaptics`

- **Type:** `bool`

- **Default Value:** `true`



### `maxGamepads`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `maxTouchPoints`

- **Type:** `uint32_t`

- **Default Value:** `10`



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



- **Parameters:** ``

- **Return:** `static InputSystem&`

---

## Initialize



- **Parameters:** `const InputConfig& config = {}`

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

## SetMouseMode



- **Parameters:** `MouseMode mode`

- **Return:** `void`

---

## SetGamepadVibration



- **Parameters:** `uint32_t index`, `float leftMotor`, `float rightMotor`

- **Return:** `void`

---

## RegisterInputCallback



- **Parameters:** `const InputCallback& callback`

- **Return:** `void`

---

## UnregisterInputCallback



- **Parameters:** `InputCallbackId id`

- **Return:** `void`

---
