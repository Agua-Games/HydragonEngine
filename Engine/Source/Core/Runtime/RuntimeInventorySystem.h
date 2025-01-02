/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Runtime inventory and item management
 */

#pragma once
#include "RuntimeTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Core {

struct RuntimeInventoryConfig {
    bool enableStacking = true;
    bool enableWeightLimits = true;
    bool enableEquipmentSlots = true;
    bool enableItemCategories = true;
    uint32_t maxInventories = 1000;
    uint32_t maxSlotsPerInventory = 100;
    uint32_t maxStackSize = 999;
    float defaultWeightCapacity = 100.0f;
    std::string itemDatabasePath = "Config/Items";
};

class RuntimeInventorySystem {
public:
    static RuntimeInventorySystem& Get();
    
    void Initialize(const RuntimeInventoryConfig& config = {});
    void Shutdown();
    
    void Update();
    
    InventoryHandle CreateInventory(const InventoryDesc& desc);
    void DestroyInventory(InventoryHandle handle);
    
    bool AddItem(InventoryHandle handle, const ItemDesc& item, uint32_t count = 1);
    bool RemoveItem(InventoryHandle handle, ItemId itemId, uint32_t count = 1);
    bool TransferItem(InventoryHandle from, InventoryHandle to, ItemId itemId, uint32_t count = 1);
    
    bool EquipItem(InventoryHandle handle, ItemId itemId, const std::string& slot);
    bool UnequipItem(InventoryHandle handle, const std::string& slot);
    bool SwapItems(InventoryHandle handle, uint32_t slotA, uint32_t slotB);
    
    void SetWeightCapacity(InventoryHandle handle, float capacity);
    void SetSlotCount(InventoryHandle handle, uint32_t count);
    void SetStackSize(ItemId itemId, uint32_t maxStack);
    
    void RegisterItemProcessor(const std::string& type, std::unique_ptr<IItemProcessor> processor);
    void UnregisterItemProcessor(const std::string& type);
    
    void RegisterInventoryCallback(InventoryEventType type, const InventoryCallback& callback);
    void UnregisterInventoryCallback(InventoryEventType type);
    
    bool HasItem(InventoryHandle handle, ItemId itemId, uint32_t count = 1) const;
    uint32_t GetItemCount(InventoryHandle handle, ItemId itemId) const;
    float GetCurrentWeight(InventoryHandle handle) const;
    ItemId GetEquippedItem(InventoryHandle handle, const std::string& slot) const;
    
    const RuntimeInventoryStats& GetStats() const { return m_Stats; }

private:
    RuntimeInventorySystem() = default;
    
    RuntimeInventoryConfig m_Config;
    RuntimeInventoryStats m_Stats;
    std::unordered_map<InventoryHandle, InventoryInstance> m_Inventories;
    std::unordered_map<std::string, std::unique_ptr<IItemProcessor>> m_ItemProcessors;
    std::unordered_map<InventoryEventType, InventoryCallback> m_Callbacks;
    std::unique_ptr<IItemDatabase> m_Database;
    bool m_Initialized = false;
};

} // namespace Hydragon::Core 