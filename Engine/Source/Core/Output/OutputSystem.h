/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Core output management and logging system
 */

#pragma once
#include "OutputTypes.h"
#include "Core/Memory/Management/IMemoryStrategy.h"
#include <memory>
#include <vector>
#include <string>

namespace Hydragon::Output {

class OutputSystem {
public:
    struct OutputConfig {
        bool enableFileOutput = true;
        bool enableConsoleOutput = true;
        bool enableRemoteOutput = false;
        bool enableTimestamps = true;
        bool enableSourceLocation = true;
        bool enableThreadId = true;
        uint32_t maxQueueSize = 10000;
        size_t maxFileSize = 100 * 1024 * 1024;  // 100MB
        std::string outputDirectory;
    };

    static OutputSystem& Get();
    
    void Initialize(const OutputConfig& config = {});
    void Shutdown();
    
    void Update();
    
    void Log(const LogMessage& message);
    void LogFormat(LogLevel level, const char* format, ...);
    void LogError(const char* message, const char* file = nullptr, int line = 0);
    
    void AddOutputChannel(std::unique_ptr<IOutputChannel> channel);
    void RemoveOutputChannel(OutputChannelId id);
    
    void SetLogLevel(LogLevel level);
    void SetLogFilter(const LogFilter& filter);
    
    void Flush();
    
    const OutputStats& GetStats() const { return m_Stats; }

private:
    OutputSystem() = default;
    
    OutputConfig m_Config;
    OutputStats m_Stats;
    std::vector<std::unique_ptr<IOutputChannel>> m_Channels;
    std::unique_ptr<IOutputFormatter> m_Formatter;
    bool m_Initialized = false;
};

} // namespace Hydragon::Output 