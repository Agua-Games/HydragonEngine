# Classes
---

## RuntimeEventSystem
---




# Variables
---

### `enableEventBuffering`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePriorityQueue`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEventFiltering`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEventLogging`

- **Type:** `bool`

- **Default Value:** `true`



### `maxQueueSize`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxHandlersPerEvent`

- **Type:** `uint32_t`

- **Default Value:** `32`



### `bufferTimeMs`

- **Type:** `uint32_t`

- **Default Value:** `16`



### `config`

- **Type:** `const RuntimeEventConfig&`

- **Default Value:** `{})`



### `priority`

- **Type:** `EventPriority`

- **Default Value:** `EventPriority::Normal)`



### `data`

- **Type:** `const EventData&`

- **Default Value:** `{})`



### `data`

- **Type:** `const EventData&`

- **Default Value:** `{}, uint32_t delayMs = 0)`



### `event`

- **Type:** `string&`

- **Default Value:** `"")`



### `event`

- **Type:** `string&`

- **Default Value:** `"")`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeEventConfig`



### `m_Stats`

- **Type:** `RuntimeEventStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeEventConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static RuntimeEventSystem&`

---

## Initialize



- **Parameters:** `const RuntimeEventConfig& config = {}`

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

## RegisterHandler



- **Parameters:** `const std::string& event`, `const EventCallback& handler`, `EventPriority priority = EventPriority::Normal`

- **Return:** `void`

---

## UnregisterHandler



- **Parameters:** `const std::string& event`, `const EventCallback& handler`

- **Return:** `void`

---

## PostEvent



- **Parameters:** `const std::string& event`, `const EventData& data = {}`

- **Return:** `void`

---

## PostEventDeferred



- **Parameters:** `const std::string& event`, `const EventData& data = {}`, `uint32_t delayMs = 0`

- **Return:** `void`

---

## FlushEvents



- **Parameters:** `const std::string& event = ""`

- **Return:** `void`

---

## CancelDeferredEvents



- **Parameters:** `const std::string& event = ""`

- **Return:** `void`

---
