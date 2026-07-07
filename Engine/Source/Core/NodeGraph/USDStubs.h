// USDStubs.h
//
// Lightweight stubs simulating the Pixar OpenUSD (pxr) library structures.
// Allows compiling the C++ node/scene architecture without linking the full USD SDK.

#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <vector>
#include <math/mat4.h>

namespace pxr {

class UsdStage;

class UsdPrim {
public:
    UsdPrim() = default;
    UsdPrim(const std::string& path, const std::string& typeName = "")
        : m_path(path), m_typeName(typeName) {}

    bool IsValid() const { return !m_path.empty(); }
    operator bool() const { return IsValid(); }

    std::string GetPath() const { return m_path; }
    std::string GetTypeName() const { return m_typeName; }

    template<typename T>
    void SetAttribute(const std::string& name, const T& value) {
        m_attributes[name] = value;
    }

    template<typename T>
    T GetAttribute(const std::string& name, const T& defaultValue = T()) const {
        auto it = m_attributes.find(name);
        if (it != m_attributes.end()) {
            try {
                return std::any_cast<T>(it->second);
            } catch (...) {}
        }
        return defaultValue;
    }

    void SetTransform(const filament::math::mat4f& transform) {
        m_transform = transform;
    }

    filament::math::mat4f GetTransform() const {
        return m_transform;
    }

private:
    std::string m_path;
    std::string m_typeName;
    std::unordered_map<std::string, std::any> m_attributes;
    filament::math::mat4f m_transform = filament::math::mat4f();
};

class UsdStage {
public:
    static std::shared_ptr<UsdStage> Open(const std::string& filePath) {
        auto stage = std::make_shared<UsdStage>();
        stage->m_filePath = filePath;
        stage->m_rootPrim = UsdPrim("/", "StageRoot");
        return stage;
    }

    UsdPrim GetPseudoRoot() const {
        return m_rootPrim;
    }

    UsdPrim DefinePrim(const std::string& path, const std::string& typeName = "") {
        UsdPrim prim(path, typeName);
        m_prims[path] = prim;
        return prim;
    }

    UsdPrim GetPrimAtPath(const std::string& path) const {
        auto it = m_prims.find(path);
        if (it != m_prims.end()) {
            return it->second;
        }
        if (path == "/") return m_rootPrim;
        return UsdPrim();
    }

private:
    std::string m_filePath;
    UsdPrim m_rootPrim;
    std::unordered_map<std::string, UsdPrim> m_prims;
};

} // namespace pxr
