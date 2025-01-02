/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Plugin interface definitions and management
 */

#pragma once
#include "../PluginTypes.h"
#include <string>
#include <memory>

namespace Hydragon::Plugin {

class IPluginInterface {
public:
    virtual ~IPluginInterface() = default;
    virtual const char* GetInterfaceName() const = 0;
    virtual Version GetInterfaceVersion() const = 0;
};

template<typename T>
class PluginInterface : public IPluginInterface {
public:
    static const char* InterfaceName;
    static const Version InterfaceVersion;
    
    const char* GetInterfaceName() const override { return InterfaceName; }
    Version GetInterfaceVersion() const override { return InterfaceVersion; }
    
    virtual T* GetImplementation() = 0;
};

class InterfaceManager {
public:
    template<typename T>
    bool RegisterInterface(std::unique_ptr<T> interface) {
        return RegisterInterfaceImpl(interface->GetInterfaceName(), 
                                   interface->GetInterfaceVersion(),
                                   std::move(interface));
    }
    
    template<typename T>
    T* GetInterface() const {
        auto* interface = GetInterfaceImpl(T::InterfaceName);
        return interface ? static_cast<T*>(interface) : nullptr;
    }

private:
    virtual bool RegisterInterfaceImpl(const char* name, Version version, 
                                     std::unique_ptr<IPluginInterface> interface) = 0;
    virtual IPluginInterface* GetInterfaceImpl(const char* name) const = 0;
};

} // namespace Hydragon::Plugin 