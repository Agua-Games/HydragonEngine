/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file EngineTypes.h
 * @brief Header file for the EngineTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - EngineTypes is a namespace that contains various types and enums related to the engine.
 * - It includes types for engine states, engine modes, and more.
 * 
 */
#pragma once

namespace hd {

enum class EngineMode {
    GAMEPLAY_ONLY,       // Full gameplay enabled, editor disabled, overlays disabled
    EDITOR_ONLY,         // Core systems only, gameplay logic disabled, editor overlays enabled
    FULL,                // Full systems with editor overlays, like gameplay, audio, etc. enabled.
    EDITOR_MINIMAL,      // Full gameplay enabled, only core editor systems, like nodegraph, coroutines, for immersive editing experience.
    SELECTIVE            // Selective subsystems enabled, like gameplay, audio, etc. based on mode.
};

} // namespace hd