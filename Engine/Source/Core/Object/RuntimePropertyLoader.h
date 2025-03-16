/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file RuntimePropertyLoader.h
 * @brief Header file for the RuntimePropertyLoader class.
 * 
 * ARCHITECTURAL NOTES:
 * - RuntimePropertyLoader is a utility class for loading and updating runtime properties from JSON/YAML files.
 * 
 * TODO:
 * - Move from this bare design sketch to a full implementation.
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 */
#pragma once

class RuntimePropertyLoader {
public:
    static void LoadFromFile(Object* obj, const std::string& filepath) {
        // Load property values from JSON/YAML file
        // and update them at runtime
        auto values = LoadJSON(filepath);
        for (const auto& [name, value] : values.items()) {
            if (obj->GetRuntimeProperties().contains(name)) {
                // Update property based on type...
                UpdatePropertyFromJSON(obj, name, value);
            }
        }
    }
};