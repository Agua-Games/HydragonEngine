/** 
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once
#include <string>
#include <unordered_map>
#include <any>
#include <memory>

/**
 * @brief Base class for all objects in Hydragon.
 * @details Provides universally useful functionality, like reflection, 
 * serialization, and logging utilities, among others.
 */
namespace hd {

    // Struct to hold metadata and attributes for HD_Object
    struct HD_ObjectInfo {
        std::string Name;               // Optional name for debugging or identification
        bool IsSerializable = true;     // Whether the object supports serialization
        bool IsEditableInEditor = true; // Whether the object's properties are exposed to the editor
        std::unordered_map<std::string, std::any> CustomAttributes; // User-defined attributes

        // Constructor for convenience
        HD_ObjectInfo(const std::string& name = "", bool isSerializable = true, bool isEditableInEditor = true)
            : Name(name), IsSerializable(isSerializable), IsEditableInEditor(isEditableInEditor) {}
    };

    // Base class for all objects in Hydragon
    class HD_Object {
    public:
        // Constructor with HD_ObjectInfo for initialization
        explicit HD_Object(const HD_ObjectInfo& info) : Info(info) {}

        // Virtual destructor for proper cleanup of derived classes
        virtual ~HD_Object() = default;

        // Getters for metadata
        const HD_ObjectInfo& GetInfo() const { return Info; }

        // Reflection mechanism (can be overridden by derived classes)
        virtual void Reflect() {}

        // Serialization (can be overridden by derived classes)
        virtual void Serialize(std::ostream& stream) const {
            // Default implementation (can be extended by derived classes)
        }

        virtual void Deserialize(std::istream& stream) {
            // Default implementation (can be extended by derived classes)
        }

        // Logging utility
        void Log(const std::string& message) const {
            std::cout << "[HD_Object: " << Info.Name << "] " << message << std::endl;
        }

        // Debugging utility
        virtual void DebugInfo() const {
            std::cout << "Debug info for HD_Object: " << Info.Name << std::endl;
        }

    protected:
        HD_ObjectInfo Info; // Metadata and attributes for the object
    };

} // namespace hd