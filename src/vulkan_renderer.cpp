#include "vulkan_renderer.h"

std::string *Graphics::Vulkan_Renderer::get_error_report(void)
{
    return &this->error_report;
}

VkResult Graphics::Vulkan_Renderer::init(bool user_extensions, bool user_layers, bool debug)
{
    VkResult err = VK_SUCCESS;

    // поиск доступных расширений
    if (!err)
        err = enumerate_instance_extensions_properties();

    // если пользовательские расширения не введены - использовать по умолчанию
    if (!err && !user_extensions)
        set_std_instance_extensions(debug);

    // Проверка поддержки запрашиваемых расширений
    if (!err)
        err = check_extensions_support();

    // поиск доступных слоев
    if (!err)
        err = enumerate_instance_layers_properties();

    // если пользовательские слои не введены - использовать по умолчанию
    if (!err && !user_layers)
        set_std_instance_layers(debug);

    // Проверка поддержки запрашиваемых слоев
    if (!err)
        err = check_layers_support();

    // поиск доуступной версии экземпляра вулкан
    if (!err)
        err = find_instance_version();

    // создание экземпляра
    if (!err)
        err = create_instance();

    // загрузка расширений vulkan
    if (!err)
        err = load_extensions();

    // создание отладочного вывода vulkan
    if (!err && debug)
        err = create_debug_utils_messenger_ext();

    // создание поверхности рисования
    if (!err)
        err = create_surface();

    // поиск доступных устройств
    if (!err)
        err = enumerate_physical_devices();

    // выбор устройства поддерживающего необходимые расширения и слой
    if (!err)
    {
        set_std_devise_extensions();
        err = choosing_physical_device();
    }

    // поиск семейств очередей у ф. у.
    if (!err)
        err = get_physical_device_queue_family_properties();

    // выбор определеннеого семейства ф. у.
    if (!err)
        err = choosing_phisical_device_queue_family();

    // создание логического устройства
    if (!err)
        err = create_logical_device();

    // получение очереди л. у.
    if (!err)
        err = get_queue();

    return err;
}

void Graphics::Vulkan_Renderer::set_std_instance_extensions(bool debug)
{
    this->req_extensions.clear();
    this->req_extensions.push_back("VK_KHR_surface");
    this->req_extensions.push_back("VK_KHR_win32_surface");

    if (debug)
    {
        this->req_extensions.push_back("VK_EXT_debug_utils");
    }
}

void Graphics::Vulkan_Renderer::set_std_instance_layers(bool debug)
{
    if (debug)
    {
        this->req_layers.push_back("VK_LAYER_KHRONOS_validation");
    }
}

VkResult Graphics::Vulkan_Renderer::enumerate_instance_extensions_properties(void)
{
    VkResult err = VK_SUCCESS;
    uint32_t extension_count = 0;

    this->extensions.clear();

    if ((err = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr)) != VK_SUCCESS)
        this->gen_report_error("vkEnumerateInstanceExtensionProperties", nullptr, err);

    if (!err && !extension_count)
    {
        err = VkResult::VK_INCOMPLETE; // самый подходящий код возврата, который я смог найти
        this->gen_report_error("vkEnumerateInstanceExtensionProperties",
                               "No Vulkan instance extensions available",
                               err);
    }

    if (!err)
    {
        this->extensions.resize(extension_count);

        if ((err = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, this->extensions.data())) != VK_SUCCESS)
            this->gen_report_error("vkEnumerateInstanceExtensionProperties", nullptr, err);
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::enumerate_instance_layers_properties(void)
{
    VkResult err = VK_SUCCESS;
    uint32_t layers_count = 0;

    this->layers.clear();

    if ((err = vkEnumerateInstanceLayerProperties(&layers_count, nullptr)) != VK_SUCCESS)
        this->gen_report_error("vkEnumerateInstanceExtensionProperties", nullptr, err);

    if (!err && !layers_count)
    {
        err = VkResult::VK_INCOMPLETE; // самый подходящий код возврата, который я смог найти
        this->gen_report_error("vkEnumerateInstanceLayerProperties",
                               "No Vulkan instance layers available",
                               err);
    }

    if (!err)
    {
        this->layers.resize(layers_count);

        if ((err = vkEnumerateInstanceLayerProperties(&layers_count, this->layers.data())) != VK_SUCCESS)
            this->gen_report_error("vkEnumerateInstanceExtensionProperties", nullptr, err);
    }

    return err;
}

