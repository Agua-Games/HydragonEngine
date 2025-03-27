/**
 * Copyright (c) 2025 Agua Games. All rights reserved.
 * Licensed under the Agua Games License 1.0
 * 
 * @file ResourcePool.h
 * @brief ResourcePool is a class that manages a pool of resources. It is a type of Object.
 * 
 * ARCHITECTURAL NOTES:
 * - It provides a common interface for all resource pools in the engine, like textures, models, shaders, audio, fonts, etc.
 * - ResourcePool is a type of Object, so it can be serialized and deserialized.
 * - ResourcePool is a type of Node, so it can be connected to other nodes in the engine's node graph.
 */
#pragma once
#include "Resource.h"

namespace hd {
    struct ResourcePoolInfo : public ResourceInfo {
        ResourcePoolInfo() {
            nodeType = "Resource/ResourcePool";
            inputs = {
                "resourceType",   // Type of resource (texture, model, shader, audio, font, etc.)
                "resourcePath",   // Path to the resource
                "resourceData",   // Resource data
                "resourceMetadata"// Resource metadata
            };
            outputs = {
                "loadedResource", // Loaded resource
                "resourceStatus", // Resource loading status
                "resourceMetadata"// Resource metadata
            };
        }
    };

class ResourcePool : public Resource {
public:
    // === Allocation, Initialization, Loading ===
    explicit ResourcePool(const ResourcePoolInfo& info = ResourcePoolInfo())
        : Resource(info) {}
    initialize() override {}
    load() override {}

    // === Resource Management ===
    void loadResource(const std::string& resourcePath);
    void unloadResource(const std::string& resourcePath);
    void reloadResource(const std::string& resourcePath);

    // === Processing ===
    void processNode() override {
 
    }
    void update();

    // === Cleanup ===
    void unload() override {}
    void cleanup() override {}
    ~ResourcePool() = default;     // Default destructor
};

} // namespace hd


