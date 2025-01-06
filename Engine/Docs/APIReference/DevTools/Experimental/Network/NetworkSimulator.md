# Classes
---

## NetworkSimulator
---




# Variables
---

### `enableLatencySimulation`

- **Type:** `bool`

- **Default Value:** `true`



### `enablePacketLoss`

- **Type:** `bool`

- **Default Value:** `true`



### `enableBandwidthLimiting`

- **Type:** `bool`

- **Default Value:** `true`



### `enableJitterSimulation`

- **Type:** `bool`

- **Default Value:** `true`



### `baseLatency`

- **Type:** `float`

- **Default Value:** `50.0f`



### `packetLossRate`

- **Type:** `float`

- **Default Value:** `0.01f`



### `bandwidthLimit`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024`



### `config`

- **Type:** `const NetworkSimConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `NetworkSimConfig`



### `m_Stats`

- **Type:** `NetworkSimStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `NetworkSimConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static NetworkSimulator&`

---

## Initialize



- **Parameters:** const NetworkSimConfig& config = {}

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

## SetLatencyProfile



- **Parameters:** const LatencyProfile& profile

- **Return:** `void`

---

## SetPacketLossProfile



- **Parameters:** const PacketLossProfile& profile

- **Return:** `void`

---

## SetBandwidthProfile



- **Parameters:** const BandwidthProfile& profile

- **Return:** `void`

---

## StartCapture



- **Parameters:** const std::string& name

- **Return:** `void`

---

## StopCapture



- **Parameters:** 

- **Return:** `void`

---

## AnalyzeCapture



- **Parameters:** const std::string& name

- **Return:** `void`

---
