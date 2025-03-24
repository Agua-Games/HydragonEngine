/**
 * This is still a sketch, on how to design the resource classes, using Vulkan API, objects, encapsulating them with wrappers.
 * 
 * ARCHITECTURAL NOTES:
 * - Resource is a node that can load and manage resources.
 * - Like other nodes related to graphics, rendering, Texture loosely matches and tries to fit Vulkan concepts and objects. So, it is much like like a 
 * VkImageView, VkSampler, VkImageLayout, etc.
 * 
 * TODO:
 * - Refactor the sketch to match the latest Object and Node design.
 */
#pragma once
#include "Node.h"
#include "Resource.h"

namespace hd {

// Node (usage/view)
class Texture : public Resource {
    std::shared_ptr<TextureResource> resource;
    VkImageView view;
    VkSampler sampler;
    VkImageLayout currentLayout;
    SamplerParameters params;
};

} // namespace hd