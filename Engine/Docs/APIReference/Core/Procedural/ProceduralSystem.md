# Classes
---

## ProceduralSystem
---




# Variables
---

### `enableMultithreading`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCaching`

- **Type:** `bool`

- **Default Value:** `true`



### `enableProgressiveGeneration`

- **Type:** `bool`

- **Default Value:** `true`



### `maxGenerators`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxConcurrentJobs`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `memoryBudget`

- **Type:** `size_t`

- **Default Value:** `256 * 1024 * 1024`



### `seedOffset`

- **Type:** `uint32_t`

- **Default Value:** `0`



### `config`

- **Type:** `const ProceduralConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `ProceduralConfig`



### `m_Stats`

- **Type:** `ProceduralStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `ProceduralConfig`

- **Description:** 

- **Members:**

  - `1024`: `256 * 1024 *` - 




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static ProceduralSystem&`

---

## Initialize



- **Parameters:** const ProceduralConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## Update



- **Parameters:** 

- **Return:** `void`

---

## CreateGenerator



- **Parameters:** const GeneratorDesc& desc

- **Return:** `GeneratorHandle`

---

## DestroyGenerator



- **Parameters:** GeneratorHandle handle

- **Return:** `void`

---

## SetSeed



- **Parameters:** GeneratorHandle handle, uint64_t seed

- **Return:** `void`

---

## SetParameters



- **Parameters:** GeneratorHandle handle, const GeneratorParams& params

- **Return:** `void`

---

## Generate



- **Parameters:** GeneratorHandle handle, const GenerationContext& context

- **Return:** `void`

---

## CancelGeneration



- **Parameters:** GeneratorHandle handle

- **Return:** `void`

---

## RegisterCallback



- **Parameters:** GeneratorHandle handle, const GenerationCallback& callback

- **Return:** `void`

---

## UnregisterCallback



- **Parameters:** GeneratorHandle handle

- **Return:** `void`

---
