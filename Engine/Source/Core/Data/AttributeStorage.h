/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Attribute storage system for flexible data management
 */

#pragma once
#include "Core/Data/AttributeTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::Data {

// Interface for attribute storage
class IAttributeStorage {
public:
    virtual ~IAttributeStorage() = default;
    
    virtual void Resize(size_t size) = 0;
    virtual void Clear() = 0;
    virtual size_t Size() const = 0;
    virtual uint64_t MemoryUsage() const = 0;
    
    template<typename T>
    void SetValue(size_t index, const T& value);
    
    template<typename T>
    T GetValue(size_t index) const;
    
    virtual void* GetRawData() = 0;
    virtual const void* GetRawData() const = 0;
    virtual ValueType GetValueType() const = 0;
};

// Concrete attribute storage implementation
template<typename T>
class AttributeStorage : public IAttributeStorage {
public:
    explicit AttributeStorage(AttributeOwner owner, size_t initialSize = 0)
        : m_Owner(owner)
        , m_Data(initialSize) {}
    
    void Resize(size_t size) override {
        m_Data.resize(size);
    }
    
    void Clear() override {
        m_Data.clear();
        m_Data.shrink_to_fit();
    }
    
    size_t Size() const override {
        return m_Data.size();
    }
    
    uint64_t MemoryUsage() const override {
        return sizeof(T) * m_Data.capacity() + sizeof(*this);
    }
    
    void SetValue(size_t index, const T& value) {
        if (index >= m_Data.size()) {
            m_Data.resize(index + 1);
        }
        m_Data[index] = value;
    }
    
    T GetValue(size_t index) const {
        return index < m_Data.size() ? m_Data[index] : T{};
    }
    
    void* GetRawData() override {
        return m_Data.data();
    }
    
    const void* GetRawData() const override {
        return m_Data.data();
    }
    
    ValueType GetValueType() const override {
        return GetValueTypeForType<T>();
    }

private:
    AttributeOwner m_Owner;
    std::vector<T> m_Data;
    
    template<typename U>
    static ValueType GetValueTypeForType();  // Implemented for each supported type
};

// Specialized storage for arrays and dictionaries
class ArrayAttributeStorage : public IAttributeStorage {
public:
    explicit ArrayAttributeStorage(AttributeOwner owner, ValueType elementType);
    // Array-specific implementation...
};

class DictionaryAttributeStorage : public IAttributeStorage {
public:
    explicit DictionaryAttributeStorage(AttributeOwner owner, ValueType keyType, ValueType valueType);
    // Dictionary-specific implementation...
};

} // namespace Hydragon::Data 