//
// Created by pompei on 2026-06-09.
//

#pragma once
#include <optional>
#include <vector>
#include <vulkan/vulkan.h>

namespace vul::model {
  struct QueueFamilyIndices
  {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    [[nodiscard]] bool complete() const
    {
      return graphicsFamily.has_value() && presentFamily.has_value();
    }
  };

  // ReSharper disable once CppVariableCanBeMadeConstexpr
  inline const std::vector<const char *> K_DEVICE_EXTENSIONS = {
      VK_KHR_SWAPCHAIN_EXTENSION_NAME,
  };

  QueueFamilyIndices findQueueFamilies(const VkPhysicalDevice vkPhysicalDevice, const VkSurfaceKHR vkSdkSurface);
} // namespace vul::model
