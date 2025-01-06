# Classes
---

## VisualizationMode
---




# Variables
---

### `baseColor`

- **Type:** `QColor`



### `opacity`

- **Type:** `float`



### `visible`

- **Type:** `bool`



### `mode`

- **Type:** `VisualizationMode`

- **Default Value:** `VisualizationMode::AllocationFrequency`



### `showMultiLayers`

- **Type:** `bool`

- **Default Value:** `false`



### `updateInterval`

- **Type:** `float`

- **Default Value:** `0.5f`



### `decayRate`

- **Type:** `float`

- **Default Value:** `0.95f`



### `gridResolution`

- **Type:** `size_t`

- **Default Value:** `64`



### `adaptiveColoring`

- **Type:** `bool`

- **Default Value:** `true`



### `m_Config`

- **Type:** `ViewConfig`




# Structs
---

### `HeatmapLayer`

- **Description:** 

- **Members:**

  - `baseColor`: `QColor` - 

  - `opacity`: `float` - 

  - `visible`: `bool` - 



### `ViewConfig`

- **Description:** 

- **Members:**




# Functions
---

## setMode



- **Parameters:** VisualizationMode mode

- **Return:** `void`

---

## toggleLayer



- **Parameters:** const QString& layerName, bool visible

- **Return:** `void`

---

## setViewConfig



- **Parameters:** const ViewConfig& config

- **Return:** `void`

---

## zoomToRegion



- **Parameters:** const QRectF& region

- **Return:** `void`

---

## highlightPattern



- **Parameters:** const std::vector<void*>& addresses

- **Return:** `void`

---

## showTimelineAnimation



- **Parameters:** const std::vector<MemoryState>& states

- **Return:** `void`

---

## updateLayerIntensities



- **Parameters:** const MemoryState& state

- **Return:** `void`

---

## calculateFragmentationPattern



- **Parameters:** 

- **Return:** `void`

---

## updatePressureVisualization



- **Parameters:** 

- **Return:** `void`

---

## calculateBlendedColor



- **Parameters:** const std::vector<HeatmapLayer*>& activeLayers, 
                               size_t x, size_t y

- **Return:** `QColor`

---
