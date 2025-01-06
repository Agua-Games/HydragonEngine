# Classes
---

## GeneratorType
---




# Variables
---

### `GeneratorHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `frequency`

- **Type:** `float`

- **Default Value:** `1.0f`



### `amplitude`

- **Type:** `float`

- **Default Value:** `1.0f`



### `octaves`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `persistence`

- **Type:** `float`

- **Default Value:** `0.5f`



### `lacunarity`

- **Type:** `float`

- **Default Value:** `2.0f`



### `offset`

- **Type:** `Vector3`

- **Default Value:** `{0.0f, 0.0f, 0.0f}`



### `noise`

- **Type:** `NoiseParams`



### `density`

- **Type:** `float`

- **Default Value:** `1.0f`



### `variation`

- **Type:** `float`

- **Default Value:** `0.5f`



### `detail`

- **Type:** `float`

- **Default Value:** `1.0f`



### `center`

- **Type:** `Vector3`



### `extent`

- **Type:** `Vector3`



### `resolution`

- **Type:** `uint32_t`



### `lod`

- **Type:** `uint32_t`



### `async`

- **Type:** `bool`

- **Default Value:** `true`



### `type`

- **Type:** `GeneratorType`



### `name`

- **Type:** `string`



### `params`

- **Type:** `GeneratorParams`



### `persistent`

- **Type:** `bool`

- **Default Value:** `false`



### `incremental`

- **Type:** `bool`

- **Default Value:** `false`



### `activeGenerators`

- **Type:** `uint32_t`



### `pendingJobs`

- **Type:** `uint32_t`



### `completedJobs`

- **Type:** `uint32_t`



### `failedJobs`

- **Type:** `uint32_t`



### `averageGenerationTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`



### `GenerationCallback`

- **Type:** `using`

- **Default Value:** `std::function<void(GeneratorHandle, bool success)>`




# Structs
---

### `NoiseParams`

- **Description:** 

- **Members:**



### `GeneratorParams`

- **Description:** 

- **Members:**

  - `noise`: `NoiseParams` - 



### `GenerationContext`

- **Description:** 

- **Members:**

  - `center`: `Vector3` - 

  - `extent`: `Vector3` - 

  - `resolution`: `uint32_t` - 

  - `lod`: `uint32_t` - 



### `GeneratorDesc`

- **Description:** 

- **Members:**

  - `type`: `GeneratorType` - 

  - `name`: `string` - 

  - `params`: `GeneratorParams` - 



### `ProceduralStats`

- **Description:** 

- **Members:**

  - `activeGenerators`: `uint32_t` - 

  - `pendingJobs`: `uint32_t` - 

  - `completedJobs`: `uint32_t` - 

  - `failedJobs`: `uint32_t` - 

  - `averageGenerationTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 


