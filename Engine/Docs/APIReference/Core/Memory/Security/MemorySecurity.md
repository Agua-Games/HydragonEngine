# Classes
---

## MemorySecurity
---




# Variables
---

### `CANARY_SIZE`

- **Type:** `static constexpr size_t`

- **Default Value:** `8`



### `Canary`

- **Type:** `using`

- **Default Value:** `std::array<uint8_t, CANARY_SIZE>`




# Functions
---

## validateMemory



- **Parameters:** `void* ptr`, `size_t size`

- **Return:** `bool`

---

## enableFeature



- **Parameters:** `SecurityFeature feature`

- **Return:** `bool`

---

## onAllocation



- **Parameters:** `void* ptr`, `size_t size`

- **Return:** `void`

---

## onDeallocation



- **Parameters:** `void* ptr`

- **Return:** `void`

---
