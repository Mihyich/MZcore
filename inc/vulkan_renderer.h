#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "definers.h"

namespace Graphics
{
    class Vulkan_Renderer
    {
    private:
    	uint32_t version = 0; // версия вулкана
    	VkInstance instance = VK_NULL_HANDLE; // экземпляр Vulkan
    	VkApplicationInfo appInfo = {}; // информация о приложении для Vulkan
    	VkInstanceCreateInfo createInfo = {}; // структура для создания экземпляра Vulkan

    	std::vector<VkLayerProperties> layers; // слои
    	std::vector<VkExtensionProperties> extensions; // расширения
    	std::vector<VkPhysicalDevice> physical_devices; // физические устройства
    	std::vector<VkQueueFamilyProperties> queue_family_props; // семейтва очередей

    	// информация о последней ошибке
    	std::string error_report;

    public:

    	int init();

    	int enumerate_instance_layers_properties(void);

    	int enumerate_instance_extensions_properties(void);

    	int enumerate_physical_devices(void);

    	// int

    	std::string *get_error_report(void);

    	void output_version(void) const;

    	void output_instance_layers_properties(void) const;

    	void output_instance_extensions_preperties(void) const;

    	void output_physical_devices(void) const;

    	~Vulkan_Renderer();

    private:
    	void gen_report_error(const char *func_name, const char *description, int err_code = VK_SUCCESS);
    };
}
#endif // VULKAN_RENDERER_H
