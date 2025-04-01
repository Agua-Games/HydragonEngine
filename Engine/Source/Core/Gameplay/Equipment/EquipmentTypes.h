/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EquipmentTypes.h
 * @brief Header file for the EquipmentTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - EquipmentTypes is a namespace that contains various types and enums related to equipment.
 * - It includes types for equipment types, equipment data, and more.
 * 
 */
#pragma once

namespace hd {

enum class EquipmentType {
    None, // For when equipment is not equipped, or not equippable.
    Armor,
    Weapon,
    Tool,
    Accessory,
    Hair,
    Consumable,
    Custom
};

} // namespace hd