// Scene.h
//
// Scene class representing a 3D Entity with a spatial transform.
// Inherits from hd::Node to support nesting and graph connections.

#pragma once
#include "../NodeGraph/Node.h"

#if defined(HYDRAGON_USE_FILAMENT)
#include <math/mat4.h>
#include <utils/Entity.h>

namespace filament {
class Engine;
}
#else
namespace filament {
class Engine;
namespace math {
struct float3 {
    float x, y, z;
    float3(float x = 0.f, float y = 0.f, float z = 0.f) : x(x), y(y), z(z) {}
};
struct mat4f {
    mat4f() = default;
    static mat4f translation(const float3&) { return mat4f(); }
};
}
}
namespace utils {
struct Entity {
    bool isNull() const { return true; }
};
}
#endif

namespace hd {

class Scene : public Node {
public:
    Scene(const std::string& name = "", filament::Engine* engine = nullptr);
    virtual ~Scene() override = default;

    // 3D Transform Management (Local Space)
    void setTransform(const filament::math::mat4f& transform);
    filament::math::mat4f getTransform() const { return m_transform; }

    // Filament Entity Binding (Render Authority)
    void bindFilamentEntity(utils::Entity entity);
    utils::Entity getFilamentEntity() const { return m_filamentEntity; }

    // Synchronize Transform from USD authority to Filament renderer
    void syncTransformFromUSD();

    // Scene loading interface (mirrors traversing a UsdStage)
    void loadFromFile(const std::string& path);

protected:
    filament::math::mat4f m_transform;
    utils::Entity m_filamentEntity;
    filament::Engine* m_engine = nullptr;
};

} // namespace hd
