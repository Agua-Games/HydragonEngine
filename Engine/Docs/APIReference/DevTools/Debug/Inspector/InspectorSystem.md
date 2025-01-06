# Classes
---

## InspectorSystem
---




# Variables
---

### `enableLiveEdit`

- **Type:** `bool`

- **Default Value:** `true`



### `enableTypeInfo`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCallstack`

- **Type:** `bool`

- **Default Value:** `true`



### `enableMemoryView`

- **Type:** `bool`

- **Default Value:** `true`



### `enableHistoryTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `maxHistoryEntries`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxWatchVariables`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `config`

- **Type:** `const InspectorConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `InspectorConfig`



### `m_Stats`

- **Type:** `InspectorStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `InspectorConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static InspectorSystem&`

---

## Initialize



- **Parameters:** const InspectorConfig& config = {}

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

## AddWatch



- **Parameters:** const std::string& name, const WatchCallback& callback

- **Return:** `void`

---

## RemoveWatch



- **Parameters:** const std::string& name

- **Return:** `void`

---

## AddBreakpoint



- **Parameters:** const std::string& location

- **Return:** `void`

---

## RemoveBreakpoint



- **Parameters:** const std::string& location

- **Return:** `void`

---

## InspectObject



- **Parameters:** const void* object, const TypeInfo& typeInfo

- **Return:** `void`

---

## ModifyVariable



- **Parameters:** const std::string& name, const Variable& value

- **Return:** `void`

---

## CaptureCallstack



- **Parameters:** 

- **Return:** `void`

---

## CaptureMemorySnapshot



- **Parameters:** 

- **Return:** `void`

---
