/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Type mapping and conversion system for attributes
 */

#pragma once
#include "Core/Data/AttributeTypes.h"
#include <type_traits>
#include <string>

namespace Hydragon::Data {

// Type mapping specializations
template<typename T>
struct TypeToValueType;

template<> struct TypeToValueType<bool> { static constexpr ValueType value = ValueType::Boolean; };
template<> struct TypeToValueType<int32_t> { static constexpr ValueType value = ValueType::Integer; };
template<> struct TypeToValueType<float> { static constexpr ValueType value = ValueType::Float; };
template<> struct TypeToValueType<std::string> { static constexpr ValueType value = ValueType::String; };

// Value type traits
template<typename T>
constexpr ValueType GetValueTypeForType() {
    return TypeToValueType<std::remove_cv_t<std::remove_reference_t<T>>>::value;
}

// Type conversion utilities
class TypeConverter {
public:
    template<typename To, typename From>
    static To Convert(const From& value) {
        if constexpr (std::is_same_v<To, From>) {
            return value;
        } else {
            return ConvertImpl<To, From>(value);
        }
    }

    static bool CanConvert(ValueType from, ValueType to);
    static std::string GetTypeName(ValueType type);
    
private:
    template<typename To, typename From>
    static To ConvertImpl(const From& value) {
        if constexpr (std::is_arithmetic_v<To> && std::is_arithmetic_v<From>) {
            return static_cast<To>(value);
        } else if constexpr (std::is_same_v<To, std::string>) {
            return std::to_string(value);
        } else {
            throw std::runtime_error("Unsupported type conversion");
        }
    }
};

// Type validation
class TypeValidator {
public:
    static bool ValidateType(ValueType expected, ValueType actual);
    static bool ValidateRange(ValueType type, const std::string& value);
    static bool ValidateFormat(ValueType type, const std::string& value);
    
    template<typename T>
    static bool ValidateValue(const T& value, const AttributeDesc& desc) {
        return ValidateValueImpl(value, desc.type);
    }

private:
    template<typename T>
    static bool ValidateValueImpl(const T& value, ValueType expectedType) {
        return GetValueTypeForType<T>() == expectedType;
    }
};

} // namespace Hydragon::Data 