#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <string>
#include <string.h>
#include <vector>
#include <list>
#include <iterator>

#define MAX_VK_PHYSICAL_DEVICE_TYPE_NAME 14

namespace Graphics
{
    void get_physical_device_type_name(char name[MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1], VkPhysicalDeviceType type);

    void output_physical_device(const VkPhysicalDevice *vpd);

    void output_queue_falgs(VkQueueFlags vqf);

    void output_physical_device_queue_family_prop(const VkQueueFamilyProperties *vqfp);

    VkResult check_device_extension_support(VkPhysicalDevice *vpd, const char **req_ext, size_t count);
}

#endif // VULKAN_DEVICE_H
