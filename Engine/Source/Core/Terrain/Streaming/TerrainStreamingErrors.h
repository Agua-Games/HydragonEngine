/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Error handling for terrain streaming
 */

#pragma once
#include "Core/Common/Error.h"

namespace Hydragon {
namespace Terrain {

enum class StreamingErrorCode {
    None = 0,
    OutOfMemory,
    DeviceNotSupported,
    InvalidRegion,
    StreamingFailed,
    InvalidResource,
    BufferOverflow,
    InvalidState
};

class StreamingError : public Error {
public:
    StreamingError(StreamingErrorCode code, const char* message) 
        : Error(message)
        , m_Code(code) {}

    StreamingErrorCode code() const { return m_Code; }
    
    static StreamingError outOfMemory(const char* details) {
        return StreamingError(StreamingErrorCode::OutOfMemory, 
            fmt::format("Out of memory: {}", details).c_str());
    }
    
    static StreamingError deviceNotSupported(const char* feature) {
        return StreamingError(StreamingErrorCode::DeviceNotSupported,
            fmt::format("Device doesn't support: {}", feature).c_str());
    }

private:
    StreamingErrorCode m_Code;
};

}} // namespace Hydragon::Terrain 