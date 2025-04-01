/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file SpaceHeaders.h
 * @brief Header file for the Space namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - SpaceHeaders is one of the convenience headers for sparing users the trouble of including all the headers (less verbose) for a specific gameplay set of functionality, 
 * in this case, space.
 * - It includes classes for space probes, space suits, space stations, space beacons, and more.
 * - An important note: try to use these "pack" headers sparingly, generally only if you need most or all of the functionality, as they can cause a lot of bloat, when
 * compiled. For stricter memory economy, prefer explicitly including the headers you need.
 */
#pragma once
#include "AsteroidBelt.h"
#include "AstrophysicsEntity.h"
#include "Satellite.h"
#include "SatellitesArray.h"
#include "SpaceBeacon.h"
#include "SpaceDecoy.h"
#include "SpaceNavMechanics.h"
#include "SpaceProbe.h"
#include "SpacePropulsion.h"
#include "SpaceRadioAntenna.h"
#include "SpaceStation.h"
#include "SpaceSuit.h"
#include "TerraformingDevice.h"