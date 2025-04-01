/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file CombatHeaders.h
 * @brief Header file for the Combat namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - CombatHeaders is one of the convenience headers for sparing users the trouble of including all the headers (less verbose) for a specific gameplay set of functionality, 
 * in this case, combat.
 * - It includes classes for combat vehicles, combat zones, combat triggers, combat events, and more.
 * - An important note: try to use these "pack" headers sparingly, generally only if you need most or all of the functionality, as they can cause a lot of bloat, when
 * compiled. For stricter memory economy, prefer explicitly including the headers you need.
 */
#pragma once
#include "CombatVehicle.h"
#include "BattleEvent.h"
#include "BattleManager.h"
#include "BattleZone.h"
#include "CombatManager.h"
#include "CombatMechanics.h"
#include "CombatTrigger.h"
#include "CombatTypes.h"
#include "CombatVehicle.h"
