/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EnvironmentSensor.h
 * @brief EnvironmentSensor represents an environment sensor in the engine's node graph.
 * 
 * ARCHITECTURAL NOTES:
 * - EnvironmentSensor is a class that represents an environment sensor in Hydragon.
 * - It is used to represent any environment sensor in the game world, from character sensory organs, senses, to environment sensors, such as cameras, microphones, etc.
 * - For vision sensors, look for the EM (Electromagnetic) parameters, as vision is a narrow band of the EM spectrum.
 * - It supports interactive features, such as sensing, monitoring, and analyzing. Also supports two-way messaging with other environment sensors, environment, character, etc.
 * - It supports sampling different types of environment data, such as light, sound, temperature, humidity, etc.
 * - Not to be confused with EnvironmentManager, which is a class that manages the environment in the game.
 */

#pragma once

#include "Node.h"
#include "DataTable.h"
#include "Noise.h"

namespace hd {

struct EnvironmentSensorInfo : public NodeInfo {
    EnvironmentSensorInfo() {
        nodeType = "Gameplay/EnvironmentSensor";
        
        inputs = {
            "sensorType",                // Type of environment sensor (camera, microphone, etc.)
            "visionCone",                // Vision cone of the environment sensor
            "visionRange",               // Vision range of the environment sensor
            "lightSensitivity",          // Light sensitivity of the environment sensor
            "soundDetectionRadius",      // Sound detection radius of the environment sensor
            "soundDetectionBinaural",    // Sound detection binaural of the environment sensor
            "alertnessLevel",            // Alertness level of the environment sensor (0.0f to 1.0f)
            "suspicionThreshold",        // Suspicion threshold of the environment sensor
            "sensorData",                // Environment sensor data
            "environment",               // Environment data
            "characterData",             // Character data
            "sensorState"                // Environment sensor state
        };
        
        outputs = {
            "sensorStatus",      // Environment sensor status
            "sensorMetrics"      // Environment sensor performance metrics
        };
    }
};

class EnvironmentSensor : public Node {
public:
    // === Structure Definitions ===
    enum class SensorType {
        Acoustic,
        Visual,
        Thermal,
        Chemical,
        Electrical,
        Magnetic,
        EM,
        Gravity,        // For gravity sensors, gyroscopes, etc.
        Kinetic,
        Other
    };

    enum class SensorDevice {
        Audition,
        Vision,
        Thermoreception,
        Olfaction,
        Tactition,
        Gustation,
        Camera,
        Microphone,
        Thermometer,
        GasSensor,
        Voltmeter,
        Ohmmeter,
        Electrometer,
        Magnetometer,
        Accelerometer,
        Gyroscope,
        Other
    };

    enum class EMRangeLowEnd {
        Radio,
        Microwave,
        Infrared,
        Visible,
        Ultraviolet,
        XRay,
        GammaRay,
        CosmicRay
    };

    enum class EMRangeHighEnd {
        Radio,
        Microwave,
        Infrared,
        Visible,
        Ultraviolet,
        XRay,
        GammaRay,
        CosmicRay
    };

    // === Allocation, Initialization, Loading ===
    explicit EnvironmentSensor(const EnvironmentSensorInfo& info = EnvironmentSensorInfo())
        : Node(info) {}    // Default constructor
    void initialize() override {}
    void load() override {}

    // Set default values
    DataTable environmentSensorData;
    DataTable environmentSensorState;
    SensorType sensorType = SensorType::Other;
    SensorDevice sensorDevice = SensorDevice::Other;
    bool isDevice = false;
    Noise noise;                              // For both emitted noise and detection noise
    bool EMDetection = false;                 // Covers radio, microwave, infrared, visible, ultraviolet, x-ray, gamma-ray, cosmic-ray
    float EMDetectionSensitivity = 0.0f;
    float EMDetectionCone = 0.0f;             // For directional detection. Note: this is used to model visionCone
    float EMDetectionRange = 0.0f;            // For range detection. Note: this is used to model visionRange
    bool nightVision = false;
    EMRangeLowEnd emRangeLowEnd = EMRangeLowEnd::Radio;
    EMRangeHighEnd emRangeHighEnd = EMRangeHighEnd::CosmicRay;
    float EMDeviceResolution = 0.0f;

    bool thermalDetection = false;
    float thermalDetectionSensitivity = 0.0f;
    float thermalDetectionRange = 0.0f;
    bool thermalDetectionCone = false;

    bool soundDetection = false;
    float soundDetectionSensitivity = 0.0f;
    float soundDetectionThreshold = 0.0f;
    float soundDetectionRadius = 0.0f;
    bool soundDetectionBinaural = false;
    bool soundDetectionCone = false;
    bool soundDetectionDirectional = false;
    bool soundDetectionFrequency = false;

    bool tactileDetection = false;
    bool olfactoryDetection = false;
    bool gustatoryDetection = false;
    bool chemicalDetection = false;

    float alertnessLevel = 0.0f;
    float suspicionThreshold = 0.0f;

    // === Processing ===
    void sampleEnvironmentData();
    void senseEnvironmentData();
    void monitorEnvironmentData();
    void analyzeEnvironmentData();
    void processEnvironmentSensor();
    void processNode() override {}
    void update() override {}

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~EnvironmentSensor() = default;     // Default destructor
};
}