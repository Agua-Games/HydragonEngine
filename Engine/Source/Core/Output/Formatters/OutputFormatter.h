/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Output formatting and customization
 */

#pragma once
#include "../OutputTypes.h"
#include <string>
#include <memory>

namespace Hydragon::Output {

class IOutputFormatter {
public:
    virtual ~IOutputFormatter() = default;
    virtual std::string Format(const LogMessage& message) = 0;
};

class DefaultFormatter : public IOutputFormatter {
public:
    struct FormatterConfig {
        bool includeTimestamp = true;
        bool includeLevel = true;
        bool includeCategory = true;
        bool includeLocation = true;
        bool includeThreadId = true;
        std::string timestampFormat = "%Y-%m-%d %H:%M:%S";
        std::string separator = " | ";
    };
    
    explicit DefaultFormatter(const FormatterConfig& config = {});
    std::string Format(const LogMessage& message) override;

private:
    FormatterConfig m_Config;
};

class JSONFormatter : public IOutputFormatter {
public:
    struct JSONConfig {
        bool prettyPrint = false;
        bool includeNulls = false;
        bool escapeUnicode = true;
    };
    
    explicit JSONFormatter(const JSONConfig& config = {});
    std::string Format(const LogMessage& message) override;

private:
    JSONConfig m_Config;
};

} // namespace Hydragon::Output 