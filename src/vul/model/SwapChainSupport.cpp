//
// Created by pompei on 2026-06-09.
//

#include "SwapChainSupport.h"

namespace vul::model {

  SwapChainSupport querySwapChainSupport(const VkPhysicalDevice vkPhysicalDevice, const VkSurfaceKHR vkSdkSurface)
  {

    SwapChainSupport details;
    // Получаем ограничения Vulkan surface для swap-chain.
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(vkPhysicalDevice, vkSdkSurface, &details.capabilities);

    uint32_t formatCount = 0;
    // Запрашиваем количество форматов Vulkan surface.
    vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSdkSurface, &formatCount, nullptr);
    if (formatCount != 0) {
      details.formats.resize(formatCount);
      // Получаем доступные форматы Vulkan surface.
      vkGetPhysicalDeviceSurfaceFormatsKHR(vkPhysicalDevice, vkSdkSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount = 0;
    // Запрашиваем количество режимов показа Vulkan surface.
    vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSdkSurface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
      details.presentModes.resize(presentModeCount);
      // Получаем доступные режимы показа Vulkan surface.
      vkGetPhysicalDeviceSurfacePresentModesKHR(vkPhysicalDevice, vkSdkSurface, &presentModeCount, details.presentModes.data());
    }

    return details;
  }

}