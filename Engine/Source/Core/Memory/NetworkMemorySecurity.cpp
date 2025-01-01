/*
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 *
 * Network memory security for Hydragon
 */

#include "NetworkMemorySecurity.h"
#include <openssl/aes.h>
#include <openssl/evp.h>

namespace Hydragon {

NetworkMemorySecurity::ValidationCallback NetworkMemorySecurity::s_ValidationCallback;
NetworkMemorySecurity::NetworkSecurityConfig NetworkMemorySecurity::s_Config;

bool NetworkMemorySecurity::ValidateRemoteMemoryAccess(const void* ptr, size_t size) {
    if (!s_Config.validateRemotePointers) return true;
    
    // Size validation
    if (size > s_Config.maxRemoteAllocationSize) return false;
    
    // Pointer alignment check
    if (reinterpret_cast<uintptr_t>(ptr) % alignof(std::max_align_t) != 0) 
        return false;
    
    // Custom validation through callback
    if (s_ValidationCallback) {
        return s_ValidationCallback(ptr, size);
    }
    
    return true;
}

bool NetworkMemorySecurity::EncryptMemoryForNetwork(void* data, size_t size, const uint8_t* key) {
    if (!s_Config.enableMemoryEncryption) return true;
    
    EVP_CIPHER_CTX* ctx = EVP_CIPHER_CTX_new();
    if (!ctx) return false;
    
    // Use AES-256-GCM for authenticated encryption
    if (!EVP_EncryptInit_ex(ctx, EVP_aes_256_gcm(), nullptr, key, nullptr)) {
        EVP_CIPHER_CTX_free(ctx);
        return false;
    }
    
    // Encryption implementation...
    EVP_CIPHER_CTX_free(ctx);
    return true;
}

} // namespace Hydragon 