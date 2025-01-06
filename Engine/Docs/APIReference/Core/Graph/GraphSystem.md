# Classes
---

## GraphSystem
---




# Variables
---

### `maxGraphs`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxNodesPerGraph`

- **Type:** `uint32_t`

- **Default Value:** `10000`



### `maxEdgesPerNode`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `enableParallelTraversal`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `enableOptimization`

- **Type:** `bool`

- **Default Value:** `true`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `128 * 1024 * 1024`



### `config`

- **Type:** `const GraphConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `GraphConfig`



### `m_Stats`

- **Type:** `GraphStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `GraphConfig`

- **Description:** 

- **Members:**

  - `1024`: `128 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static GraphSystem&`

---

## Initialize



- **Parameters:** `const GraphConfig& config = {}`

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

## CreateGraph



- **Parameters:** `const GraphDesc& desc`

- **Return:** `GraphHandle`

---

## DestroyGraph



- **Parameters:** `GraphHandle handle`

- **Return:** `void`

---

## AddNode



- **Parameters:** `GraphHandle graph`, `const NodeDesc& desc`

- **Return:** `NodeHandle`

---

## RemoveNode



- **Parameters:** `GraphHandle graph`, `NodeHandle node`

- **Return:** `void`

---

## AddEdge



- **Parameters:** `GraphHandle graph`, `NodeHandle from`, `NodeHandle to`, `const EdgeDesc& desc`

- **Return:** `EdgeHandle`

---

## RemoveEdge



- **Parameters:** `GraphHandle graph`, `EdgeHandle edge`

- **Return:** `void`

---

## TraverseGraph



- **Parameters:** `GraphHandle graph`, `const TraversalParams& params`

- **Return:** `void`

---

## OptimizeGraph



- **Parameters:** `GraphHandle graph`, `const OptimizationParams& params`

- **Return:** `void`

---
