/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Flexible attribute system for procedural data management
 */

#pragma once
#include "Core/Data/AttributeTypes.h"
#include "Core/Memory/Management/Strategies/DataMemoryStrategy.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Data {

class AttributeSystem {
public:
    static AttributeSystem& Get();
    
    void Initialize(const AttributeSystemConfig& config = {});
    void Shutdown();
    
    void Update();

    // Attribute management
    template<typename T>
    void CreateAttribute(const std::string& name, const AttributeDesc& desc = {}) {
        auto storage = m_Storage.AllocateStorage<T>(desc.owner);
        m_Attributes[name] = AttributeInstance{storage, desc};
        NotifyAttributeCreated(name, desc);
    }

    template<typename T>
    void SetAttributeValue(const std::string& name, size_t index, const T& value) {
        auto& attr = GetAttribute(name);
        attr.storage->SetValue(index, value);
        NotifyAttributeModified(name, index);
    }

    template<typename T>
    T GetAttributeValue(const std::string& name, size_t index) const {
        const auto& attr = GetAttribute(name);
        return attr.storage->GetValue<T>(index);
    }

    // Group management
    GroupHandle CreateGroup(const std::string& name, const GroupDesc& desc = {});
    void DeleteGroup(const std::string& name);
    void AddToGroup(const std::string& name, size_t index);
    void RemoveFromGroup(const std::string& name, size_t index);
    bool IsInGroup(const std::string& name, size_t index) const;

    // Attribute operations
    void InterpolateAttributes(const std::string& sourceName, const std::string& targetName);
    void TransferAttributes(AttributeOwner sourceOwner, AttributeOwner targetOwner);
    void CompactAttributes(const std::string& pattern = "*");
    
    // Event handlers
    void RegisterAttributeHandler(std::unique_ptr<IAttributeHandler> handler);
    void RegisterGroupHandler(std::unique_ptr<IGroupHandler> handler);

    // Queries and utilities
    bool HasAttribute(const std::string& name) const;
    const AttributeDesc& GetAttributeDesc(const std::string& name) const;
    std::vector<std::string> GetAttributeNames(AttributeOwner owner = AttributeOwner::Point) const;
    std::vector<std::string> GetGroupNames(AttributeOwner owner = AttributeOwner::Point) const;
    
    // Debug utilities
    void DumpAttributeState(const std::string& path) const;
    void ValidateAttributeIntegrity();
    AttributeStats GetStats() const { return m_Stats; }

private:
    AttributeSystem() = default;
    
    struct AttributeInstance {
        std::shared_ptr<IAttributeStorage> storage;
        AttributeDesc desc;
    };

    void NotifyAttributeCreated(const std::string& name, const AttributeDesc& desc);
    void NotifyAttributeModified(const std::string& name, size_t index);
    const AttributeInstance& GetAttribute(const std::string& name) const;

    AttributeSystemConfig m_Config;
    AttributeStats m_Stats;
    DataMemoryStrategy m_Storage;
    std::unordered_map<std::string, AttributeInstance> m_Attributes;
    std::unordered_map<std::string, GroupInstance> m_Groups;
    std::vector<std::unique_ptr<IAttributeHandler>> m_AttributeHandlers;
    std::vector<std::unique_ptr<IGroupHandler>> m_GroupHandlers;
    bool m_Initialized = false;
};

} // namespace Hydragon::Data 