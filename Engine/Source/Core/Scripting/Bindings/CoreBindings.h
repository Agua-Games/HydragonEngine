/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core engine functionality bindings for scripting languages
 */

#pragma once
#include "Core/Scripting/ScriptingTypes.h"
#include <memory>
#include <string>
#include <vector>
#include <unordered_map>

namespace Hydragon::Scripting {

struct CoreBindingsConfig {
    bool enableReflection = true;
    bool enableTypeConversion = true;
    bool enableExceptionHandling = true;
    bool enableDebugInfo = true;
    uint32_t maxCallStackDepth = 1000;
    std::string scriptRootPath = "Scripts";
};

class CoreBindings {
public:
    static CoreBindings& Get();
    
    void Initialize(const CoreBindingsConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void RegisterType(const std::string& name, const TypeInfo& typeInfo);
    void UnregisterType(const std::string& name);
    
    void RegisterFunction(const std::string& name, const FunctionInfo& funcInfo);
    void UnregisterFunction(const std::string& name);
    
    void RegisterEnum(const std::string& name, const EnumInfo& enumInfo);
    void UnregisterEnum(const std::string& name);
    
    void RegisterConverter(const TypePair& types, std::unique_ptr<ITypeConverter> converter);
    void UnregisterConverter(const TypePair& types);
    
    const CoreBindingsStats& GetStats() const { return m_Stats; }

private:
    CoreBindings() = default;
    
    CoreBindingsConfig m_Config;
    CoreBindingsStats m_Stats;
    std::unordered_map<std::string, TypeInfo> m_Types;
    std::unordered_map<std::string, FunctionInfo> m_Functions;
    std::unordered_map<std::string, EnumInfo> m_Enums;
    std::unordered_map<TypePair, std::unique_ptr<ITypeConverter>> m_Converters;
    std::unique_ptr<IBindingProcessor> m_Processor;
    bool m_Initialized = false;
};

} // namespace Hydragon::Scripting 