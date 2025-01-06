# Classes
---

## AgentType
---



## Type
---




# Variables
---

### `AgentHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `BehaviorHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `type`

- **Type:** `AgentType`



### `radius`

- **Type:** `float`

- **Default Value:** `0.5f`



### `height`

- **Type:** `float`

- **Default Value:** `2.0f`



### `maxSpeed`

- **Type:** `float`

- **Default Value:** `5.0f`



### `maxAcceleration`

- **Type:** `float`

- **Default Value:** `10.0f`



### `avoidance`

- **Type:** `bool`

- **Default Value:** `true`



### `behaviorPath`

- **Type:** `string`



### `type`

- **Type:** `Type`



### `position`

- **Type:** `Vector3`



### `intensity`

- **Type:** `float`



### `duration`

- **Type:** `float`



### `activeAgents`

- **Type:** `uint32_t`



### `activeBehaviors`

- **Type:** `uint32_t`



### `pathRequestsPerFrame`

- **Type:** `uint32_t`



### `averageUpdateTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`




# Structs
---

### `AgentDesc`

- **Description:** 

- **Members:**

  - `type`: `AgentType` - 

  - `behaviorPath`: `string` - 



### `PerceptionEvent`

- **Description:** 

- **Members:**



### `AIStats`

- **Description:** 

- **Members:**

  - `activeAgents`: `uint32_t` - 

  - `activeBehaviors`: `uint32_t` - 

  - `pathRequestsPerFrame`: `uint32_t` - 

  - `averageUpdateTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 


