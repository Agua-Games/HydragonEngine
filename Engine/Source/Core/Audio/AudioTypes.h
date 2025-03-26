/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AudioTypes.h
 * @brief Header file for the AudioTypes namespace.
 * 
 * ARCHITECTURAL NOTES:
 * - AudioTypes is a namespace that contains various types and enums related to audio.
 * - It includes types for audio formats, audio channels, and more.
 * 
 */
#pragma once

namespace hd {

// Define audio formats
enum class AudioFormat {
    PCM,
    MP3,
    OGG,
    WAV,
    FLAC,
    Custom
};

// Define audio channels
enum class AudioChannel {
    Mono,
    Stereo,
    Surround,
    Custom
};

} // namespace hd