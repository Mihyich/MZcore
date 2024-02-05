#ifndef VULKAN_DEVICE_H
#define VULKAN_DEVICE_H

#include <vulkan/vulkan.h>
#include <stdio.h>
#include <string.h>

#define MAX_VK_PHYSICAL_DEVICE_TYPE_NAME 14

namespace Graphics
{
    void get_physical_device_type_name(char name[MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1], VkPhysicalDeviceType type);

    void output_physical_device(const VkPhysicalDevice *vpd);
}

#endif // VULKAN_DEVICE_H
