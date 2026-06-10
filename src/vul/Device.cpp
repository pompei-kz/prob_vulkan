//
// Created by pompei on 2026-06-09.
//

#include "Device.h"

#include "../util/util.h"
#include "model/QueueFamilyIndices.h"

#include <set>
#include <stdexcept>
#include <vector>

namespace vul {

  Device::~Device()
  {
    if (vkDevice_) {
      // Уничтожаем логическое устройство Vulkan.
      vkDestroyDevice(vkDevice_, nullptr);
      vkDevice_ = VK_NULL_HANDLE;
    }
  }

  void Device::create(const VkPhysicalDevice vkPhysicalDevice, const VkSurfaceKHR vkSdkSurface)
  {
    {
      model::QueueFamilyIndices indices = model::findQueueFamilies(vkPhysicalDevice, vkSdkSurface);

      if (!indices.complete()) {
        throw std::runtime_error("fPNvCE8M4v :: Queue families not complete");
      }

      graphicsFamilyIndex_ = indices.graphicsFamily.value();
      presentFamilyIndex_  = indices.presentFamily.value();
    }

    // Параметры очередей Vulkan для логического устройства.
    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

    // ReSharper disable once CppTemplateArgumentsCanBeDeduced
    std::set<uint32_t> uniqueQueueFamilies = {graphicsFamilyIndex_, presentFamilyIndex_};

    constexpr float queuePriority = 1.0F;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
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

    // Создаем логическое устройство Vulkan.
    if (const VkResult result = vkCreateDevice(vkPhysicalDevice, &createInfo, nullptr, &vkDevice_); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("cY7pD4nVaR :: failed to create logical device: VkResult = ") + util::VkResult_to_str(result));
    }

    // Получаем графическую очередь Vulkan.
    vkGetDeviceQueue(vkDevice_, graphicsFamilyIndex_, 0, &graphicsQueue_);
    // Получаем очередь показа Vulkan.
    vkGetDeviceQueue(vkDevice_, presentFamilyIndex_, 0, &presentQueue_);
  }
} // namespace vul