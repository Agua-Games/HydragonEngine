# Classes
---

## InputBindings
---




# Variables
---

### `enableRawInput`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHapticFeedback`

- **Type:** `bool`

- **Default Value:** `true`



### `enableGestureRecognition`

- **Type:** `bool`

- **Default Value:** `true`



### `enableInputRecording`

- **Type:** `bool`

- **Default Value:** `true`



### `maxInputCallbacks`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxGesturePatterns`

- **Type:** `uint32_t`

- **Default Value:** `50`



### `recordingBufferSize`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `inputMappingPath`

- **Type:** `string`

- **Default Value:** `"Config/Input"`



### `config`

- **Type:** `const InputBindingsConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `InputBindingsConfig`



### `m_Stats`

- **Type:** `InputBindingsStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `InputBindingsConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static InputBindings&`

---

## Initialize



- **Parameters:** `const InputBindingsConfig& config = {}`

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

## CreateAction



- **Parameters:** `const std::string& name`, `const ActionDesc& desc`

- **Return:** `ActionHandle`

---

## DestroyAction



- **Parameters:** `ActionHandle handle`

- **Return:** `void`

---

## BindAction



- **Parameters:** `ActionHandle handle`, `const ActionBinding& binding`

- **Return:** `void`

---

## UnbindAction



- **Parameters:** `ActionHandle handle`

- **Return:** `void`

---

## RegisterActionCallback



- **Parameters:** `const std::string& name`, `const ActionCallback& callback`

- **Return:** `void`

---

## UnregisterActionCallback



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## RegisterGesturePattern



- **Parameters:** `const std::string& name`, `const GesturePattern& pattern`

- **Return:** `void`

---

## UnregisterGesturePattern



- **Parameters:** `const std::string& name`

- **Return:** `void`

---

## StartInputRecording



- **Parameters:** ``

- **Return:** `void`

---

## StopInputRecording



- **Parameters:** ``

- **Return:** `void`

---

## PlaybackRecording



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## SaveRecording



- **Parameters:** `const std::string& path`

- **Return:** `void`

---

## TriggerHapticFeedback



- **Parameters:** `const std::string& device`, `const HapticEffect& effect`

- **Return:** `void`

---
