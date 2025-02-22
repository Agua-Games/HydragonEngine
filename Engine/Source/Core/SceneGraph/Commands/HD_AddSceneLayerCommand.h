/**
 * Copyright (c) 2024 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 */
#pragma once

#include "Core/NodeGraph/HD_CommandNode.h"
#include "Core/NodeGraph/HD_CommandValidation.h"
#include "Core/SceneGraph/HD_SceneNode.h"

namespace hd {

/**
 * @brief Factory function for creating an AddSceneLayer command node
 * @return Shared pointer to the command node
 */
inline std::shared_ptr<HD_CommandNode<HD_SceneNode*, std::string>> 
CreateAddSceneLayerCommand() {
    // Create validator
    auto validator = std::make_shared<HD_CommandValidator>();
    
    // Add validation rules
    validator->AddRule<std::string>("LayerPath",
        [](const std::string& path) { 
            return !path.empty() && path.ends_with(".usd"); 
        },
        "Layer path must be a non-empty USD file path"
    );

    // Create async command
    auto asyncCmd = [](HD_SceneNode* scene, const std::string& path) {
        return std::async(std::launch::async, [scene, path]() {
            // Simulate async operation
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
            scene->AddLayer(path);
        });
    };

    return MakeAsyncCommandNode<HD_SceneNode*, std::string>(
        "AddSceneLayer",
        asyncCmd,
        std::make_tuple("Scene", "LayerPath")
    );
}

} // namespace hd