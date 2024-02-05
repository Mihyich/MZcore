#ifndef VULKAN_MESSENGER_H
#define VULKAN_MESSENGER_H

#include <vulkan/vulkan.h>
#include <iostream>

namespace Graphics
{
    // обратный вызов vulkan для отладки
    VKAPI_ATTR VkBool32 VKAPI_CALL vk_debug_msg_callback(
        VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
        VkDebugUtilsMessageTypeFlagsEXT msg_type,
        const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
        void *user_data
    );
}

#endif // VULKAN_MESSENGER_H
