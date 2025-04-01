/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file HorseGameplaHeaders.h
 * @brief Header file for the Horse namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - HorseGameplaHeaders is one of the convenience headers for sparing users the trouble of including all the headers (less verbose) for a specific gameplay set of functionality, 
 * in this case, horse.
 * - It includes classes for horse, horse AI, horse mechanics, horse types, horse breeds, and more.
 * - An important note: try to use these "pack" headers sparingly, generally only if you need most or all of the functionality, as they can cause a lot of bloat, when
 * compiled. For stricter memory economy, prefer explicitly including the headers you need.
 */
#pragma once
#include "Horse.h"
#include "HorseAI.h"
#include "HorseMechanics.h"
#include "RescueRiderAbility.h"
#include "WarHorseMechanics.h"
#include "WingAttackAbility.h"