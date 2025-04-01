/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file DialogueHeaders.h
 * @brief Header file for the Dialogue namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - DialogueHeaders is one of the convenience headers for sparing users the trouble of including all the headers (less verbose) for a specific gameplay set of functionality,
 * in this case, dialogue.
 * - It includes classes for dialogue branches, dialogue checkpoints, dialogue managers, dialogue states, dialogue statuses, and more.
 * - An important note: try to use these "pack" headers sparingly, generally only if you need most or all of the functionality, as they can cause a lot of bloat, when
 * compiled. For stricter memory economy, prefer explicitly including the headers you need.
 */
#pragma once
#include "DialogueBranch.h"
#include "DialogueCheckpoint.h"
#include "DialogueManager.h"
#include "GameplayManager.h"    // Necessary for global flags, for dialogue - flagging gameplay events
