#include "vulkan_device.h"

namespace Graphics
{
    void get_physical_device_type_name(char name[MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1], VkPhysicalDeviceType type)
    {
        switch (type)
        {
        case VK_PHYSICAL_DEVICE_TYPE_OTHER:
            strncpy(name, "OTHER", MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1);
            break;

        case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU:
            strncpy(name, "INTEGRATED_GPU", MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1);
            break;

        case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU:
            strncpy(name, "DISCRETE_GPU", MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1);
            break;

        case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
            strncpy(name, "VIRTUAL_GPU", MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1);
            break;

        case VK_PHYSICAL_DEVICE_TYPE_CPU:
            strncpy(name, "CPU", MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1);
            break;

        default:
            strncpy(name, "UNKNOWN", MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1);
            break;
        }
    }

    void output_physical_device(const VkPhysicalDevice *vpd)
    {
        if (!vpd)
        {
            return;
        }

        VkPhysicalDeviceProperties pdp;
        char type_name[MAX_VK_PHYSICAL_DEVICE_TYPE_NAME + 1];

        vkGetPhysicalDeviceProperties(*vpd, &pdp);
        get_physical_device_type_name(type_name, pdp.deviceType);

        printf("\tDevice Name: %s\n", pdp.deviceName);
        printf("\tDevice Type: %s\n", type_name);
        printf("\tAPI Version: %u.%u.%u\n",
            VK_VERSION_MAJOR(pdp.apiVersion),
            VK_VERSION_MINOR(pdp.apiVersion),
            VK_VERSION_PATCH(pdp.apiVersion));

        // Другие свойства...
    }

    VkResult check_device_extension_support(VkPhysicalDevice *vpd, const char **req_ext, size_t count)
    {
        VkResult err = VK_SUCCESS;
        uint32_t extensionCount = 0;
        std::vector<VkExtensionProperties> extensions;

        err = vkEnumerateDeviceExtensionProperties(*vpd, nullptr, &extensionCount, nullptr);

        if (!err)
        {
            extensions.resize(extensionCount);
            err = vkEnumerateDeviceExtensionProperties(*vpd, nullptr, &extensionCount, extensions.data());
        }

        if (!err)
        {
            bool exist;

            for (size_t i = 0; i < count; ++i)
            {
                exist = false;

                for (VkExtensionProperties aext : extensions)
                {
                    if (strcmp(*(req_ext + i), aext.extensionName) == 0)
                    {
                        exist = true;
                        break;
                    }
                }

                if (!exist)
                {
                    err = VK_ERROR_EXTENSION_NOT_PRESENT;
                    break;
                }
            }
        }

        return err;
    }
}
