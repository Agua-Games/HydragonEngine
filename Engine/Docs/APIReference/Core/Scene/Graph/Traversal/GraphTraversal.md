# Classes
---

## TraversalOrder
---



## GraphTraversal
---




# Variables
---

### `order`

- **Type:** `TraversalOrder`

- **Default Value:** `TraversalOrder::PreOrder`



### `skipInvisible`

- **Type:** `bool`

- **Default Value:** `true`



### `skipCulled`

- **Type:** `bool`

- **Default Value:** `true`



### `maxDepth`

- **Type:** `uint32_t`

- **Default Value:** `UINT32_MAX`



### `VisitorFunc`

- **Type:** `using`

- **Default Value:** `std::function<void(NodeHandle)>`



### `options`

- **Type:** `const TraversalOptions&`

- **Default Value:** `{})`



### `options`

- **Type:** `const TraversalOptions&`

- **Default Value:** `{})`




# Structs
---

### `TraversalOptions`

- **Description:** 

- **Members:**




# Functions
---

## TraverseSubtree



- **Parameters:** NodeHandle root, const VisitorFunc& visitor, 
                               const TraversalOptions& options = {}

- **Return:** `static void`

---

## TraverseAncestors



- **Parameters:** NodeHandle node, const VisitorFunc& visitor

- **Return:** `static void`

---

## TraverseSiblings



- **Parameters:** NodeHandle node, const VisitorFunc& visitor

- **Return:** `static void`

---

## FindCommonAncestor



- **Parameters:** NodeHandle a, NodeHandle b

- **Return:** `static NodeHandle`

---

## GetDepth



- **Parameters:** NodeHandle node

- **Return:** `static uint32_t`

---

## TraversePreOrder



- **Parameters:** NodeHandle node, const VisitorFunc& visitor, 
                                const TraversalOptions& options, uint32_t depth

- **Return:** `static void`

---

## TraversePostOrder



- **Parameters:** NodeHandle node, const VisitorFunc& visitor,
                                 const TraversalOptions& options, uint32_t depth

- **Return:** `static void`

---

## TraverseBreadthFirst



- **Parameters:** NodeHandle root, const VisitorFunc& visitor,
                                    const TraversalOptions& options

- **Return:** `static void`

---
