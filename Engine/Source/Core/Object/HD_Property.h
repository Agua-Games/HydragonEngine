/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <type_traits>
#include <string_view>

namespace hd {

// Property ID for compile-time lookup
using PropertyId = uint32_t;

/**
 * @brief Type-safe property base class
 */
class PropertyBase {
public:
    virtual ~PropertyBase() = default;
    virtual const std::type_info& GetType() const = 0;
    virtual void* GetRawPtr() = 0;
    virtual const void* GetRawPtr() const = 0;
};

/**
 * @brief Type-safe property implementation
 */
template<typename T>
class TypedProperty : public PropertyBase {
public:
    explicit TypedProperty(T&& defaultValue) 
        : value(std::forward<T>(defaultValue)) {}

    const std::type_info& GetType() const override { 
        return typeid(T); 
    }

    void* GetRawPtr() override { 
        return &value; 
    }

    const void* GetRawPtr() const override { 
        return &value; 
    }

    T& Get() { return value; }
    const T& Get() const { return value; }
    void Set(T&& newValue) { value = std::forward<T>(newValue); }

private:
    T value;
};

/**
 * @brief Property registry for compile-time property lookup
 */
class PropertyRegistry {
public:
    template<typename T>
    static PropertyId Register(std::string_view name) {
        static PropertyId nextId = 0;
        PropertyId id = nextId++;
        registeredProperties[id] = PropertyInfo{
            .name = std::string(name),
            .type = typeid(T)
        };
        return id;
    }

private:
    struct PropertyInfo {
        std::string name;
        std::type_info const& type;
    };
    
    static inline std::unordered_map<PropertyId, PropertyInfo> registeredProperties;
};

// Macro for property registration
#define REGISTER_PROPERTY(Type, Name) \
    static const PropertyId Name##Id = PropertyRegistry::Register<Type>(#Name)

} // namespace hd