# Classes
---

## BlockQuadTree
---




# Variables
---

### `bounds`

- **Type:** `QRectF`



### `occupied`

- **Type:** `bool`

- **Default Value:** `false`



### `blockName`

- **Type:** `QString`



### `utilizationRatio`

- **Type:** `double`



### `maxDepth`

- **Type:** `int`

- **Default Value:** `8)`



### `m_MaxDepth`

- **Type:** `int`




# Structs
---

### `Node`

- **Description:** 

- **Members:**

  - `bounds`: `QRectF` - 

  - `blockName`: `QString` - 

  - `utilizationRatio`: `double` - 




# Functions
---

## BlockQuadTree



- **Parameters:** const QRectF& bounds, int maxDepth = 8

- **Return:** `explicit`

---

## insert



- **Parameters:** const QRectF& rect

- **Return:** `Node*`

---

## remove



- **Parameters:** const QRectF& rect

- **Return:** `bool`

---

## findSpace



- **Parameters:** double size

- **Return:** `Node*`

---

## findSpaceInNode



- **Parameters:** Node* node, double size, int depth

- **Return:** `Node*`

---

## subdivide



- **Parameters:** Node* node

- **Return:** `bool`

---
