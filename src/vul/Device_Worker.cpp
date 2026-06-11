//
// Created by pompei on 2026-06-11.
//

#include "Device_Worker.h"

namespace vul {
  void Device_Worker::create() const
  {

    const VkPhysicalDevice vkPhysicalDevice = topStore_->selectedVkPhysicalDevice();

    std::unique_ptr<Device> device = std::make_unique<Device>();

    device->setQueueFamilyIndices(selectRequiredQueueFamilyIndices());

    // Параметры очередей Vulkan для логического устройства.
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    uint32_t queueFamilyIndexGraphics = device->queueFamilyIndices().indexGraphics.value();
    uint32_t queueFamilyIndexPresent  = device->queueFamilyIndices().indexPresent.value();

    // ReSharper disable once CppTemplateArgumentsCanBeDeduced
    std::set<uint32_t> uniqueQueueFamilyIndices = {queueFamilyIndexGraphics, queueFamilyIndexPresent};

    constexpr float queuePriority = 1.0F;

    for (uint32_t queueFamily : uniqueQueueFamilyIndices) {
      // Описание очереди Vulkan для логического устройства.
      VkDeviceQueueCreateInfo queueCreateInfo{};
      queueCreateInfo.sType            = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; // Тип структуры создания очереди устройства.
      queueCreateInfo.queueFamilyIndex = queueFamily;                                // Индекс семейства очередей, из которого берется очередь.
      queueCreateInfo.queueCount       = 1;                                          // Количество создаваемых очередей в этом семействе.
      queueCreateInfo.pQueuePriorities = &queuePriority;                             // Приоритет очереди для планировщика устройства.
      queueCreateInfos.push_back(queueCreateInfo);
    }

    // Набор включаемых возможностей физического устройства Vulkan.
    VkPhysicalDeviceFeatures deviceFeatures{};

    // Параметры создания логического устройства Vulkan.
    VkDeviceCreateInfo createInfo{};
    createInfo.sType                   = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;                     // Тип структуры создания логического устройства.
    createInfo.queueCreateInfoCount    = static_cast<uint32_t>(queueCreateInfos.size());           // Количество описаний очередей устройства.
    createInfo.pQueueCreateInfos       = queueCreateInfos.data();                                  // Описания очередей, которые нужно создать.
    createInfo.pEnabledFeatures        = &deviceFeatures;                                          // Включаемые возможности физического устройства.
    createInfo.enabledExtensionCount   = static_cast<uint32_t>(model::K_DEVICE_EXTENSIONS.size()); // Количество расширений устройства.
    createInfo.ppEnabledExtensionNames = model::K_DEVICE_EXTENSIONS.data();                        // Имена включаемых расширений устройства.

    VkDevice vkDevice = VK_NULL_HANDLE;

    // Создаем логическое устройство Vulkan.
    if (const VkResult result = vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &vkDevice); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("cY7pD4nVaR :: failed to create logical device: VkResult = ") + util::VkResult_to_str(result));
    }

    device->resetHandle(vkDevice);

    model::Queues queues{};

    // Получаем графическую очередь Vulkan.
    vkGetDeviceQueue(vkDevice, queueFamilyIndexGraphics, 0, &queues.graphicsQueue);
    // Получаем очередь показа Vulkan.
    vkGetDeviceQueue(vkDevice, queueFamilyIndexPresent, 0, &queues.presentQueue);

    device->setQueues(queues);

    topStore_->resetDevice(std::move(device));
  }

  model::QueueFamilyIndices Device_Worker::selectRequiredQueueFamilyIndices() const
  {
    const VkSurfaceKHR              vkSdkSurface             = topStore_->vkSdkSurface();
    const VkPhysicalDevice          selectedVkPhysicalDevice = topStore_->selectedVkPhysicalDevice();
    const model::QueueFamilyIndices indices                  = model::findQueueFamilies(selectedVkPhysicalDevice, vkSdkSurface);

    if (!indices.complete()) {
      throw std::runtime_error("fPNvCE8M4v :: Queue families not complete");
    }

    return indices;
  }
} // vul