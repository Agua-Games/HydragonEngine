# Classes
---

## LogLevel
---




# Variables
---

### `level`

- **Type:** `LogLevel`



### `message`

- **Type:** `string`



### `timestamp`

- **Type:** `time_point`



### `file`

- **Type:** `string`



### `line`

- **Type:** `uint32_t`



### `WatchCallback`

- **Type:** `using`

- **Default Value:** `std::function<std::string()>`



### `name`

- **Type:** `string`



### `callback`

- **Type:** `WatchCallback`



### `enabled`

- **Type:** `bool`



### `id`

- **Type:** `uint64_t`



### `name`

- **Type:** `string`



### `start`

- **Type:** `time_point`



### `activeDrawCalls`

- **Type:** `uint32_t`



### `totalLogEntries`

- **Type:** `uint32_t`



### `activeWatches`

- **Type:** `uint32_t`



### `activeProfilerScopes`

- **Type:** `uint32_t`



### `memoryUsed`

- **Type:** `size_t`




# Structs
---

### `LogEntry`

- **Description:** 

- **Members:**

  - `level`: `LogLevel` - 

  - `message`: `string` - 

  - `timestamp`: `time_point` - 

  - `file`: `string` - 

  - `line`: `uint32_t` - 



### `WatchEntry`

- **Description:** 

- **Members:**

  - `name`: `string` - 

  - `callback`: `WatchCallback` - 

  - `enabled`: `bool` - 



### `ProfilerScope`

- **Description:** 

- **Members:**

  - `id`: `uint64_t` - 

  - `name`: `string` - 

  - `start`: `time_point` - 



### `DebugStats`

- **Description:** 

- **Members:**

  - `activeDrawCalls`: `uint32_t` - 

  - `totalLogEntries`: `uint32_t` - 

  - `activeWatches`: `uint32_t` - 

  - `activeProfilerScopes`: `uint32_t` - 

  - `memoryUsed`: `size_t` - 


