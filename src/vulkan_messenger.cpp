#include "vulkan_messenger.h"

namespace Graphics
{
    VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_msg_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
        VkDebugUtilsMessageTypeFlagsEXT msg_type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        void *user_data
    )
    {
        std::cout << "validation layer: " << callback_data->pMessage << '\n';
        return VK_FALSE;
    }
}
