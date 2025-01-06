
# Variables
---

### `bufferCount`

- **Type:** `uint32_t`

- **Default Value:** `3`



### `bufferSize`

- **Type:** `size_t`

- **Default Value:** `256 * 1024`



### `loop`

- **Type:** `bool`

- **Default Value:** `false`



### `priority`

- **Type:** `float`

- **Default Value:** `0.0f`



### `handle`

- **Type:** `SoundHandle`



### `params`

- **Type:** `StreamParams`



### `timestamp`

- **Type:** `uint64_t`



### `activeStreams`

- **Type:** `uint32_t`



### `pendingRequests`

- **Type:** `uint32_t`



### `bufferUnderruns`

- **Type:** `uint32_t`



### `averageLatencyMs`

- **Type:** `float`



### `totalBufferSize`

- **Type:** `size_t`



### `peakMemoryUsed`

- **Type:** `size_t`




# Structs
---

### `StreamParams`

- **Description:** 

- **Members:**

  - `1024`: `256 *` - 



### `StreamRequest`

- **Description:** 

- **Members:**

  - `handle`: `SoundHandle` - 

  - `params`: `StreamParams` - 

  - `timestamp`: `uint64_t` - 



### `StreamingStats`

- **Description:** 

- **Members:**

  - `activeStreams`: `uint32_t` - 

  - `pendingRequests`: `uint32_t` - 

  - `bufferUnderruns`: `uint32_t` - 

  - `averageLatencyMs`: `float` - 

  - `totalBufferSize`: `size_t` - 

  - `peakMemoryUsed`: `size_t` - 


