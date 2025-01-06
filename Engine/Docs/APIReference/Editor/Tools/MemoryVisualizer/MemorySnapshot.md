# Classes
---

## MemorySnapshot
---




# Variables
---

### `currentThroughput`

- **Type:** `double`



### `m_CaptureTime`

- **Type:** `time_point`



### `m_StreamingState`

- **Type:** `StreamingState`




# Structs
---

### `StreamingState`

- **Description:** 

- **Members:**

  - `currentThroughput`: `double` - 




# Functions
---

## capture



- **Parameters:** 

- **Return:** `void`

---

## save



- **Parameters:** const std::string& filename

- **Return:** `void`

---

## load



- **Parameters:** const std::string& filename

- **Return:** `bool`

---

## deserializeFromJson



- **Parameters:** const QJsonObject& json

- **Return:** `void`

---
