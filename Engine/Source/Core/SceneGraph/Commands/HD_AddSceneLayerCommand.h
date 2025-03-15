/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file AddSceneLayerCommand.h
 * @brief Header file for the AddSceneLayer command node.
 * 
 * ARCHITECTURAL NOTES:
 * - AddSceneLayer is a command node that adds a USD layer to a scene node.
 * 
 * TODO:
 * - Flesh out the class and its methods, structs, enums, etc.
 * - After design sketch phase and first use sessions, cleanup and tidy up again the whole content.
 * - Unify, cleanup, and refactor the code, to exactly match the design, architecture goals.
 * - Create .cpp file and move the implementation there.
 * - Organize the existing code into logical sections and functions.
 */
#pragma once

#include "CommandNode.h"
#include "CommandValidation.h"
#include "SceneNode.h"

namespace hd {

/**
 * @brief Factory function for creating an AddSceneLayer command node
 * @return Shared pointer to the command node
 */
inline std::shared_ptr<CommandNode<SceneNode*, std::string>> 
CreateAddSceneLayerCommand() {
    // Create validator
    auto validator = std::make_shared<CommandValidator>();
    
    // Add validation rules
    validator->AddRule<std::string>("LayerPath",
        [](const std::string& path) { 
            return !path.empty() && path.ends_with(".usd"); 
        },
        "Layer path must be a non-empty USD file path"
    );

    // Create async command
    auto asyncCmd = [](SceneNode* scene, const std::string& path) {
        return std::async(std::launch::async, [scene, path]() {
            // Simulate async operation
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            scene->AddLayer(path);
        });
    };

    return MakeAsyncCommandNode<SceneNode*, std::string>(
        "AddSceneLayer",
        asyncCmd,
        std::make_tuple("Scene", "LayerPath")
    );
}

} // namespace hd