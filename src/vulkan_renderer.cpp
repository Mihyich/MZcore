#include "vulkan_renderer.h"

int Graphics::Vulkan_Renderer::init()
{
    VkResult err = VK_SUCCESS;

    // поиск доступной версии вулкана
    if ((err = vkEnumerateInstanceVersion(&this->version)) != VK_SUCCESS)
        this->gen_report_error("vkEnumerateInstanceVersion", nullptr, err);

    if (!err)
    {
        this->version &= ~(0xFFFU); // убрать состовляющую патча
        // version = VK_MAKE_API_VERSION(0, 1, 0, 0);

        // Инициализация информации о приложении для Vulkan
        this->appInfo = {};
        this->appInfo.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO; // Какую структур данных должен использовать vk для инициализации
        this->appInfo.pApplicationName = "Galactic Engineer"; // Название приложения
        this->appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0); // Версия приложения
        this->appInfo.pEngineName = "MZcore"; // Название игрового движка
        this->appInfo.engineVersion = VK_MAKE_VERSION(1, 0, 0); // Версия игрового движка
        this->appInfo.apiVersion = this->version; // Версия vk

        // Создание структуры для создания экземпляра Vulkan
        this->createInfo = {};
        this->createInfo.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
        this->createInfo.pApplicationInfo = &this->appInfo;

        // Создание экземпляра Vulkan
        if ((err = vkCreateInstance(&this->createInfo, nullptr, &this->instance)) != VK_SUCCESS)
            this->gen_report_error("vkCreateInstance", nullptr, err);
    } 

    // Другие шаги инициализации могут включать в себя создание устройства, swap chain и т.д.

    return err;
}

int Graphics::Vulkan_Renderer::enumerate_instance_layers_properties(void)
{
    VkResult err = VK_SUCCESS;
    uint32_t layers_count = 0;

    this->layers.clear();

    if (!(err = vkEnumerateInstanceLayerProperties(&layers_count, nullptr)) != VK_SUCCESS)
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

int Graphics::Vulkan_Renderer::enumerate_instance_extensions_properties(void)
{
    VkResult err = VK_SUCCESS;
    uint32_t extension_count = 0;

    this->extensions.clear();

    if (!(err = vkEnumerateInstanceExtensionProperties(nullptr, &extension_count, nullptr)) != VK_SUCCESS)
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

int Graphics::Vulkan_Renderer::enumerate_physical_devices(void)
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

std::string *Graphics::Vulkan_Renderer::get_error_report(void)
{
    return &this->error_report;
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
    const VkPhysicalDevice *pd = this->physical_devices.data();
    VkPhysicalDeviceProperties pdp;
    int nomer = 1;

    printf("----------------------------------------\n");
    printf("Avaible physical devices:\n");

    for (uint32_t i = 0; i < this->physical_devices.size(); ++i, ++pd)
    {
        vkGetPhysicalDeviceProperties(*pd, &pdp);

        printf("\n%d:\tDevice Name: %s\n", nomer, pdp.deviceName);
        printf("\tDevice Type: %d\n", pdp.deviceType);

        printf("\tAPI Version: %u.%u.%u\n",
               VK_VERSION_MAJOR(pdp.apiVersion),
               VK_VERSION_MINOR(pdp.apiVersion),
               VK_VERSION_PATCH(pdp.apiVersion));

        // Другие свойства...

        // printf("----------------------------------------\n");

        ++nomer;
    }
}

Graphics::Vulkan_Renderer::~Vulkan_Renderer()
{

}

void Graphics::Vulkan_Renderer::gen_report_error(const char *func_name, const char *description, int err_code)
{
    std::string tmp;

    this->error_report.assign("error from ");
    this->error_report += func_name;
    this->error_report += "()";

    if (description)
    {
        this->error_report += "\ndescription: ";
        this->error_report += description;
    }

    if (err_code)
    {
        tmp = std::to_string(err_code);
        this->error_report += "\nerr_code: ";
        this->error_report += tmp;
    }
}
