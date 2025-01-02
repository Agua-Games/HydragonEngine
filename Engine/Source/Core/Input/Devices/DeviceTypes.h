/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Device system type definitions
 */

#pragma once
#include <cstdint>
#include <string>
#include <vector>

namespace Hydragon::Input {

struct DeviceInfo {
    std::string name;
    std::string manufacturer;
    std::string serialNumber;
    std::string firmwareVersion;
    DeviceType type;
    bool wireless;
    bool hasBattery;
    bool hasHaptics;
    std::vector<InputType> capabilities;
};

struct DeviceStats {
    uint32_t totalDevices;
    uint32_t connectedDevices;
    uint32_t wirelessDevices;
    uint32_t discoveryAttempts;
    float lastScanTimeMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Input 