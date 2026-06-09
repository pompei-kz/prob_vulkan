//
// Created by pompei on 2026-06-09.
//

#include "QueueFamilyIndices.h"

#include "vul/SelectPhysicalDevice.h"

namespace vul::model {

  QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice vkPhysicalDevice, const VkSurfaceKHR vkSdkSurface)
  {

    uint32_t queueFamilyCount = 0;
    // Запрашиваем количество семейств очередей Vulkan.
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, nullptr);

    // Свойства семейств очередей Vulkan.
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    // Получаем свойства семейств очередей Vulkan.
    vkGetPhysicalDeviceQueueFamilyProperties(vkPhysicalDevice, &queueFamilyCount, queueFamilies.data());

    QueueFamilyIndices result;

    for (uint32_t i = 0; i < queueFamilies.size(); ++i) {

      if ((queueFamilies[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) != 0) {
        result.graphicsFamily = i;
      }

      // Флаг поддержки показа через Vulkan surface.
      VkBool32 presentSupport = VK_FALSE;

      // Проверяем поддержку показа Vulkan для семейства очередей.
      vkGetPhysicalDeviceSurfaceSupportKHR(vkPhysicalDevice, i, vkSdkSurface, &presentSupport);
      if (presentSupport == VK_TRUE) {
        result.presentFamily = i;
      }

      if (result.complete()) {
        break;
      }
    }

    return result;
  }

}
