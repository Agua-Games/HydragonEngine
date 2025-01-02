/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Device management and detection system
 */

#pragma once
#include "../InputTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>

namespace Hydragon::Input {

class DeviceManager {
public:
    struct DeviceConfig {
        bool enableAutoDetection = true;
        bool enableWireless = true;
        bool enableBluetooth = true;
        uint32_t scanIntervalMs = 1000;
        uint32_t connectionTimeoutMs = 5000;
    };

    static DeviceManager& Get();
    
    void Initialize(const DeviceConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void StartDeviceDiscovery();
    void StopDeviceDiscovery();
    
    bool IsDeviceConnected(DeviceHandle handle) const;
    const DeviceInfo& GetDeviceInfo(DeviceHandle handle) const;
    
    void SetDeviceConfig(DeviceHandle handle, const DeviceConfig& config);
    float GetBatteryLevel(DeviceHandle handle) const;
    
    const DeviceStats& GetStats() const { return m_Stats; }

private:
    DeviceManager() = default;
    
    DeviceConfig m_Config;
    DeviceStats m_Stats;
    std::unique_ptr<IDeviceProvider> m_Provider;
    std::vector<InputDevice> m_Devices;
    bool m_Initialized = false;
};

} // namespace Hydragon::Input 