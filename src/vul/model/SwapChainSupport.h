//
// Created by pompei on 2026-06-09.
//

#pragma once
#include <vector>
#include <vulkan/vulkan.h>

namespace vul::model {
  struct SwapChainSupport
  {
    // Храним ограничения поверхности Vulkan для выбора параметров swap-chain.
    VkSurfaceCapabilitiesKHR capabilities{};
    // Доступные форматы поверхности Vulkan.
    std::vector<VkSurfaceFormatKHR> formats;
    // Доступные режимы показа Vulkan.
    std::vector<VkPresentModeKHR> presentModes;
  };

  SwapChainSupport querySwapChainSupport(const VkPhysicalDevice vkPhysicalDevice, const VkSurfaceKHR vkSdkSurface);

} // namespace vul::model
