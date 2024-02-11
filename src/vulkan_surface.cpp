#include "vulkan_surface.h"

namespace Graphics
{
    VkResult createWin32SurfaceKHR(VkInstance instance, VkSurfaceKHR *surface, std::string *err_msg)
    {
        VkResult err = VK_SUCCESS;
        VkWin32SurfaceCreateInfoKHR createInfo = {};

        createInfo.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;
        createInfo.hinstance = app::hInstance;
        createInfo.hwnd = app::game_wnd.get_hWnd();
        createInfo.pNext = nullptr;
        createInfo.flags = 0;

        err = vkCreateWin32SurfaceKHR(instance, &createInfo, nullptr, surface);

        if (err && err_msg)
        {
            err_msg->assign("Error from vkCreateWin32SurfaceKHR()\n");
            (*err_msg) += "Err_code: ";
            (*err_msg) += std::to_string(err);
        }

        return err;
    }
}
