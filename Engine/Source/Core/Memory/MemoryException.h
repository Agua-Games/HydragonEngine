/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Memory exception handling for Hydragon
 */

#pragma once
#include <stdexcept>
#include <string>

namespace Hydragon {
namespace Memory {

class MemoryException : public std::runtime_error {
public:
    enum class ErrorCode {
        AllocationFailed,
        InvalidDeallocation,
        MemoryCorruption,
        OutOfMemory,
        InvalidAlignment,
        ThreadSafetyViolation
    };

    MemoryException(ErrorCode code, const char* message)
        : std::runtime_error(message)
        , m_ErrorCode(code) {}

    ErrorCode getErrorCode() const { return m_ErrorCode; }

private:
    ErrorCode m_ErrorCode;
};

#define HD_THROW_MEMORY_ERROR(code, message) \
    throw MemoryException(MemoryException::ErrorCode::code, message)

#define HD_CHECK_MEMORY(condition, code, message) \
    if (!(condition)) HD_THROW_MEMORY_ERROR(code, message)

}} // namespace Hydragon::Memory 