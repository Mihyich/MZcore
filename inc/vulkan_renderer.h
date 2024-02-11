#ifndef VULKAN_RENDERER_H
#define VULKAN_RENDERER_H

#include <vulkan/vulkan.h>
#include <string>
#include <vector>
#include <stdio.h>
#include "definers.h"
#include "vulkan_dynamic_ext_loader.h"
#include "vulkan_device.h"
#include "vulkan_messenger.h"
#include "vulkan_surface.h"

namespace Graphics
{
    struct QueueFamilyIndex
    {
        uint32_t fi = 0; // индекс семейства очереди
        uint32_t qi = 0; // индекс очереди в семействе
        bool fset = false; // был ли получен индекс семейства очереди
        bool qset = false; // был ли получен индекс очереди в семействе
    };

    class Vulkan_Renderer
    {
    private:
    	uint32_t version = 0; // версия вулкана
    	VkInstance instance = VK_NULL_HANDLE; // экземпляр Vulkan
    	VkApplicationInfo appInfo = {}; // информация о приложении для Vulkan
    	VkInstanceCreateInfo createInfo = {}; // структура для создания экземпляра Vulkan
        VkDebugUtilsMessengerEXT dbg_msg = VK_NULL_HANDLE; // отладодчный вывод Vulkan

    	std::vector<VkExtensionProperties> extensions; // расширения
        std::vector<VkLayerProperties> layers; // слои
    	std::vector<VkPhysicalDevice> physical_devices; // физические устройства
        VkPhysicalDevice physical_device = VK_NULL_HANDLE; // выбранное физическое устройство
        std::vector<VkQueueFamilyProperties> physical_device_queue_family_props; // семейcтва очередей выбранного ф. у.
        QueueFamilyIndex queue_family; // выбранное семейство очереди на физическом устройстве (индекс)
        VkDevice device = VK_NULL_HANDLE; // выбранное логическое устройство
        VkQueue queue = VK_NULL_HANDLE; // очередь
        VkSurfaceKHR surface; // поверхность рисования (Переходник Vulkan <--> Система)

    	// информация о последней ошибке
    	std::string error_report;

    public:

        // запрашиваемые расширения экземпляра vulkan
        std::vector<const char*> req_extensions;

        // запрашиваемые слои экземпляра vulkan
        std::vector<const char*> req_layers;

        // запрашиваемые расширения логического устройства
        std::vector<const char*> req_device_extensions;

        // получить отчет о последней ошибке
        std::string *get_error_report(void);

        // инициализация vulkan
    	VkResult init(bool user_extensions = false, bool user_layers = false, bool debug = false);

    private:

        void set_std_instance_extensions(bool debug);

        void set_std_instance_layers(bool debug);

        VkResult enumerate_instance_extensions_properties(void);

    	VkResult enumerate_instance_layers_properties(void);

        bool is_extension_supported(const char *extension) const;

        VkResult check_extensions_support(void);

        bool is_layer_supported(const char *layers) const;

        VkResult check_layers_support(void);

        VkResult find_instance_version(void);

        VkResult create_instance(void);

        VkResult load_extensions(void);

        VkResult create_debug_utils_messenger_ext(void);

        VkResult enumerate_physical_devices(void);

        void set_std_devise_extensions(void);

        VkResult choosing_physical_device(void);

        VkResult get_physical_device_queue_family_properties(void);

        VkResult choosing_phisical_device_queue_family(void);

        VkResult create_logical_device(void);

        VkResult get_queue(void);

    public:

    	void output_version(void) const;

    	void output_instance_layers_properties(void) const;

    	void output_instance_extensions_preperties(void) const;

    	void output_physical_devices(void) const;

        void output_choosen_physical_device(void) const;

        void output_physical_device_queue_family_properties(void) const;

        VkLayerProperties *get_instance_layers_properties_arr(size_t *lenght);

        VkExtensionProperties *get_instance_extensions_properties_arr(size_t *lenght);

        VkPhysicalDevice *get_physical_devices_arr(size_t *lenght);

    	~Vulkan_Renderer();

    private:
    	void gen_report_error(const char *func_name, const char *description, int err_code = VK_SUCCESS);
    };

    extern bool debugging;
    extern Vulkan_Renderer vk_renderer;
}

#endif // VULKAN_RENDERER_H
