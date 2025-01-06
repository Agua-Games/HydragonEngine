# Classes
---

## GraphAlgorithms
---




# Variables
---

### `enableParallel`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `maxIterations`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `convergenceThreshold`

- **Type:** `float`

- **Default Value:** `0.001f`



### `config`

- **Type:** `const AlgorithmConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `AlgorithmConfig`



### `m_Stats`

- **Type:** `AlgorithmStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `AlgorithmConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static GraphAlgorithms&`

---

## Initialize



- **Parameters:** `const AlgorithmConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## HasCycle



- **Parameters:** `GraphHandle graph`

- **Return:** `bool`

---

## GetConnectedComponents



- **Parameters:** `GraphHandle graph`

- **Return:** `uint32_t`

---

## ComputeGraphDensity



- **Parameters:** `GraphHandle graph`

- **Return:** `float`

---

## OptimizeLayout



- **Parameters:** `GraphHandle graph`, `const LayoutParams& params`

- **Return:** `void`

---

## ReduceEdges



- **Parameters:** `GraphHandle graph`, `float threshold`

- **Return:** `void`

---
