/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Specialized attribute storage implementations
 */

#pragma once
#include "Core/Data/AttributeStorage.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <variant>
#include <any>

namespace Hydragon::Data {

// Array storage implementation
class ArrayAttributeStorage : public IAttributeStorage {
public:
    explicit ArrayAttributeStorage(AttributeOwner owner, ValueType elementType)
        : m_Owner(owner)
        , m_ElementType(elementType) {}

    void Resize(size_t size) override {
        m_Arrays.resize(size);
    }

    void Clear() override {
        m_Arrays.clear();
        m_Arrays.shrink_to_fit();
    }

    size_t Size() const override { return m_Arrays.size(); }
    
    uint64_t MemoryUsage() const override {
        uint64_t total = sizeof(*this);
        for (const auto& arr : m_Arrays) {
            total += arr.capacity() * GetElementSize();
        }
        return total;
    }

    template<typename T>
    void PushBack(size_t index, const T& value) {
        EnsureCompatibleType<T>();
        EnsureIndex(index);
        m_Arrays[index].push_back(std::any(value));
    }

    template<typename T>
    T Get(size_t arrayIndex, size_t elementIndex) const {
        EnsureCompatibleType<T>();
        return std::any_cast<T>(m_Arrays[arrayIndex][elementIndex]);
    }

    void* GetRawData() override { return m_Arrays.data(); }
    const void* GetRawData() const override { return m_Arrays.data(); }
    ValueType GetValueType() const override { return ValueType::Array; }
    ValueType GetElementType() const { return m_ElementType; }

private:
    AttributeOwner m_Owner;
    ValueType m_ElementType;
    std::vector<std::vector<std::any>> m_Arrays;

    template<typename T>
    void EnsureCompatibleType() const {
        if (GetValueTypeForType<T>() != m_ElementType) {
            throw std::runtime_error("Type mismatch in array storage");
        }
    }

    void EnsureIndex(size_t index) {
        if (index >= m_Arrays.size()) {
            m_Arrays.resize(index + 1);
        }
    }

    size_t GetElementSize() const;
};

// Dictionary storage implementation
class DictionaryAttributeStorage : public IAttributeStorage {
public:
    explicit DictionaryAttributeStorage(AttributeOwner owner, ValueType keyType, ValueType valueType)
        : m_Owner(owner)
        , m_KeyType(keyType)
        , m_ValueType(valueType) {}

    void Resize(size_t size) override {
        m_Dictionaries.resize(size);
    }

    void Clear() override {
        m_Dictionaries.clear();
        m_Dictionaries.shrink_to_fit();
    }

    size_t Size() const override { return m_Dictionaries.size(); }
    
    uint64_t MemoryUsage() const override {
        uint64_t total = sizeof(*this);
        for (const auto& dict : m_Dictionaries) {
            total += dict.size() * (GetKeySize() + GetValueSize());
        }
        return total;
    }

    template<typename K, typename V>
    void Set(size_t index, const K& key, const V& value) {
        EnsureCompatibleTypes<K, V>();
        EnsureIndex(index);
        m_Dictionaries[index][std::any(key)] = std::any(value);
    }

    template<typename K, typename V>
    V Get(size_t dictIndex, const K& key) const {
        EnsureCompatibleTypes<K, V>();
        const auto& dict = m_Dictionaries[dictIndex];
        auto it = dict.find(std::any(key));
        return it != dict.end() ? std::any_cast<V>(it->second) : V{};
    }

    void* GetRawData() override { return m_Dictionaries.data(); }
    const void* GetRawData() const override { return m_Dictionaries.data(); }
    ValueType GetValueType() const override { return ValueType::Object; }
    
    std::pair<ValueType, ValueType> GetKeyValueTypes() const {
        return {m_KeyType, m_ValueType};
    }

private:
    AttributeOwner m_Owner;
    ValueType m_KeyType;
    ValueType m_ValueType;
    std::vector<std::unordered_map<std::any, std::any, AnyHash>> m_Dictionaries;

    template<typename K, typename V>
    void EnsureCompatibleTypes() const {
        if (GetValueTypeForType<K>() != m_KeyType || 
            GetValueTypeForType<V>() != m_ValueType) {
            throw std::runtime_error("Type mismatch in dictionary storage");
        }
    }

    void EnsureIndex(size_t index) {
        if (index >= m_Dictionaries.size()) {
            m_Dictionaries.resize(index + 1);
        }
    }

    size_t GetKeySize() const;
    size_t GetValueSize() const;
};

// Helper for std::any hashing
struct AnyHash {
    size_t operator()(const std::any& a) const {
        return a.type().hash_code();
    }
};

} // namespace Hydragon::Data 