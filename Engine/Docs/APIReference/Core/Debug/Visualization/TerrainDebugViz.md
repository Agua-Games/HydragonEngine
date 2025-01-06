
# Variables
---

### `memoryStats`

- **Type:** `MemoryStats`



### `m_state`

- **Type:** `VisualizationState`



### `m_renderer`

- **Type:** `DebugRenderer`



### `bounds`

- **Type:** `auto`

- **Default Value:** `GetMeshletBounds(handle)`



### `density`

- **Type:** `float`

- **Default Value:** `CalculateVertexDensity(handle)`



### `heatmapColor`

- **Type:** `Color`

- **Default Value:** `MapDensityToColor(density)`



### `mesh`

- **Type:** `auto`

- **Default Value:** `GetMeshletGeometry(handle)`



### `ss`

- **Type:** `stringstream`



### `minDensity`

- **Type:** `const float`

- **Default Value:** `0.0f`



### `maxDensity`

- **Type:** `const float`

- **Default Value:** `1.0f`



### `t`

- **Type:** `float`

- **Default Value:** `(density - minDensity) / (maxDensity - minDensity)`




# Structs
---

### `VisualizationState`

- **Description:** 

- **Members:**

  - `memoryStats`: `MemoryStats` - 


