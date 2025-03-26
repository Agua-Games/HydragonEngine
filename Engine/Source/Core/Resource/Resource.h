/**
 * This is still a sketch, on how to design the resource classes, using Vulkan API, objects, encapsulating them with wrappers.
 * 
 * ARCHITECTURAL NOTES:
 * - Resource is a node that can load and manage resources.
 * - Like other nodes related to graphics, rendering, Resource loosely matches and tries to fit Vulkan concepts and objects. So, it is much like like a 
 * Vulkan raw data object, like VkImage, VkBuffer, VkShaderModule, etc.
 * 
 * TODO:
 * - Refactor the sketch to match the latest Object and Node design.
 */
#pragma once
#include <vulkan/vulkan.h>
#include <string>
#include <unordered_map>
#include "Node.h"
#include "Resource.h"

namespace hd {

struct ResourceInfo : public NodeInfo {
    ResourceInfo() {
        NodeType = "Resource";
        inputs = {
            "ResourcePath",     // Path to the resource
            "ResourceData",     // Resource data
            "ResourceMetadata"  // Resource metadata
        };
        outputs = {
            "LoadedResource",   // Loaded resource
            "ResourceStatus",   // Resource loading status
            "ResourceMetadata"  // Resource metadata
        }
    }
};

// Node (usage/view)
class Resource : public Node {
    public:
    explicit Resource(const ResourceInfo& info = ResourceInfo())
        : Node(info) {}
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
    ~Resource() = default;     // Default destructor
};

} // namespace hd