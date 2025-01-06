# Classes
---

## DependencyTracker
---




# Variables
---

### `m_Stats`

- **Type:** `return`



### `visited`

- **Type:** `bool`

- **Default Value:** `false`



### `inStack`

- **Type:** `bool`

- **Default Value:** `false`



### `m_Stats`

- **Type:** `DependencyStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `DependencyNode`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** 

- **Return:** `static DependencyTracker&`

---

## Initialize



- **Parameters:** 

- **Return:** `void`

---

## Shutdown



- **Parameters:** 

- **Return:** `void`

---

## AddDependency



- **Parameters:** const std::string& target, const std::string& dependency

- **Return:** `void`

---

## RemoveDependency



- **Parameters:** const std::string& target, const std::string& dependency

- **Return:** `void`

---

## Clear



- **Parameters:** 

- **Return:** `void`

---
