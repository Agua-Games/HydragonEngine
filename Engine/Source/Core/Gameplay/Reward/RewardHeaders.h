/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RewardHeaders.h
 * @brief Header file for the Reward namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - RewardHeaders is one of the convenience headers for sparing users the trouble of including all the headers (less verbose) for a specific gameplay set of functionality, 
 * in this case, reward.
 * - It includes classes for reward types, reward data, reward mechanics, and more.
 * - An important note: try to use these "pack" headers sparingly, generally only if you need most or all of the functionality, as they can cause a lot of bloat, when
 * compiled. For stricter memory economy, prefer explicitly including the headers you need.
 */
#pragma once
#include "Reward.h"
#include "RewardManager.h"
#include "RewardMechanics.h"
