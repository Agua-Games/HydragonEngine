
## Classes

### RuntimeInventorySystem




## Functions

### Get



**Parameters:** 

---

### Initialize



**Parameters:** const RuntimeInventoryConfig& config = {}

---

### Shutdown



**Parameters:** 

---

### Update



**Parameters:** 

---

### CreateInventory



**Parameters:** const InventoryDesc& desc

---

### DestroyInventory



**Parameters:** InventoryHandle handle

---

### AddItem



**Parameters:** InventoryHandle handle, const ItemDesc& item, uint32_t count = 1

---

### RemoveItem



**Parameters:** InventoryHandle handle, ItemId itemId, uint32_t count = 1

---

### TransferItem



**Parameters:** InventoryHandle from, InventoryHandle to, ItemId itemId, uint32_t count = 1

---

### EquipItem



**Parameters:** InventoryHandle handle, ItemId itemId, const std::string& slot

---

### UnequipItem



**Parameters:** InventoryHandle handle, const std::string& slot

---

### SwapItems



**Parameters:** InventoryHandle handle, uint32_t slotA, uint32_t slotB

---

### SetWeightCapacity



**Parameters:** InventoryHandle handle, float capacity

---

### SetSlotCount



**Parameters:** InventoryHandle handle, uint32_t count

---

### SetStackSize



**Parameters:** ItemId itemId, uint32_t maxStack

---

### RegisterItemProcessor



**Parameters:** const std::string& type, std::unique_ptr<IItemProcessor> processor

---

### UnregisterItemProcessor



**Parameters:** const std::string& type

---

### RegisterInventoryCallback



**Parameters:** InventoryEventType type, const InventoryCallback& callback

---

### UnregisterInventoryCallback



**Parameters:** InventoryEventType type

---
