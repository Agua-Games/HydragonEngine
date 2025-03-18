/**
 * This is still a sketch, on how to design the resource classes, using Vulkan API, objects, encapsulating them with wrappers.
 * 
 * ARCHITECTURAL NOTES:
 * - ResourceNode is a node that can load and manage resources.
 * - Like other nodes related to graphics, rendering, ResourceNode loosely matches and tries to fit Vulkan concepts and objects. So, it is much like like a 
 * Vulkan raw data object, like VkImage, VkBuffer, VkShaderModule, etc.
 * 
 * TODO:
 * - Refactor the sketch to match the latest Object and Node design.
 */
#pragma once
#include "Node.h"
#include "Resource.h"

namespace hd {

// Node (usage/view)
class ResourceNode : public Node {
    std::shared_ptr<Resource> resource;
    // This is from first design sketch. Vulkan code should use proper wrappers
    VkImage image;
    VkBuffer buffer;
    VkShaderModule shaderModule;
};

} // namespace hd