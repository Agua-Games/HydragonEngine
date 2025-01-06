# Classes
---

## NetworkMemorySecurity
---




# Variables
---

### `enableMemoryEncryption`

- **Type:** `bool`

- **Default Value:** `true`



### `validateRemotePointers`

- **Type:** `bool`

- **Default Value:** `true`



### `maxRemoteAllocationSize`

- **Type:** `uint32_t`

- **Default Value:** `1024 * 1024`



### `ValidationCallback`

- **Type:** `using`

- **Default Value:** `std::function<bool(const void*, size_t)>`



### `s_ValidationCallback`

- **Type:** `static ValidationCallback`



### `s_Config`

- **Type:** `static NetworkSecurityConfig`




# Structs
---

### `NetworkSecurityConfig`

- **Description:** 

- **Members:**

  - `1024`: `1024 *` - 




# Functions
---

## ValidateRemoteMemoryAccess



- **Parameters:** const void* ptr, size_t size

- **Return:** `static bool`

---

## EncryptMemoryForNetwork



- **Parameters:** void* data, size_t size, const uint8_t* key

- **Return:** `static bool`

---

## DecryptMemoryFromNetwork



- **Parameters:** void* data, size_t size, const uint8_t* key

- **Return:** `static bool`

---

## SetValidationCallback



- **Parameters:** ValidationCallback callback

- **Return:** `static void`

---
