#pragma once
#include "MemorySecurity.h"
#include <functional>
#include <chrono>

namespace Hydragon {

class NetworkMemorySecurity {
public:
    struct NetworkSecurityConfig {
        bool enableMemoryEncryption = true;
        bool validateRemotePointers = true;
        uint32_t maxRemoteAllocationSize = 1024 * 1024;  // 1MB default
        std::chrono::milliseconds validationTimeout{100}; // 100ms default
    };

    // Secure network memory operations
    static bool ValidateRemoteMemoryAccess(const void* ptr, size_t size);
    static bool EncryptMemoryForNetwork(void* data, size_t size, const uint8_t* key);
    static bool DecryptMemoryFromNetwork(void* data, size_t size, const uint8_t* key);
    
    // Network memory validation callback
    using ValidationCallback = std::function<bool(const void*, size_t)>;
    static void SetValidationCallback(ValidationCallback callback);

private:
    static ValidationCallback s_ValidationCallback;
    static NetworkSecurityConfig s_Config;
};

} // namespace Hydragon 