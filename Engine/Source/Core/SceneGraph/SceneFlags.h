/**
 * ARCHITECTURAL NOTES:
 * - SceneFlags is a utility class for managing scene flags, to be used in the SceneInfo struct (see SceneNode.h), to reduce memory usage considerably for each SceneNode instance.
 * - See the file SceneFlags_README.md in Core/SceneGraph/ for more information.
 * 
 * TODO:
 * - Replace the use of booleans, etc, for SceneInfo member variables with SceneFlags.
 * - Check if we have all needed flags here, and update as needed.
 * - Maybe move the content here to SceneNode.h, for the sake of simplicity and avoiding dependency. Or keep it here to helo with context length reduction for assistants.
 */

#pragma once
#include <bitset>
#include <cstdint>
#include <string>
#include <fmt/format.h>

namespace hd {

// Define flag bits using an enum class for type safety
enum class SceneFlag : uint32_t {
    Culling         = 1 << 0,   // 0x00000001
    FrustumCulling  = 1 << 1,   // 0x00000002
    OcclusionCulling= 1 << 2,   // 0x00000004
    DistanceCulling = 1 << 3,   // 0x00000008
    
    GPUInstancing   = 1 << 4,   // 0x00000010
    AsyncLoading    = 1 << 5,   // 0x00000020
    CacheResults    = 1 << 6,   // 0x00000040
    
    ClipCaching     = 1 << 7,   // 0x00000080
    ClipStreaming   = 1 << 8,   // 0x00000100
    ClipCompression = 1 << 9,   // 0x00000200
    
    // Group masks for convenience
    AllCulling      = Culling | FrustumCulling | OcclusionCulling | DistanceCulling,
    AllCaching      = CacheResults | ClipCaching,
    AllClipFeatures = ClipCaching | ClipStreaming | ClipCompression
};

// Bitfield structure using union for easy access
struct SceneFlags {
    union {
        struct {
            uint32_t culling : 1;
            uint32_t frustumCulling : 1;
            uint32_t occlusionCulling : 1;
            uint32_t distanceCulling : 1;
            
            uint32_t gpuInstancing : 1;
            uint32_t asyncLoading : 1;
            uint32_t cacheResults : 1;
            
            uint32_t clipCaching : 1;
            uint32_t clipStreaming : 1;
            uint32_t clipCompression : 1;
            
            uint32_t reserved : 22;  // Reserved for future use
        };
        uint32_t rawFlags = 0;  // Initialize all flags to 0
    };
    
    // Convenience methods
    void setFlag(SceneFlag flag, bool value = true) {
        if (value)
            rawFlags |= static_cast<uint32_t>(flag);
        else
            rawFlags &= ~static_cast<uint32_t>(flag);
    }
    
    bool hasFlag(SceneFlag flag) const {
        return (rawFlags & static_cast<uint32_t>(flag)) != 0;
    }
    
    bool hasAllFlags(uint32_t flags) const {
        return (rawFlags & flags) == flags;
    }
    
    void clearAllFlags() {
        rawFlags = 0;
    }

    // Debug string function
    std::string SceneFlags::toString() const {
        std::vector<std::string> enabledFlags;
        
        if (flags.culling)          enabledFlags.push_back("Culling");
        if (flags.frustumCulling)   enabledFlags.push_back("FrustumCulling");
        if (flags.occlusionCulling) enabledFlags.push_back("OcclusionCulling");
        if (flags.distanceCulling)  enabledFlags.push_back("DistanceCulling");
        if (flags.gpuInstancing)    enabledFlags.push_back("GPUInstancing");
        if (flags.asyncLoading)     enabledFlags.push_back("AsyncLoading");
        if (flags.cacheResults)     enabledFlags.push_back("CacheResults");
        if (flags.clipCaching)      enabledFlags.push_back("ClipCaching");
        if (flags.clipStreaming)    enabledFlags.push_back("ClipStreaming");
        if (flags.clipCompression)  enabledFlags.push_back("ClipCompression");
        
        std::string result = fmt::format("Flags(0x{:08X}): ", flags.rawFlags);
        for (const auto& flag : enabledFlags) {
            result += flag + " | ";
        }
        return result;
    }
};

} // namespace hd