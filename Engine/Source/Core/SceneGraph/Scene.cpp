// Scene.cpp
//
// Implementation of the 3D Scene Node with OpenUSD and Filament synchronization.

#include "Scene.h"

#if defined(HYDRAGON_USE_FILAMENT)
#include <filament/Engine.h>
#include <filament/TransformManager.h>
#endif

namespace hd {

Scene::Scene(const std::string& name, filament::Engine* engine)
    : Node(name)
    , m_transform(filament::math::mat4f())
    , m_engine(engine) {
}

void Scene::setTransform(const filament::math::mat4f& transform) {
    m_transform = transform;
    
    // Propagate to USD data authority
    if (m_usdPrim) {
        m_usdPrim.SetTransform(transform);
    }
    
    // Propagate to Filament render authority
#if defined(HYDRAGON_USE_FILAMENT)
    if (m_engine && !m_filamentEntity.isNull()) {
        auto& tcm = m_engine->getTransformManager();
        auto instance = tcm.getInstance(m_filamentEntity);
        if (instance) {
            tcm.setTransform(instance, transform);
        }
    }
#endif
}

void Scene::bindFilamentEntity(utils::Entity entity) {
    m_filamentEntity = entity;
    
    // Initialize the entity's transform in Filament immediately
#if defined(HYDRAGON_USE_FILAMENT)
    if (m_engine && !m_filamentEntity.isNull()) {
        auto& tcm = m_engine->getTransformManager();
        auto instance = tcm.getInstance(m_filamentEntity);
        if (instance) {
            tcm.setTransform(instance, m_transform);
        }
    }
#endif
}

void Scene::syncTransformFromUSD() {
    if (!m_usdPrim) return;

    // 1. Pull transform from USD Prim (data authority)
    m_transform = m_usdPrim.GetTransform();

    // 2. Push transform to Filament (render authority)
#if defined(HYDRAGON_USE_FILAMENT)
    if (m_engine && !m_filamentEntity.isNull()) {
        auto& tcm = m_engine->getTransformManager();
        auto instance = tcm.getInstance(m_filamentEntity);
        if (instance) {
            tcm.setTransform(instance, m_transform);
        }
    }
#endif
}

void Scene::loadFromFile(const std::string& path) {
    // 1. Simulate opening the USD Stage
    auto stage = pxr::UsdStage::Open(path);
    if (!stage) return;

    // 2. Traversal Simulation: Read USD primitive definitions
    auto meshPrim = stage->DefinePrim("/MeshPlaceholder", "Mesh");
    
    // Set a dummy initial transform in USD
    filament::math::mat4f initialTransform = filament::math::mat4f::translation(filament::math::float3{0.0f, 2.0f, 0.0f});
    meshPrim.SetTransform(initialTransform);
    meshPrim.SetAttribute<float>("roughness", 0.35f);

    // 3. Mirror the USD primitive into our Scene Graph
    auto childScene = std::make_shared<Scene>("MeshPlaceholder", m_engine);
    childScene->bindUsdPrim(meshPrim);
    childScene->syncTransformFromUSD();

    addChild(childScene);
}

} // namespace hd
