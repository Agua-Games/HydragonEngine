# Classes
---

## TerrainVirtualTexturing
---




# Variables
---

### `pageSize`

- **Type:** `uint32_t`

- **Default Value:** `128`



### `feedbackBufferScale`

- **Type:** `uint32_t`

- **Default Value:** `4`



### `maxResidentPages`

- **Type:** `uint32_t`

- **Default Value:** `2048`



### `enableCompression`

- **Type:** `bool`

- **Default Value:** `true`



### `mipLevels`

- **Type:** `uint32_t`

- **Default Value:** `8`



### `pageId`

- **Type:** `uint32_t`



### `mipLevel`

- **Type:** `uint16_t`



### `flags`

- **Type:** `uint16_t`



### `priority`

- **Type:** `float`



### `pageId`

- **Type:** `uint32_t`



### `mipLevel`

- **Type:** `uint16_t`



### `priority`

- **Type:** `float`



### `m_Device`

- **Type:** `IRenderDevice*`



### `m_Config`

- **Type:** `VirtualTextureConfig`



### `m_VirtualMemory`

- **Type:** `VirtualMemoryManager`



### `m_FeedbackBuffer`

- **Type:** `TextureHandle`



### `m_PageTableTexture`

- **Type:** `TextureHandle`



### `m_PageRequestBuffer`

- **Type:** `BufferHandle`




# Structs
---

### `VirtualTextureConfig`

- **Description:** 

- **Members:**



### `PageTableEntry`

- **Description:** 

- **Members:**

  - `pageId`: `uint32_t` - 

  - `mipLevel`: `uint16_t` - 

  - `flags`: `uint16_t` - 

  - `priority`: `float` - 



### `PageRequest`

- **Description:** 

- **Members:**

  - `pageId`: `uint32_t` - 

  - `mipLevel`: `uint16_t` - 

  - `priority`: `float` - 




# Functions
---

## updateFeedback



- **Parameters:** `CommandBuffer* cmd`

- **Return:** `void`

---

## processPageRequests



- **Parameters:** ``

- **Return:** `void`

---

## streamPages



- **Parameters:** ``

- **Return:** `void`

---

## evictPages



- **Parameters:** `uint32_t count`

- **Return:** `void`

---

## drawPageTable



- **Parameters:** `CommandBuffer* cmd`

- **Return:** `void`

---

## drawFeedbackBuffer



- **Parameters:** `CommandBuffer* cmd`

- **Return:** `void`

---

## initializeResources



- **Parameters:** ``

- **Return:** `void`

---

## updatePageTable



- **Parameters:** ``

- **Return:** `void`

---

## loadPage



- **Parameters:** `const PageRequest& request`

- **Return:** `bool`

---
