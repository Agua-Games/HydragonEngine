/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EventTypes.h
 * @brief Header file for the EventTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - EventTypes is a namespace that contains various types and enums related to events.
 * - It includes types for event triggers, event types, and more.
 * 
 */
#pragma once

namespace hd {

// Define event trigger types
enum class EventTriggerType {
    Button,
    Switch,
    Lever,
    Timer,    // Timer-based event trigger (e.g., timed events)
    Trigger,  // Trigger-based event trigger (e.g., button press)
    Custom
};

// Define event types
enum class EnvironmentEventType {
    Hazard,        // Hazard zone event (e.g., toxic zone)
    Storm,         // Storm event (e.g., lightning storm)
    Sandstorm,     // Sandstorm event (e.g., sandstorm)
    Earthquake,    // Earthquake event (e.g., earthquake)
    Fire,          // Fire event (e.g., fire zone)
    Custom
};

enum class CharacterEventType {
    Combat,        // Combat event (e.g., combat zone)
    Race,          // Race event (e.g., race zone)
    Quest,         // Quest event (e.g., quest zone)
    Birth,         // Birth event (e.g., birth zone)
    Funeral,       // Funeral event (e.g., funeral zone)
    Death,
    Fight,
    Battle,
    War,
    Escape,
    Interaction,   // Interaction event (e.g., interaction zone)
    Oath,
    Admission,
    Dialogue,
    Baptism,
    Marriage,
    Divorce,
    Promotion,
    Demotion,
    Custom
};

enum class DeviceEventType {
    Broadcast,     // Broadcast event (e.g., broadcast zone)
    Teleport,      // Teleport event (e.g., teleportation)
    Monitoring,    // Monitoring event (e.g., monitoring zone)
    Scanner,       // Scanner event (e.g., scanner zone)
    Custom
};

struct HazardEvent {
    float frequency;
    float intensity;
    float duration;
    float damage;
    float spreadRate;
};

struct StormEvent {
    float frequency;
    float intensity;
    float duration;
    float damage;
    float spreadRate;
};

struct SandstormEvent {
    float frequency;
    float intensity;
    float duration;
    float damage;
    float spreadRate;
};

} // namespace hd