bool Graphics::Vulkan_Renderer::is_extension_supported(const char *extension) const
{
    for (size_t i = 0; i < this->extensions.size(); ++i)
        if (strcmp(extension, this->extensions[i].extensionName) == 0)
            return true;
    return false;
}

VkResult Graphics::Vulkan_Renderer::check_extensions_support(void)
{
    VkResult err = VK_SUCCESS;
    std::string tmp_str;

    for (auto req_ext : this->req_extensions)
    {
        if (!is_extension_supported(req_ext))
        {
            tmp_str = "Extension \"";
            tmp_str += req_ext;
            tmp_str += "\" is not existing";

            this->gen_report_error(
                "is_extension_supported",
                tmp_str.data(),
                err);

            err = VK_INCOMPLETE;
            break;
        }
    }

    return err;
}

bool Graphics::Vulkan_Renderer::is_layer_supported(const char *layer) const
{
    for (size_t i = 0; i < this->layers.size(); ++i)
        if (strcmp(layer, this->layers[i].layerName) == 0)
            return true;
    return false;
}

VkResult Graphics::Vulkan_Renderer::check_layers_support(void)
{
    VkResult err = VK_SUCCESS;
    std::string tmp_str;

    for (auto req_layer : this->req_layers)
    {
        if (!is_layer_supported(req_layer))
        {
            tmp_str = "Layer \"";
            tmp_str += req_layer;
            tmp_str += "\" is not existing";

            this->gen_report_error(
                "is_layer_supported",
                tmp_str.data(),
                err);

            err = VK_INCOMPLETE;
            break;
        }
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::find_instance_version(void)
{
    VkResult err = VK_SUCCESS;

    err = vkEnumerateInstanceVersion(&this->version);

    if (err)
    {
        this->gen_report_error("vkEnumerateInstanceVersion", nullptr, err);
        this->version = 0;
    }
    else
        this->version &= ~(0xFFFU); // убрать состовляющую патча

    return err;
}

VkResult Graphics::Vulkan_Renderer::create_instance()
{
    VkResult err = VK_SUCCESS;

    // Инициализация информации о приложении для Vulkan
    this->appInfo = {};
    this->appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // Какую структур данных должен использовать vk для инициализации
    this->appInfo.pApplicationName = "Galactic Engineer"; // Название приложения
    this->appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // Версия приложения
    this->appInfo.pEngineName = "MZcore"; // Название игрового движка
    this->appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // Версия игрового движка
    this->appInfo.apiVersion = this->version; // Версия vk

    // Заполнение структуры для создания экземпляра Vulkan
    this->createInfo = {};
    this->createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    this->createInfo.pApplicationInfo = &this->appInfo;
    this->createInfo.enabledExtensionCount = this->req_extensions.size();
    this->createInfo.ppEnabledExtensionNames = this->req_extensions.data();
    this->createInfo.enabledLayerCount = this->req_layers.size();
    this->createInfo.ppEnabledLayerNames = this->req_layers.data();

    // Создание экземпляра Vulkan
    if ((err = vkCreateInstance(&this->createInfo, nullptr, &this->instance)) != VK_SUCCESS)
        this->gen_report_error("vkCreateInstance", nullptr, err);

    return err;
}

VkResult Graphics::Vulkan_Renderer::load_extensions(void)
{
    VkResult err = VK_SUCCESS;

    if (!err)
    {
        if ((err = VK_DYNAMIC_LOAD_FUNC_CHECK(this->instance, vkCreateDebugUtilsMessengerEXT)))
        {
            this->gen_report_error("vkGetInstanceProcAddr",
                "cannot load \"vkCreateDebugUtilsMessengerEXT\" function",
                err);
        }
    }

    if (!err)
    {
        if ((err = VK_DYNAMIC_LOAD_FUNC_CHECK(this->instance, vkDestroyDebugUtilsMessengerEXT)))
        {
            this->gen_report_error("vkGetInstanceProcAddr",
                "cannot load \"vkDestroyDebugUtilsMessengerEXT\" function",
                err);
        }
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::create_debug_utils_messenger_ext(void)
{
    VkResult err = VK_SUCCESS;
    VkDebugUtilsMessengerEXT debugMessenger;
    VkDebugUtilsMessengerCreateInfoEXT createInfo = {};

    createInfo.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

    createInfo.messageSeverity =
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;

    createInfo.messageType =
    VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
    VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;

    createInfo.pfnUserCallback = vk_debug_msg_callback;
    createInfo.pUserData = nullptr;

    err = vkCreateDebugUtilsMessengerEXT(this->instance, &createInfo, nullptr, &debugMessenger);

    if (err)
    {
        this->gen_report_error(
            "vkCreateDebugUtilsMessengerEXT",
            nullptr,
            err
        );
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::create_surface(void)
{
    VkResult err = createWin32SurfaceKHR(this->instance, &this->surface);

    if (err)
    {
        this->gen_report_error(
            "kCreateWin32SurfaceKHR",
            nullptr,
            err
        );
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::enumerate_physical_devices(void)
{
    VkResult err = VK_SUCCESS;
    uint32_t device_count = 0;

    this->physical_devices.clear();

    // получить количество физических устройств
    if ((err = vkEnumeratePhysicalDevices(instance, &device_count, nullptr)) != VK_SUCCESS)
        this->gen_report_error("vkEnumeratePhysicalDevices", nullptr, err);

    // проверить что нашлось хотя бы одно устройство
    if (!err && !device_count)
    {
        err = VkResult::VK_INCOMPLETE; // самый подходящий код возврата, который я смог найти
        this->gen_report_error("vkEnumeratePhysicalDevices",
                               "pc is out of any physical devices",
                               err);
    }

    // получить информацию о физических устройствах
    if (!err)
    {
        this->physical_devices.resize(device_count);

        if ((err = vkEnumeratePhysicalDevices(instance, &device_count, physical_devices.data())) != VK_SUCCESS)
            this->gen_report_error("vkEnumeratePhysicalDevices", nullptr, err);
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::choosing_physical_device(void)
{
    VkResult err = VK_INCOMPLETE;

    for (VkPhysicalDevice vpd : physical_devices)
    {
        if (check_device_extension_support(&vpd, this->req_device_extensions.data(), this->req_device_extensions.size()) == VK_SUCCESS)
        {
            this->physical_device = vpd;
            err = VK_SUCCESS;
            break;
        }
    }

    if (err)
        this->gen_report_error("choosing_physical_device", "no any suitable physical device");

    return err;
}

VkResult Graphics::Vulkan_Renderer::get_physical_device_queue_family_properties(void)
{
    VkResult err = VK_SUCCESS;
    uint32_t queue_count = 0;

    vkGetPhysicalDeviceQueueFamilyProperties(this->physical_device, &queue_count, nullptr);

    if (!queue_count)
    {
        this->gen_report_error(
            "vkGetPhysicalDeviceQueueFamilyProperties",
            "no any device queue device properties");

        err = VK_INCOMPLETE;
    }

    if (!err)
    {
        this->physical_device_queue_family_props.resize(queue_count);

        vkGetPhysicalDeviceQueueFamilyProperties(
            this->physical_device, &queue_count,
            this->physical_device_queue_family_props.data()
        );
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::check_physical_device_surface_supportKHR(uint32_t index, VkBool32 *supported)
{
    VkResult err = VK_SUCCESS;

    err = vkGetPhysicalDeviceSurfaceSupportKHR(
        this->physical_device, index, this->surface, supported
    );

    if (err)
    {
        this->gen_report_error(
            "vkGetPhysicalDeviceSurfaceSupportKHR",
            nullptr,
            err
        );
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::choosing_phisical_device_queue_family(void)
{
    VkResult err = VK_SUCCESS;
    VkBool32 supported = VK_FALSE;

    this->queue_family.gset = false;
    this->queue_family.pset = false;

    uint32_t i = 0;
    bool not_ready = true;

    while (!err && not_ready && i < this->physical_device_queue_family_props.size())
    {
        if (!queue_family.gset && this->physical_device_queue_family_props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT)
        {
            this->queue_family.gi = i;
            this->queue_family.gset = true;
        }

        if (!queue_family.pset)
        {
            err = check_physical_device_surface_supportKHR(i, &supported);

            if (!err && supported)
            {
                this->queue_family.pi = i;
                this->queue_family.pset = true;
            }
        }

        not_ready = !(this->queue_family.gset && this->queue_family.pset);
        ++i;
    }

    if (!err && not_ready)
        err = VK_INCOMPLETE;

    if (err)
    {
        this->gen_report_error(
            "choosing_phisical_device_queue_family",
            "no any fitted queue family supported neccessary flags");
    }

    return err;
}

void Graphics::Vulkan_Renderer::set_std_devise_extensions(void)
{
    this->req_device_extensions.clear();
    this->req_device_extensions.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);
}

VkResult Graphics::Vulkan_Renderer::create_logical_device(void)
{
    VkResult err = VK_SUCCESS;
    uint32_t queue_count = 0;
    uint32_t unique_queues[2]; // максимальное количество уже известно
    VkDeviceQueueCreateInfo vdqci[2];
    VkPhysicalDeviceFeatures vpdf; // гарантия поддержки требуемых функций
    VkDeviceCreateInfo vdci;
    float priority = 1.f;

    if (!(this->queue_family.gset && this->queue_family.pset))
    {
        this->gen_report_error(
            "create_logical_device",
            "queue_family indeces weren't inited"
        );

        err = VK_INCOMPLETE;
    }
    else
    {
        unique_queues[0] = this->queue_family.gi;

        if (unique_queues[0] == this->queue_family.pi)
        {
            queue_count = 1;
        }
        else
        {
            unique_queues[1] = this->queue_family.pi;
            queue_count = 2;
        }

        for (uint32_t i = 0; i < queue_count; ++i)
        {
            vdqci[i].sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
            vdqci[i].flags = 0;
            vdqci[i].pNext = nullptr;
            vdqci[i].queueFamilyIndex = unique_queues[i];
            vdqci[i].queueCount = 1;
            vdqci[i].pQueuePriorities = &priority;
        }

        memset(&vpdf, VK_FALSE, sizeof(VkPhysicalDeviceFeatures));

        vdci.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
        vdci.flags = 0;
        vdci.pNext = nullptr;
        vdci.pEnabledFeatures = &vpdf;
        vdci.queueCreateInfoCount = queue_count;
        vdci.pQueueCreateInfos = vdqci;
        vdci.enabledLayerCount = this->req_layers.size();
        vdci.ppEnabledLayerNames = this->req_layers.data();
        vdci.enabledExtensionCount = this->req_device_extensions.size();
        vdci.ppEnabledExtensionNames = this->req_device_extensions.data();

        err = vkCreateDevice(
            this->physical_device, &vdci,
            nullptr, &this->device
        );
    }

    if (err)
    {
        this->gen_report_error(
            "vkCreateDevice",
            nullptr, err
        );
    }

    return err;
}

VkResult Graphics::Vulkan_Renderer::get_queue(void)
{
    VkResult err = VK_SUCCESS;

    if (!this->device)
    {
        this->gen_report_error(
            "get_queue",
            "logical device wasn't created"
        );

        err = VK_ERROR_DEVICE_LOST;
    }

    if (!err && !queue_family.gset)
    {
        this->gen_report_error(
            "get_queue",
            "family index wasn't set"
        );

        err = VK_INCOMPLETE;
    }

    if (!err && !queue_family.pset)
    {
        this->gen_report_error(
            "get_queue",
            "queue index wasn't set"
        );

        err = VK_INCOMPLETE;
    }

    if (!err)
    {
        vkGetDeviceQueue(
            this->device, this->queue_family.gi,
            0, &this->graphic_queue
        );

        if (!this->graphic_queue)
        {
            this->gen_report_error(
                "vkGetDeviceQueue",
                "Can't get graphics queue pointer"
            );

            err = VK_INCOMPLETE;
        }
    }

    if (!err)
    {
        vkGetDeviceQueue(
            this->device, this->queue_family.pi,
            0, &this->present_queue
        );

        if (!this->present_queue)
        {
            this->gen_report_error(
                "vkGetDeviceQueue",
                "Can't get present queue pointer"
            );

            err = VK_INCOMPLETE;
        }
    }

    return err;
}

void Graphics::Vulkan_Renderer::output_version(void) const
{
    printf("----------------------------------------\n");
    if (this->version)
    {
        printf("System can support VK version:\n");
        printf(" Variant: %d\n", VK_API_VERSION_VARIANT(version));
        printf(" Major:   %d\n", VK_API_VERSION_MAJOR(version));
        printf(" Minor:   %d\n", VK_API_VERSION_MINOR(version));
        printf(" Patch:   %d\n", VK_API_VERSION_PATCH(version));
    }
    else
    {
        printf("VK version: undefined\n");
    }
}

void Graphics::Vulkan_Renderer::output_instance_layers_properties(void) const
{
    printf("----------------------------------------\n");
    printf("Available Vulkan Instance Extensions:\n");

    int nomer = 1;

    for (const auto &layers : this->layers)
    {
        printf("\n%d:\tName: %s\n", nomer, layers.layerName);
        printf("\tSpec Version: %u\n", layers.specVersion);
        printf("\tImplementation Version: %u\n", layers.implementationVersion);
        printf("\tDescription: %s\n", layers.description);
        ++nomer;
    }
}

void Graphics::Vulkan_Renderer::output_instance_extensions_preperties(void) const
{
    printf("----------------------------------------\n");
    printf("Available Vulkan Instance Extensions:\n");

    int nomer = 1;

    for (const auto &extension : this->extensions)
    {
        printf("\n%d:\tName: %s\n", nomer, extension.extensionName);
        printf("\tSpec Version: %u\n", extension.specVersion);
        ++nomer;
    }
}

void Graphics::Vulkan_Renderer::output_physical_devices(void) const
{
    const VkPhysicalDevice *vpd = this->physical_devices.data();
    int nomer = 1;

    printf("----------------------------------------\n");
    printf("Avaible physical devices:\n");

    for (uint32_t i = 0; i < this->physical_devices.size(); ++i, ++vpd, ++nomer)
    {
        printf("\n%d:\n", nomer);
        output_physical_device(vpd);
    }
}

void Graphics::Vulkan_Renderer::output_choosen_physical_device(void) const
{
    printf("----------------------------------------\n");
    printf("Using physical device:\n");
    output_physical_device(&this->physical_device);
}

void Graphics::Vulkan_Renderer::output_physical_device_queue_family_properties(void) const
{
    printf("----------------------------------------\n");
    printf("Physical device queue famuly properties:\n");

    for (size_t i = 0; i < this->physical_device_queue_family_props.size(); ++i)
    {
        printf("\n%zu:\n", i);
        output_physical_device_queue_family_prop(&this->physical_device_queue_family_props[i]);
    }
}

VkLayerProperties *Graphics::Vulkan_Renderer::get_instance_layers_properties_arr(size_t *lenght)
{
    *lenght = this->layers.size();
    return this->layers.data();
}

VkExtensionProperties *Graphics::Vulkan_Renderer::get_instance_extensions_properties_arr(size_t *lenght)
{
    *lenght = this->extensions.size();
    return this->extensions.data();
}

VkPhysicalDevice *Graphics::Vulkan_Renderer::get_physical_devices_arr(size_t *lenght)
{
    *lenght = this->physical_devices.size();
    return this->physical_devices.data();
}

Graphics::Vulkan_Renderer::~Vulkan_Renderer()
{
    if (this->device)
    {
        vkDestroyDevice(this->device, nullptr);
        this->device = VK_NULL_HANDLE;
    }

    if (this->surface && this->instance)
    {
        vkDestroySurfaceKHR(this->instance, this->surface, nullptr);
        this->surface = VK_NULL_HANDLE;
    }

    if (this->dbg_msg && this->instance)
    {
        vkDestroyDebugUtilsMessengerEXT(instance, dbg_msg, nullptr);
        this->dbg_msg = VK_NULL_HANDLE;
    }

    if (this->instance)
    {
        vkDestroyInstance(instance, nullptr);
        this->instance = VK_NULL_HANDLE;
    }
}

void Graphics::Vulkan_Renderer::gen_report_error(const char *func_name, const char *description, int err_code)
{
    std::string tmp;

    this->error_report.assign("Error from ");
    this->error_report += func_name;
    this->error_report += "()";

    if (description)
    {
        this->error_report += "\nDescription: ";
        this->error_report += description;
    }

    if (err_code)
    {
        tmp = std::to_string(err_code);
        this->error_report += "\nErr_code: ";
        this->error_report += tmp;
    }

}

namespace Graphics
{
    bool debugging = true;
    Vulkan_Renderer vk_renderer;
}
