# Classes
---

## RuntimeInventorySystem
---




# Variables
---

### `enableStacking`

- **Type:** `bool`

- **Default Value:** `true`



### `enableWeightLimits`

- **Type:** `bool`

- **Default Value:** `true`



### `enableEquipmentSlots`

- **Type:** `bool`

- **Default Value:** `true`



### `enableItemCategories`

- **Type:** `bool`

- **Default Value:** `true`



### `maxInventories`

- **Type:** `uint32_t`

- **Default Value:** `1000`



### `maxSlotsPerInventory`

- **Type:** `uint32_t`

- **Default Value:** `100`



### `maxStackSize`

- **Type:** `uint32_t`

- **Default Value:** `999`



### `defaultWeightCapacity`

- **Type:** `float`

- **Default Value:** `100.0f`



### `itemDatabasePath`

- **Type:** `string`

- **Default Value:** `"Config/Items"`



### `config`

- **Type:** `const RuntimeInventoryConfig&`

- **Default Value:** `{})`



### `count`

- **Type:** `uint32_t`

- **Default Value:** `1)`



### `count`

- **Type:** `uint32_t`

- **Default Value:** `1)`



### `count`

- **Type:** `uint32_t`

- **Default Value:** `1)`



### `count`

- **Type:** `uint32_t`

- **Default Value:** `1) const`



### `m_Stats`

- **Type:** `return`



### `m_Config`

- **Type:** `RuntimeInventoryConfig`



### `m_Stats`

- **Type:** `RuntimeInventoryStats`



### `m_Initialized`

- **Type:** `bool`

- **Default Value:** `false`




# Structs
---

### `RuntimeInventoryConfig`

- **Description:** 

- **Members:**




# Functions
---

## Get



- **Parameters:** ``

- **Return:** `static RuntimeInventorySystem&`

---

## Initialize



- **Parameters:** `const RuntimeInventoryConfig& config = {}`

- **Return:** `void`

---

## Shutdown



- **Parameters:** ``

- **Return:** `void`

---

## Update



- **Parameters:** ``

- **Return:** `void`

---

## CreateInventory



- **Parameters:** `const InventoryDesc& desc`

- **Return:** `InventoryHandle`

---

## DestroyInventory



- **Parameters:** `InventoryHandle handle`

- **Return:** `void`

---

## AddItem



- **Parameters:** `InventoryHandle handle`, `const ItemDesc& item`, `uint32_t count = 1`

- **Return:** `bool`

---

## RemoveItem



- **Parameters:** `InventoryHandle handle`, `ItemId itemId`, `uint32_t count = 1`

- **Return:** `bool`

---

## TransferItem



- **Parameters:** `InventoryHandle from`, `InventoryHandle to`, `ItemId itemId`, `uint32_t count = 1`

- **Return:** `bool`

---

## EquipItem



- **Parameters:** `InventoryHandle handle`, `ItemId itemId`, `const std::string& slot`

- **Return:** `bool`

---

## UnequipItem



- **Parameters:** `InventoryHandle handle`, `const std::string& slot`

- **Return:** `bool`

---

## SwapItems



- **Parameters:** `InventoryHandle handle`, `uint32_t slotA`, `uint32_t slotB`

- **Return:** `bool`

---

## SetWeightCapacity



- **Parameters:** `InventoryHandle handle`, `float capacity`

- **Return:** `void`

---

## SetSlotCount



- **Parameters:** `InventoryHandle handle`, `uint32_t count`

- **Return:** `void`

---

## SetStackSize



- **Parameters:** `ItemId itemId`, `uint32_t maxStack`

- **Return:** `void`

---

## RegisterItemProcessor



- **Parameters:** `const std::string& type`, `std::unique_ptr<IItemProcessor> processor`

- **Return:** `void`

---

## UnregisterItemProcessor



- **Parameters:** `const std::string& type`

- **Return:** `void`

---

## RegisterInventoryCallback



- **Parameters:** `InventoryEventType type`, `const InventoryCallback& callback`

- **Return:** `void`

---

## UnregisterInventoryCallback



- **Parameters:** `InventoryEventType type`

- **Return:** `void`

---
