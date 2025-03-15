/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Property.h
 * @brief Header file for the Property class.
 * 
 * ARCHITECTURAL NOTES:
 * - Property is a type-safe property class for enhanced object metadata.
 * 
 * TODO:
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 */
#pragma once
#include <type_traits>
#include <unordered_map>
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