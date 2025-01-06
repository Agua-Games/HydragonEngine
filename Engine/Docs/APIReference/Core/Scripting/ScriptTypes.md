
# Variables
---

### `ScriptHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `CoroutineHandle`

- **Type:** `using`

- **Default Value:** `uint32_t`



### `ScriptValue`

- **Type:** `using`

- **Default Value:** `std::variant<
    bool,
    int32_t,
    float,
    std::string,
    std::vector<ScriptValue>
>`



### `path`

- **Type:** `string`



### `autoStart`

- **Type:** `bool`

- **Default Value:** `false`



### `persistent`

- **Type:** `bool`

- **Default Value:** `false`



### `activeScripts`

- **Type:** `uint32_t`



### `activeCoroutines`

- **Type:** `uint32_t`



### `totalExecutions`

- **Type:** `uint32_t`



### `averageExecutionTimeMs`

- **Type:** `float`



### `memoryUsed`

- **Type:** `size_t`



### `gcCollections`

- **Type:** `uint32_t`




# Structs
---

### `ScriptDesc`

- **Description:** 

- **Members:**

  - `path`: `string` - 



### `ScriptStats`

- **Description:** 

- **Members:**

  - `activeScripts`: `uint32_t` - 

  - `activeCoroutines`: `uint32_t` - 

  - `totalExecutions`: `uint32_t` - 

  - `averageExecutionTimeMs`: `float` - 

  - `memoryUsed`: `size_t` - 

  - `gcCollections`: `uint32_t` - 


