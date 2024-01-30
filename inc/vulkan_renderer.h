#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "definers.h"
#include "vulkan_device.h"

namespace Graphics
{
    struct QueueFamilyIndices
    {
        std::vector<uint32_t> graphicsFamily;
        std::vector<uint32_t> presentFamily;
    };

    bool is_queuefamilyindices_complete(const QueueFamilyIndices *qfi);

    class Vulkan_Renderer
    {
    private:
    	uint32_t version = 0; // версия вулкана
    	VkInstance instance = VK_NULL_HANDLE; // экземпляр Vulkan
    	VkApplicationInfo appInfo = {}; // информация о приложении для Vulkan
    	VkInstanceCreateInfo createInfo = {}; // структура для создания экземпляра Vulkan

    	std::vector<VkExtensionProperties> extensions; // расширения
        std::vector<VkLayerProperties> layers; // слои
    	std::vector<VkPhysicalDevice> physical_devices; // физические устройства
    	std::vector<VkQueueFamilyProperties> queue_family_props; // семейтва очередей

        VkPhysicalDevice phycical_device = VK_NULL_HANDLE; // выбранное физическиле устройство

    	// информация о последней ошибке
    	std::string error_report;

    public:

        // запрашиваемые расширения
        std::vector<const char*> req_extensions;

        // запрашиваемые слои
        std::vector<const char*> req_layers;

        std::string *get_error_report(void);

    	VkResult init();

    private:

    	VkResult enumerate_instance_layers_properties(void);

    	VkResult enumerate_instance_extensions_properties(void);

    	VkResult enumerate_physical_devices(void);

        bool is_layer_supported(const char *layers) const;

        bool is_extension_supported(const char *extension) const;

    	VkResult get_physical_device_queue_family_properties(const VkPhysicalDevice device);

    public:

    	void output_version(void) const;

    	void output_instance_layers_properties(void) const;

    	void output_instance_extensions_preperties(void) const;

    	void output_physical_devices(void) const;

        VkLayerProperties *get_instance_layers_properties_arr(size_t *lenght);

        VkExtensionProperties *get_instance_extensions_properties_arr(size_t *lenght);

        VkPhysicalDevice *get_physical_devices_arr(size_t *lenght);

    	~Vulkan_Renderer();

    private:
    	void gen_report_error(const char *func_name, const char *description, int err_code = VK_SUCCESS);
    };
}

#endif // VULKAN_RENDERER_H
