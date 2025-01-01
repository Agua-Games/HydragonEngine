#pragma once
#include "Core/Memory/Management/Strategies/SceneMemoryStrategy.h"

namespace Hydragon {
namespace Scene {

class SceneSystem {
public:
    // Efficient scene graph management
    struct SceneConfig {
        bool enableSpatialHashing = true;
        bool useComponentArrays = true;
        size_t initialEntityCapacity = 10000;
    };

    // Python-friendly scene operations
    EntityHandle createEntity(const char* name = nullptr) {
        return internalCreateEntity(name);
    }
};

/*
    Python usage:
    scene = hd.scene()
    
    # Natural entity creation
    player = scene.create_entity("player")
    player.add_component("transform")
    player.add_component("mesh", model="character.fbx")
*/

}} // namespace Hydragon::Scene 