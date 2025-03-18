/**
 * This is still a sketch, on how to design the resource classes, using Vulkan API, objects, encapsulating them with wrappers.
 * There are still many design factors to consider, like:
 *      - We don't want to interfaces and implementations with "hardcoded" Vulkan commands, data types, scattered across the codebase.
 *      We want to avoid dependencies like these, keep things decoupled and 'watertight' as much as possible. So, a possible solution would
 *      be to keep all code files with Vulkan API inside of Core/Graphics/ or Core/Graphics/Vulkan, to facilitate future migration or addition
 *      of support to other APIs.
 */

// Resource (raw data)
class TextureResource : public Resource {
    // This is from first design sketch. Vulkan code should use proper wrappers
    VkImage image;
    VmaAllocation allocation;
    uint32_t width, height;
    VkFormat format;
};