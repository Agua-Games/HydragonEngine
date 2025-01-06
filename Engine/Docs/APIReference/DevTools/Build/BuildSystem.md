# Classes
---

## BuildSystem
---




# Variables
---

### `enableIncrementalBuild`

- **Type:** `bool`

- **Default Value:** `true`



### `enableParallelBuild`

- **Type:** `bool`

- **Default Value:** `true`



### `enableCacheSystem`

- **Type:** `bool`

- **Default Value:** `true`



### `enableDependencyTracking`

- **Type:** `bool`

- **Default Value:** `true`



### `maxConcurrentJobs`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `intermediateDir`

- **Type:** `path`

- **Default Value:** `"Intermediate"`



### `outputDir`

- **Type:** `path`

- **Default Value:** `"Build"`



### `config`

- **Type:** `const BuildConfig&`

- **Default Value:** `{})`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `BuildConfig`



### `m_Stats`

- **Type:** `BuildStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `BuildConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static BuildSystem&`

---

## Initialize



- **Parameters:** const BuildConfig& config = {}

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## CreateBuild



- **Parameters:** const BuildDesc& desc

- **Return:** `BuildHandle`

---

## DestroyBuild



- **Parameters:** BuildHandle handle

- **Return:** `void`

---

## AddTarget



- **Parameters:** BuildHandle build, const BuildTarget& target

- **Return:** `void`

---

## RemoveTarget



- **Parameters:** BuildHandle build, const std::string& targetName

- **Return:** `void`

---

## SetBuildEnvironment



- **Parameters:** BuildHandle build, const BuildEnvironment& env

- **Return:** `void`

---

## AddDependency



- **Parameters:** BuildHandle build, const std::string& target, 
                      const std::string& dependency

- **Return:** `void`

---

## BuildTarget



- **Parameters:** BuildHandle build, const std::string& targetName

- **Return:** `bool`

---

## BuildAll



- **Parameters:** BuildHandle build

- **Return:** `bool`

---

## CancelBuild



- **Parameters:** BuildHandle build

- **Return:** `void`

---
