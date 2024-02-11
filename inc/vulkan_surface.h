#ifndef VULKAN_SURFACE_H
#define VULKAN_SURFACE_H

#define VK_USE_PLATFORM_WIN32_KHR
#include <windows.h>
#include <vulkan/vulkan.h>
#include <string>
#include "app_args.h"

namespace Graphics
{
    VkResult createWin32SurfaceKHR(VkInstance instance, VkSurfaceKHR *surface, std::string *err_msg = nullptr);
}

#endif // VULKAN_SURFACE_H
