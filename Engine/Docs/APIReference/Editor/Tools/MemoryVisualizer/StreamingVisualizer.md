# Classes
---

## StreamingVisualizer
---



## VisualizationMode
---




# Variables
---

### `resourceName`

- **Type:** `QString`



### `state`

- **Type:** `ResourceState`



### `progress`

- **Type:** `float`



### `stateColor`

- **Type:** `QColor`



### `flowAnimation`

- **Type:** `QPropertyAnimation*`



### `stateTransition`

- **Type:** `QParallelAnimationGroup*`




# Structs
---

### `StreamingBlock`

- **Description:** 

- **Members:**

  - `resourceName`: `QString` - 

  - `state`: `ResourceState` - 

  - `progress`: `float` - 

  - `stateColor`: `QColor` - 

  - `flowAnimation`: `QPropertyAnimation*` - 

  - `stateTransition`: `QParallelAnimationGroup*` - 




# Functions
---

## animateResourceFlow



- **Parameters:** const StreamingBlock& block

- **Return:** `void`

---

## updateStreamingState



- **Parameters:** const StreamingBlock& block

- **Return:** `void`

---

## highlightActiveStreams



- **Parameters:** 

- **Return:** `void`

---
