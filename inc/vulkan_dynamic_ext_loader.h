#ifndef VULKAN_DYNAMIC_EXT_LOADER
#define VULKAN_DYNAMIC_EXT_LOADER

#include <vulkan/vulkan.h>

// загрука функций vulkan
#define VK_DYNAMIC_LOAD_FUNC(VK_INST, FUNC) \
(FUNC = reinterpret_cast<PFN_##FUNC>(vkGetInstanceProcAddr(VK_INST, #FUNC)))

// Загрузка функции vulkan с возвратом результата
#define VK_DYNAMIC_LOAD_FUNC_CHECK(VK_INST, FUNC) \
(VK_DYNAMIC_LOAD_FUNC(VK_INST, FUNC) ? VK_SUCCESS : VK_ERROR_EXTENSION_NOT_PRESENT)

namespace Graphics
{
    // динамически загружаемые функции расширений

    // создание обработчки обратного вызова
    extern PFN_vkCreateDebugUtilsMessengerEXT vkCreateDebugUtilsMessengerEXT;

    // Отключение обработчика обратного вызова
    extern PFN_vkDestroyDebugUtilsMessengerEXT vkDestroyDebugUtilsMessengerEXT;
}

#endif // VULKAN_DYNAMIC_EXT_LOADER