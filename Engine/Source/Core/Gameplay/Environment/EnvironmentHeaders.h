/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EnvironmentHeaders.h
 * @brief Header file for the Environment namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - EnvironmentHeaders is one of the convenience headers for sparing users the trouble of including all the headers (less verbose) for a specific gameplay set of functionality, 
 * in this case, environment.
 * - It includes classes for environment conditions, environment events, environment sensors, and more.
 * - An important note: try to use these "pack" headers sparingly, generally only if you need most or all of the functionality, as they can cause a lot of bloat, when
 * compiled. For stricter memory economy, prefer explicitly including the headers you need.
 */
#pragma once
#include "EnvironmentManager.h"
#include "EnvironmentSensor.h"
#include "EnvironmentTypes.h"
