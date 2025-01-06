# Classes
---

## ThreadLocalCache
---




# Variables
---

### `CACHE_BUCKETS`

- **Type:** `static constexpr size_t`

- **Default Value:** `32`



### `BUCKET_SIZE`

- **Type:** `static constexpr size_t`

- **Default Value:** `256`



### `blockSize`

- **Type:** `size_t`



### `current`

- **Type:** `thread_local static ThreadLocalCache*`



### `maxThreads`

- **Type:** `size_t`

- **Default Value:** `64`



### `cacheSize`

- **Type:** `size_t`

- **Default Value:** `1024 * 1024`



### `enableThreadCache`

- **Type:** `bool`

- **Default Value:** `true`



### `config`

- **Type:** `const Config&`

- **Default Value:** `{})`



### `override`

- **Type:** `const`



### `CENTRAL_BUFFER_SIZE`

- **Type:** `static constexpr size_t`

- **Default Value:** `1024 * 1024 * 64`



### `m_Config`

- **Type:** `Config`




# Structs
---

### `Bucket`

- **Description:** 

- **Members:**



### `Config`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 




# Functions
---

## ConcurrentAllocator



- **Parameters:** const Config& config = {}

- **Return:** `explicit`

---

## allocateFromCache



- **Parameters:** size_t size

- **Return:** `void*`

---

## allocateFromCentral



- **Parameters:** size_t size

- **Return:** `void*`

---

## returnToCache



- **Parameters:** void* ptr, size_t size

- **Return:** `bool`

---
