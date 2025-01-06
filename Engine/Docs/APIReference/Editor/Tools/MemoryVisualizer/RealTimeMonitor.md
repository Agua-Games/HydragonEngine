
# Variables
---

### `trackStreaming`

- **Type:** `bool`

- **Default Value:** `true`



### `trackFragmentation`

- **Type:** `bool`

- **Default Value:** `true`



### `updateIntervalMs`

- **Type:** `int`

- **Default Value:** `1000`



### `maxEventsStored`

- **Type:** `size_t`

- **Default Value:** `1000`



### `config`

- **Type:** `const MonitoringConfig&`

- **Default Value:** `MonitoringConfig())`



### `m_Config`

- **Type:** `MonitoringConfig`




# Structs
---

### `MonitoringConfig`

- **Description:** 

- **Members:**




# Functions
---

## stopMonitoring



- **Parameters:** 

- **Return:** `void`

---

## memoryStateUpdated



- **Parameters:** const MemoryState& state

- **Return:** `void`

---

## streamingStateUpdated



- **Parameters:** const StreamingState& state

- **Return:** `void`

---

## fragmentationUpdated



- **Parameters:** const FragmentationInfo& info

- **Return:** `void`

---

## anomalyDetected



- **Parameters:** const MemoryAnomaly& anomaly

- **Return:** `void`

---

## updateMemoryState



- **Parameters:** 

- **Return:** `void`

---

## checkForAnomalies



- **Parameters:** 

- **Return:** `void`

---
