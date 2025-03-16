/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Object.h
 * @brief Enhanced base class for all objects in Hydragon
 * 
 * ARCHITECTURAL NOTES:
 * - Object is the fundamental building block in the engine's object-oriented architecture.
 * - It provides enhanced metadata, reflection, and serialization capabilities.
 * - It supports versioning, dependency management, and enhanced logging.
 * - It supports procedural generation and stateless objects.
 * 
 * TODO:
 * - Implement base caching (moved from Node to Object, meant to be overriden in Node)
 * - Fix, unify capital letters for variables - should start with lowercase.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 */
#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <typeindex>
#include <functional>
#include <vector>
#include "Property.h"

namespace hd {

/**
 * @brief Type-safe property wrapper for reflection system
 */
class Property {
public:
    template<typename T>
    explicit Property(T&& value) 
        : value(std::forward<T>(value))
        , typeInfo(std::type_index(typeid(T)))
        , getter([this]() -> const std::any& { return value; })
        , setter([this](const std::any& v) { value = std::any_cast<T>(v); }) {}

    const std::type_index& GetType() const { return typeInfo; }
    const std::any& Get() const { return getter(); }
    void Set(const std::any& v) { setter(v); }

private:
    std::any value;
    std::type_index typeInfo;
    std::function<const std::any&()> getter;
    std::function<void(const std::any&)> setter;
};

/**
 * @brief Enhanced metadata and attributes for Object
 */
struct ObjectInfo {
    std::string name;
    std::string description;
    std::string category;
    std::string version;
    bool isSerializable = true;
    bool isEditableInEditor = true;
    bool isProcedural = false;
    bool isStateless = false;
    
    // Core Properties (serialized state)
    std::unordered_map<std::string, Property> properties;  // Type-safe properties

    // Metadata Attributes (behavior/editor hints)
    std::unordered_map<std::string, std::any> attributes;

    ObjectInfo(
        const std::string& name = "",
        const std::string& category = "Default",
        bool isSerializable = true,
        bool isEditableInEditor = true,
        bool isProcedural = false,
        bool isStateless = false)
        : name(name)
        , category(category)
        , isSerializable(isSerializable)
        , isEditableInEditor(isEditableInEditor)
        , isProcedural(isProcedural)
        , isStateless(isStateless)
        , version("1.0.0") {}
};

/**
 * @brief Enhanced base class for all objects in Hydragon
 */
class Object {
public:
    explicit Object(const ObjectInfo& info) : info(info) {}
    virtual ~Object() = default;

    // Prevent copying but allow moving
    Object(const Object&) = delete;
    Object& operator=(const Object&) = delete;
    Object(Object&&) noexcept = default;
    Object& operator=(Object&&) noexcept = default;

    // Enhanced metadata access
    const ObjectInfo& GetInfo() const { return info; }
    
    // Type-safe property management
    template<typename T>
    void SetProperty(const std::string& name, T&& value) {
        info.properties[name] = Property(std::forward<T>(value));
    }

    template<typename T>
    T GetProperty(const std::string& name) const {
        auto it = info.properties.find(name);
        if (it != info.properties.end()) {
            return std::any_cast<T>(it->second.Get());
        }
        throw std::runtime_error("Property not found: " + name);
    }

    // Enhanced reflection system
    virtual void Reflect() {
        ReflectProperties();
        ReflectAttributes();
    }

    // Dependency management
    void AddDependency(const std::shared_ptr<Object>& dep) {
        Dependencies.push_back(dep);
    }

    const std::vector<std::shared_ptr<Object>>& GetDependencies() const {
        return Dependencies;
    }

    // Enhanced serialization with version support
    virtual void Serialize(std::ostream& stream) const {
        if (!info.isSerializable) return;
        
        // Write version and basic info
        WriteToStream(stream, info.version);
        WriteToStream(stream, info.name);
        
        // Serialize properties
        for (const auto& [name, prop] : info.properties) {
            WriteToStream(stream, name);
            WriteToStream(stream, prop.Get());
        }
    }

