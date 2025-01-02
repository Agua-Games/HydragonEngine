/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Centralized logging system
 */

#pragma once
#include <memory>
#include <string>
#include <vector>
#include <source_location>

namespace Hydragon::Utilities {

enum class LogLevel {
    Trace,
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

struct LogConfig {
    bool enableConsole = true;
    bool enableFile = true;
    bool enableTimestamps = true;
    bool enableSourceLocation = true;
    std::string logFilePath = "game.log";
    LogLevel minLevel = LogLevel::Info;
};

class LogSystem {
public:
    static LogSystem& Get();
    
    void Initialize(const LogConfig& config = {});
    void Shutdown();
    
    void Log(LogLevel level, const std::string& message, 
             const std::source_location& location = std::source_location::current());
             
    void Trace(const std::string& message);
    void Debug(const std::string& message);
    void Info(const std::string& message);
    void Warning(const std::string& message);
    void Error(const std::string& message);
    void Fatal(const std::string& message);
    
    void SetLogLevel(LogLevel level);
    void AddLogHandler(std::unique_ptr<ILogHandler> handler);
    
    const LogStats& GetStats() const { return m_Stats; }

private:
    LogSystem() = default;
    
    LogConfig m_Config;
    LogStats m_Stats;
    std::vector<std::unique_ptr<ILogHandler>> m_Handlers;
    bool m_Initialized = false;
};

} // namespace Hydragon::Utilities 