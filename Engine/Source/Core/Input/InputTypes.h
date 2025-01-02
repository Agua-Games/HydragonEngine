/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Input system type definitions
 */

#pragma once
#include "Core/Math/Vector2.h"
#include <cstdint>
#include <string>
#include <functional>

namespace Hydragon::Input {

using DeviceHandle = uint32_t;
using ActionHandle = uint32_t;

enum class DeviceType {
    Keyboard,
    Mouse,
    Gamepad,
    TouchScreen,
    VRController,
    Custom
};

enum class InputType {
    Button,
    Axis1D,
    Axis2D,
    Trigger,
    Motion,
    Touch
};

struct DeviceDesc {
    std::string name;
    DeviceType type;
    bool wireless = false;
    bool hasBattery = false;
    bool hasHaptics = false;
    std::vector<InputType> supportedInputs;
};

struct ActionDesc {
    std::string name;
    InputType type;
    bool continuous = false;
    float sensitivity = 1.0f;
    float threshold = 0.5f;
};

struct ActionBinding {
    DeviceHandle device;
    uint32_t inputId;
    float scale = 1.0f;
    bool invert = false;
};

struct HapticEffect {
    float intensity;
    float duration;
    float frequency;
    bool loop = false;
};

using ActionCallback = std::function<void(ActionHandle, float)>;

struct InputStats {
    uint32_t connectedDevices;
    uint32_t activeActions;
    uint32_t activeBindings;
    uint32_t eventsProcessed;
    float inputLatencyMs;
    size_t memoryUsed;
};

} // namespace Hydragon::Input 