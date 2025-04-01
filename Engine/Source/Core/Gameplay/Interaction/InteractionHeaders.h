/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file InteractionHeaders.h
 * @brief Header file for the Interaction namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - InteractionHeaders is one of the convenience headers for sparing users the trouble of including all the headers (less verbose) for a specific gameplay set of functionality, 
 * in this case, interaction.
 * - It includes classes for interaction events, interaction mechanics, interaction states, and more.
 * - An important note: try to use these "pack" headers sparingly, generally only if you need most or all of the functionality, as they can cause a lot of bloat, when
 * compiled. For stricter memory economy, prefer explicitly including the headers you need.
 */
#pragma once
#include "InteractionEvent.h"
#include "InteractionManager.h"
#include "InteractionMechanics.h"