    virtual void Deserialize(std::istream& stream) {
        if (!info.isSerializable) return;
        
        // Read and verify version
        std::string version;
        ReadFromStream(stream, version);
        if (!IsVersionCompatible(version, info.version)) {
            throw std::runtime_error("Version mismatch during deserialization");
        }

        // Read basic info and properties
        ReadFromStream(stream, info.name);
        
        std::string propName;
        while (ReadFromStream(stream, propName)) {
            std::any value;
            ReadFromStream(stream, value);
            info.properties[propName].Set(value);
        }
    }

    // Enhanced logging with categories
    void Log(const std::string& message, const std::string& category = "Info") const {
        std::cout << "[" << category << "][" << info.name << "] " << message << std::endl;
    }

    // Enhanced debug info
    virtual void DebugInfo() const {
        std::cout << "=== Debug Info for " << info.name << " ===" << std::endl;
        std::cout << "Category: " << info.category << std::endl;
        std::cout << "Version: " << info.version << std::endl;
        std::cout << "Properties:" << std::endl;
        for (const auto& [name, prop] : info.properties) {
            std::cout << "  " << name << ": " << prop.Get().type().name() << std::endl;
        }
    }

    /**
     * @brief Creates a new instance of this object
     * @param deepCopy If true, also clones all dependencies
     * @return New instance with copied properties but unique identity
     */
    virtual std::shared_ptr<Object> Clone(bool deepCopy = false) const {
        auto clone = std::make_shared<Object>(info);
        clone->info.properties = info.properties;  // Properties are copied
        clone->info.attributes = info.attributes;  // Attributes are copied
        
        if (deepCopy) {
            for (const auto& dep : Dependencies) {
                clone->Dependencies.push_back(dep->Clone(true));
            }
        }
        return clone;
    }

    /**
     * @brief Determines if this object generates content procedurally
     * 
     * Override this in derived classes to indicate procedural behavior.
     * Procedural objects may:
     * - Generate content at runtime
     * - Have different results based on input parameters
     * - Need special handling for caching/baking
     * 
     * @return true if object generates content procedurally
     */
    virtual bool IsProcedural() const { return info.isProcedural; }

    // Helper method to check if content needs regeneration
    virtual bool NeedsRegeneration() const {
        return IsProcedural() && HasInputsChanged();
    }

    /**
     * @brief Validates object state and connections
     */
    virtual bool Validate() const {
        // Check property validity
        for (const auto& [id, prop] : properties) {
            if (!ValidateProperty(id, prop)) return false;
        }

        // Check dependency validity
        for (const auto& dep : dependencies) {
            if (auto ptr = dep.lock()) {
                if (!ptr->Validate()) return false;
            } else {
                return false; // Invalid weak_ptr
            }
        }

        return true;
    }

protected:
    ObjectInfo info;
    std::vector<std::shared_ptr<Object>> Dependencies;

    virtual void ReflectProperties() {}
    virtual void ReflectAttributes() {}

private:
    // Utility functions for serialization
    template<typename T>
    static void WriteToStream(std::ostream& stream, const T& value) {
        stream.write(reinterpret_cast<const char*>(&value), sizeof(T));
    }

    template<typename T>
    static bool ReadFromStream(std::istream& stream, T& value) {
        return stream.read(reinterpret_cast<char*>(&value), sizeof(T)).good();
    }

    static bool IsVersionCompatible(const std::string& v1, const std::string& v2) {
        // Implement version compatibility check
        return v1 == v2; // Simplified for now
    }

    bool ValidateProperty(PropertyId id, const std::unique_ptr<PropertyBase>& prop) const {
        if (!prop) return false;
        
        // Additional property validation logic
        const auto& info = PropertyRegistry::GetInfo(id);
        return prop->GetType() == info.type;
    }
};

} // namespace hd