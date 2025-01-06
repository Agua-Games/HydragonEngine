
# Variables
---

### `parent`

- **Type:** `QWidget*`

- **Default Value:** `nullptr)`



### `m_MemoryUsageChart`

- **Type:** `QChartView*`



### `m_AllocationDistributionChart`

- **Type:** `QChartView*`



### `totalUsed`

- **Type:** `size_t`



### `peakUsage`

- **Type:** `size_t`




# Structs
---

### `MemorySnapshot`

- **Description:** 

- **Members:**

  - `totalUsed`: `size_t` - 

  - `peakUsage`: `size_t` - 




# Functions
---

## MemoryVisualizerWidget



- **Parameters:** `QWidget* parent = nullptr`

- **Return:** `explicit`

---

## updateMemoryView



- **Parameters:** ``

- **Return:** `void`

---

## onAllocationSelected



- **Parameters:** `const QPointF& point`

- **Return:** `void`

---

## setupCharts



- **Parameters:** ``

- **Return:** `void`

---

## createMemoryMap



- **Parameters:** ``

- **Return:** `void`

---

## updateAllocationDetails



- **Parameters:** `const Memory::MemoryBlock& block`

- **Return:** `void`

---
