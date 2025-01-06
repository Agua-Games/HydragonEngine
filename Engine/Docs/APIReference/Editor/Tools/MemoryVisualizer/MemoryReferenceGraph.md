# Classes
---

## LayoutMode
---




# Variables
---

### `block`

- **Type:** `MemoryBlock*`



### `visual`

- **Type:** `QGraphicsItem*`



### `referenceStrength`

- **Type:** `float`



### `m_CurrentLayout`

- **Type:** `LayoutMode`




# Structs
---

### `ReferenceNode`

- **Description:** 

- **Members:**

  - `block`: `MemoryBlock*` - 

  - `visual`: `QGraphicsItem*` - 

  - `referenceStrength`: `float` - 




# Functions
---

## setLayoutMode



- **Parameters:** LayoutMode mode

- **Return:** `void`

---

## updateReferences



- **Parameters:** const std::vector<Memory::MemoryBlock>& blocks

- **Return:** `void`

---

## findStrongReferences



- **Parameters:** 

- **Return:** `void`

---

## detectCycles



- **Parameters:** 

- **Return:** `void`

---

## highlightGroup



- **Parameters:** const ReferenceNode* node

- **Return:** `void`

---

## calculateLayout



- **Parameters:** 

- **Return:** `void`

---

## drawConnections



- **Parameters:** 

- **Return:** `void`

---

## updateNodeVisuals



- **Parameters:** 

- **Return:** `void`

---
