Here's a comprehensive guide for implementing and using bitfield flags:

1. First, let's define the flags structure:

````cpp path=Engine/Source/Core/SceneGraph/SceneFlags.h mode=EDIT
#pragma once
#include <cstdint>

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
};

} // namespace hd
````

2. Usage in Scene:

````cpp path=Engine/Source/Core/SceneGraph/Scene.h mode=EDIT
class Scene {
public:
    SceneFlags flags;  // Only 4 bytes for all flags
    
    // Fluent interface for flag manipulation
    Scene& enableCulling(bool enable = true) {
        flags.setFlag(SceneFlag::Culling, enable);
        return *this;
    }
    
    Scene& configureCulling(bool frustum, bool occlusion, bool distance) {
        flags.setFlag(SceneFlag::FrustumCulling, frustum);
        flags.setFlag(SceneFlag::OcclusionCulling, occlusion);
        flags.setFlag(SceneFlag::DistanceCulling, distance);
        return *this;
    }
    
    bool isCullingEnabled() const {
        return flags.hasFlag(SceneFlag::Culling);
    }
    
    bool hasAdvancedCulling() const {
        return flags.hasAllFlags(static_cast<uint32_t>(SceneFlag::AllCulling));
    }
};
````

3. Usage examples:

````cpp path=Engine/Source/Core/SceneGraph/Examples/SceneFlags_Usage.cpp mode=EDIT
// Example 1: Basic flag manipulation
Scene node;
node.flags.setFlag(SceneFlag::Culling, true);
node.flags.setFlag(SceneFlag::GPUInstancing, true);

if (node.flags.hasFlag(SceneFlag::Culling)) {
    // Handle culling...
}

// Example 2: Using fluent interface
node.enableCulling()
    .configureCulling(true, false, true);

// Example 3: Working with groups of flags
if (node.flags.hasAllFlags(static_cast<uint32_t>(SceneFlag::AllCulling))) {
    // All culling features are enabled
}

// Example 4: Combining flags
SceneFlags flags;
flags.rawFlags = static_cast<uint32_t>(SceneFlag::Culling) | 
                 static_cast<uint32_t>(SceneFlag::GPUInstancing);

// Example 5: Direct bit access (more efficient but less safe)
node.flags.culling = true;
node.flags.gpuInstancing = true;

if (node.flags.culling && node.flags.frustumCulling) {
    // Handle specific flag combination...
}

// Example 6: Serialization
void serialize(const SceneFlags& flags, Serializer& s) {
    s.write(flags.rawFlags);  // Just write 4 bytes
}

void deserialize(SceneFlags& flags, Deserializer& d) {
    d.read(flags.rawFlags);   // Just read 4 bytes
}
````

4. Debug support:

````cpp path=Engine/Source/Core/SceneGraph/SceneFlags.cpp mode=EDIT
std::string getFlagsDebugString(const SceneFlags& flags) {
    std::vector<std::string> enabledFlags;
    
    if (flags.culling)          enabledFlags.push_back("Culling");
    if (flags.frustumCulling)   enabledFlags.push_back("FrustumCulling");
    if (flags.occlusionCulling) enabledFlags.push_back("OcclusionCulling");
    if (flags.distanceCulling)  enabledFlags.push_back("DistanceCulling");
    if (flags.gpuInstancing)    enabledFlags.push_back("GPUInstancing");
    // ... add other flags
    
    std::string result = fmt::format("Flags(0x{:08X}): ", flags.rawFlags);
    for (const auto& flag : enabledFlags) {
        result += flag + " | ";
    }
    return result;
}
````

Key benefits:
1. Memory efficient: 32 flags in just 4 bytes
2. Fast operations: Bit manipulation is very fast
3. Type-safe when using enum class
4. Easy to serialize/deserialize
5. Good debugging support

Drawbacks:
1. Limited to 32 flags (can use uint64_t if more needed)
2. Bit fields order is compiler-dependent
3. Need to be careful with direct bit access vs. safe methods

Would you like me to:
1. Show how to implement more complex flag combinations?
2. Demonstrate integration with other systems?
3. Explain the bitwise operations in more detail?
