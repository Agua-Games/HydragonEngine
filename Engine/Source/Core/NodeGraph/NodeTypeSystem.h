/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * NodeTypeSystem provides the foundation for node compatibility
 * and validation in the engine's node-graph architecture.
 */
#pragma once
#include <string>
#include <vector>
#include <map>
#include <functional>

namespace hd {

/**
 * Represents a data type in the node system with compatibility information
 */
class DataType {
public:
    DataType(const std::string& name, const std::string& category);
    
    // Type compatibility
    bool IsCompatibleWith(const DataType& other) const;
    bool CanConvertTo(const DataType& other) const;
    float GetConversionCost(const DataType& other) const;
    
    // Type information
    const std::string& GetName() const { return m_name; }
    const std::string& GetCategory() const { return m_category; }
    
    // Type visualization
    ImColor GetTypeColor() const { return m_color; }
    const std::string& GetIcon() const { return m_icon; }
    
private:
    std::string m_name;
    std::string m_category;
    ImColor m_color;
    std::string m_icon;
    std::vector<std::string> m_compatibleTypes;
    std::map<std::string, float> m_conversionCosts;
};

/**
 * Central registry for all data types in the node system
 */
class TypeRegistry {
public:
    static TypeRegistry& Get();
    
    // Type registration
    void RegisterType(const DataType& type);
    void RegisterTypeConversion(const std::string& fromType, 
                               const std::string& toType, 
                               float conversionCost,
                               std::function<void(const void*, void*)> converter);
    
    // Type lookup
    DataType* GetType(const std::string& name);
    bool AreTypesCompatible(const std::string& sourceType, const std::string& targetType);
    float GetConversionCost(const std::string& sourceType, const std::string& targetType);
    
    // Type conversion
    bool CanConvert(const std::string& sourceType, const std::string& targetType);
    void* ConvertValue(const std::string& sourceType, const std::string& targetType, const void* value);
    
private:
    std::map<std::string, DataType> m_types;
    std::map<std::pair<std::string, std::string>, std::function<void(const void*, void*)>> m_converters;
};

} // namespace hd