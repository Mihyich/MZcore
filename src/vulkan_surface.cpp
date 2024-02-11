#include "vulkan_surface.h"

namespace Graphics
{
    VkResult createWin32SurfaceKHR(VkInstance instance, VkSurfaceKHR *surface)
    {
        VkWin32SurfaceCreateInfoKHR createInfo = {};

        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hinstance = app::hInstance;
        createInfo.hwnd = app::game_wnd.get_hWnd();
        createInfo.pNext = nullptr;
        createInfo.flags = 0;

        return vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, surface);
    }
}
