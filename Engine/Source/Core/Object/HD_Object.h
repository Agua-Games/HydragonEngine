/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <memory>
#include <typeindex>
#include <functional>
#include <vector>

namespace hd {

/**
 * @brief Type-safe property wrapper for reflection system
 */
class HD_Property {
public:
    template<typename T>
    explicit HD_Property(T&& value) 
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
 * @brief Enhanced metadata and attributes for HD_Object
 */
struct HD_ObjectInfo {
    std::string Name;               
    std::string Description;        // Changed to capital D for consistency
    std::string Category;           // Added for object categorization
    std::string Version;           // Added for version tracking
    bool IsSerializable = true;     
    bool IsEditableInEditor = true; 
    bool IsProcedural = false;      // Added for procedural generation support
    bool IsStateless = false;       // Added for pure functional nodes
    
    // Core Properties (serialized state)
    std::unordered_map<std::string, HD_Property> Properties;  // Type-safe properties

    // Metadata Attributes (behavior/editor hints)
    std::unordered_map<std::string, std::any> Attributes;  // Renamed from CustomAttributes

    HD_ObjectInfo(
        const std::string& name = "",
        const std::string& category = "Default",
        bool isSerializable = true,
        bool isEditableInEditor = true,
        bool isProcedural = false,
        bool isStateless = false)
        : Name(name)
        , Category(category)
        , IsSerializable(isSerializable)
        , IsEditableInEditor(isEditableInEditor)
        , IsProcedural(isProcedural)
        , IsStateless(isStateless)
        , Version("1.0.0") {}
};

/**
 * @brief Enhanced base class for all objects in Hydragon
 */
class HD_Object {
public:
    explicit HD_Object(const HD_ObjectInfo& info) : Info(info) {}
    virtual ~HD_Object() = default;

    // Prevent copying but allow moving
    HD_Object(const HD_Object&) = delete;
    HD_Object& operator=(const HD_Object&) = delete;
    HD_Object(HD_Object&&) noexcept = default;
    HD_Object& operator=(HD_Object&&) noexcept = default;

    // Enhanced metadata access
    const HD_ObjectInfo& GetInfo() const { return Info; }
    
    // Type-safe property management
    template<typename T>
    void SetProperty(const std::string& name, T&& value) {
        Info.Properties[name] = HD_Property(std::forward<T>(value));
    }

    template<typename T>
    T GetProperty(const std::string& name) const {
        auto it = Info.Properties.find(name);
        if (it != Info.Properties.end()) {
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
    void AddDependency(const std::shared_ptr<HD_Object>& dep) {
        Dependencies.push_back(dep);
    }

    const std::vector<std::shared_ptr<HD_Object>>& GetDependencies() const {
        return Dependencies;
    }

    // Enhanced serialization with version support
    virtual void Serialize(std::ostream& stream) const {
        if (!Info.IsSerializable) return;
        
        // Write version and basic info
        WriteToStream(stream, Info.Version);
        WriteToStream(stream, Info.Name);
        
        // Serialize properties
        for (const auto& [name, prop] : Info.Properties) {
            WriteToStream(stream, name);
            WriteToStream(stream, prop.Get());
        }
    }

    virtual void Deserialize(std::istream& stream) {
        if (!Info.IsSerializable) return;
        
        // Read and verify version
        std::string version;
        ReadFromStream(stream, version);
        if (!IsVersionCompatible(version, Info.Version)) {
            throw std::runtime_error("Version mismatch during deserialization");
        }

        // Read basic info and properties
        ReadFromStream(stream, Info.Name);
        
        std::string propName;
        while (ReadFromStream(stream, propName)) {
            std::any value;
            ReadFromStream(stream, value);
            Info.Properties[propName].Set(value);
        }
    }

    // Enhanced logging with categories
    void Log(const std::string& message, const std::string& category = "Info") const {
        std::cout << "[" << category << "][" << Info.Name << "] " << message << std::endl;
    }

    // Enhanced debug info
    virtual void DebugInfo() const {
        std::cout << "=== Debug Info for " << Info.Name << " ===" << std::endl;
        std::cout << "Category: " << Info.Category << std::endl;
        std::cout << "Version: " << Info.Version << std::endl;
        std::cout << "Properties:" << std::endl;
        for (const auto& [name, prop] : Info.Properties) {
            std::cout << "  " << name << ": " << prop.Get().type().name() << std::endl;
        }
    }

    /**
     * @brief Creates a new instance of this object
     * @param deepCopy If true, also clones all dependencies
     * @return New instance with copied properties but unique identity
     */
    virtual std::shared_ptr<HD_Object> Clone(bool deepCopy = false) const {
        auto clone = std::make_shared<HD_Object>(Info);
        clone->Info.Properties = Info.Properties;  // Properties are copied
        clone->Info.Attributes = Info.Attributes;  // Attributes are copied
        
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
    virtual bool IsProcedural() const { return Info.IsProcedural; }

    // Helper method to check if content needs regeneration
    virtual bool NeedsRegeneration() const {
        return IsProcedural() && HasInputsChanged();
    }

protected:
    HD_ObjectInfo Info;
    std::vector<std::shared_ptr<HD_Object>> Dependencies;

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
};

} // namespace hd
