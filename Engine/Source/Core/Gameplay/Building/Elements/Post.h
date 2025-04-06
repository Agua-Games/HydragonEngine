/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file Post.h
 * @brief Header file for the Post class.
 * 
 * ARCHITECTURAL NOTES:
 * - Post is a class that represents a post in Hydragon.
 * - It is used to represent any post in the game world, such as sign posts, lamp posts, etc.
 * - It supports interactive features, such as entering, exiting, and interacting with. Also supports two-way messaging with other posts, environment, character, etc.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <vector>
#include <unordered_map>
#include "BuildingElement.h"
#include "DataTable.h"

namespace hd {

struct PostInfo : public BuildingElementInfo {
    PostInfo() {
        nodeType = "Gameplay/Post"; 
        
        inputs = {
            "postType",  // Type of post (sign post, lamp post, etc.)
            "postData",  // Post data
            "environment", // Environment data
            "characterData", // Character data
            "postState" // Post state
        };
        
        outputs = {
            "postStatus", // Post status
            "postMetrics" // Post performance metrics
        };
    }
};

class Post : public BuildingElement {
public:
    // === Allocation, Initialization, Loading ===
    explicit Post(const PostInfo& info = PostInfo())
        : BuildingElement(info) {}
    initialize() override {}
    load() override {}

    // === Processing ===
    void processNode() override {
    
    }    
    void enter();
    void exit();
};

} // namespace hd
