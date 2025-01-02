/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Output channel interface and implementations
 */

#pragma once
#include "../OutputTypes.h"
#include <memory>
#include <fstream>

namespace Hydragon::Output {

class IOutputChannel {
public:
    virtual ~IOutputChannel() = default;
    virtual void Write(const LogMessage& message) = 0;
    virtual void Flush() = 0;
    virtual bool IsValid() const = 0;
};

class FileOutputChannel : public IOutputChannel {
public:
    struct FileConfig {
        std::string filename;
        size_t maxSize = 100 * 1024 * 1024;  // 100MB
        bool appendMode = true;
        bool autoFlush = false;
        bool enableRotation = true;
        uint32_t maxRotatedFiles = 5;
    };
    
    explicit FileOutputChannel(const FileConfig& config);
    void Write(const LogMessage& message) override;
    void Flush() override;
    bool IsValid() const override;

private:
    FileConfig m_Config;
    std::ofstream m_File;
    size_t m_CurrentSize = 0;
};

class ConsoleOutputChannel : public IOutputChannel {
public:
    struct ConsoleConfig {
        bool enableColors = true;
        bool enableFormatting = true;
        bool redirectStderr = true;
    };
    
    explicit ConsoleOutputChannel(const ConsoleConfig& config = {});
    void Write(const LogMessage& message) override;
    void Flush() override;
    bool IsValid() const override;

private:
    ConsoleConfig m_Config;
};

class NetworkOutputChannel : public IOutputChannel {
public:
    struct NetworkConfig {
        std::string host;
        uint16_t port;
        bool useTLS = true;
        uint32_t retryInterval = 5000;  // ms
        uint32_t bufferSize = 8192;
    };
    
    explicit NetworkOutputChannel(const NetworkConfig& config);
    void Write(const LogMessage& message) override;
    void Flush() override;
    bool IsValid() const override;

private:
    NetworkConfig m_Config;
    std::unique_ptr<INetworkConnection> m_Connection;
};

} // namespace Hydragon::Output 