# Classes
---

## GraphType
---



## TraversalType
---




# Variables
---

### `GraphHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `NodeHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `EdgeHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `name`

- **Type:** `string`



### `type`

- **Type:** `GraphType`



### `persistent`

- **Type:** `bool`

- **Default Value:** `false`



### `allowCycles`

- **Type:** `bool`

- **Default Value:** `true`



### `expectedSize`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `name`

- **Type:** `string`



### `weight`

- **Type:** `uint32_t`

- **Default Value:** `1`



### `active`

- **Type:** `bool`

- **Default Value:** `true`



### `weight`

- **Type:** `float`

- **Default Value:** `1.0f`



### `bidirectional`

- **Type:** `bool`

- **Default Value:** `false`



### `active`

- **Type:** `bool`

- **Default Value:** `true`



### `NodeVisitor`

- **Type:** `using`

- **Default Value:** `std::function<void(NodeHandle, const NodeDesc&)>`



### `EdgeVisitor`

- **Type:** `using`

- **Default Value:** `std::function<void(EdgeHandle, const EdgeDesc&)>`



### `type`

- **Type:** `TraversalType`



### `startNode`

- **Type:** `NodeHandle`



### `nodeVisitor`

- **Type:** `NodeVisitor`



### `edgeVisitor`

- **Type:** `EdgeVisitor`



### `parallel`

- **Type:** `bool`

- **Default Value:** `false`



### `minimizeEdges`

- **Type:** `bool`

- **Default Value:** `true`



### `balanceNodes`

- **Type:** `bool`

- **Default Value:** `true`



### `removeIsolatedNodes`

- **Type:** `bool`

- **Default Value:** `true`



### `weightThreshold`

- **Type:** `float`

- **Default Value:** `0.1f`



### `totalGraphs`

- **Type:** `uint32_t`



### `totalNodes`

- **Type:** `uint32_t`



### `totalEdges`

- **Type:** `uint32_t`



### `traversalCount`

- **Type:** `uint32_t`



### `averageTraversalTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`




# Structs
---

### `GraphDesc`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `type`: `GraphType` - 



### `NodeDesc`

- **Description:** 

- **Members:**

  - `name`: `string` - 



### `EdgeDesc`

- **Description:** 

- **Members:**



### `TraversalParams`

- **Description:** 

- **Members:**

  - `type`: `TraversalType` - 

  - `startNode`: `NodeHandle` - 

  - `nodeVisitor`: `NodeVisitor` - 

  - `edgeVisitor`: `EdgeVisitor` - 



### `OptimizationParams`

- **Description:** 

- **Members:**



### `GraphStats`

- **Description:** 

- **Members:**

  - `totalGraphs`: `uint32_t` - 

  - `totalNodes`: `uint32_t` - 

  - `totalEdges`: `uint32_t` - 

  - `traversalCount`: `uint32_t` - 

  - `averageTraversalTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 


