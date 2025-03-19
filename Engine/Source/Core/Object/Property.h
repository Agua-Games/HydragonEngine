/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Property.h
 * @brief Header file for the Property class.
 * 
 * ARCHITECTURAL NOTES:
 * - Property is a type-safe property class for enhanced object metadata.
 * 
 * TODO:
 * - Check with assistant: explain it and check if this is the right place for this class, how much it overlaps with Object and Node, how much of its 
 * functionality is actually needed considering the other core classes (Object, Node, etc.)
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 */
#if 0
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
    virtual const std::type_info& getType() const = 0;
    virtual void* getRawPtr() = 0;
    virtual const void* getRawPtr() const = 0;
};

/**
 * @brief Type-safe property implementation
 */
template<typename T>
class Property : public PropertyBase {
public:
    // === Structure Definitions ===
    struct PropertyInfo {
        std::string name;
        std::type_info const& type;
    };

    static inline std::unordered_map<PropertyId, PropertyInfo> registeredProperties;

    explicit Property(T&& defaultValue) 
        : value(std::forward<T>(defaultValue)) {}

    const std::type_info& getType() const override { 
        return typeid(T); 
    }

    void* getRawPtr() override { 
        return &value; 
    }

    const void* getRawPtr() const override { 
        return &value; 
    }

    T& get() { return value; }
    const T& get() const { return value; }
    void set(T&& newValue) { value = std::forward<T>(newValue); }

private:
    T value;
};

/**
 * @brief Property registry for compile-time property lookup
 */
class PropertyRegistry {
public:
    template<typename T>
    static PropertyId register(std::string_view name) {
        static PropertyId nextId = 0;
        PropertyId id = nextId++;
        registeredProperties[id] = PropertyInfo{
            .name = std::string(name),
            .type = typeid(T)
        };
        return id;
    }

    static const PropertyInfo& getInfo(PropertyId id) {
        return registeredProperties[id];
    }

private:
};

// Macro for property registration
#define REGISTER_PROPERTY(Type, Name) \
    static const PropertyId Name##Id = PropertyRegistry::register<Type>(#Name)

} // namespace hd

#endif