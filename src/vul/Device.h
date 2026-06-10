//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "SwapChain.h"

#include <memory>
#include <vulkan/vulkan.h>

namespace vul {
  class Device
  {
    /**
     * Логическое устройство Vulkan - с помощью него всё будет рисоваться на экране.
     */
    VkDevice vkDevice_ = VK_NULL_HANDLE;

    /**
     * Индекс семейства очередей для команд рисования графики.
     *
     * Значение этого поля является мусором, если `this->vkDevice_ == VK_NULL_HANDLE`
     */
    uint32_t graphicsFamilyIndex_{};

    /**
     * Индекс семейства очередей для команд передачи изображений на экран.
     *
     * Значение этого поля является мусором, если `this->vkDevice_ == VK_NULL_HANDLE`
     */
    uint32_t presentFamilyIndex_{};

    /**
     * Очередь Vulkan для графических команд.
     *
     * Значение этого поля является мусором, если `this->vkDevice_ == VK_NULL_HANDLE`
     */
    VkQueue graphicsQueue_ = VK_NULL_HANDLE;

    /**
     * Очередь Vulkan для показа изображений.
     *
     * Значение этого поля является мусором, если `this->vkDevice_ == VK_NULL_HANDLE`
     */
    VkQueue presentQueue_ = VK_NULL_HANDLE;

  public:
    ~Device();

    void create(const VkPhysicalDevice vkPhysicalDevice, const VkSurfaceKHR vkSdkSurface);

    VkDevice handle() const
    {
      return vkDevice_;
    }

    uint32_t graphicsFamilyIndex() const
    {
      return graphicsFamilyIndex_;
    }
  };
} // namespace vul
