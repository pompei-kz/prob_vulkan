//
// Created by pompei on 2026-06-11.
//

#pragma once
#include <vulkan/vulkan.h>

namespace vul::model {

  struct Queues
  {
    /**
     * Очередь Vulkan для графических команд.
     *
     * Значение этого поля является мусором, если `this->handle_ == VK_NULL_HANDLE`
     */
    VkQueue graphicsQueue = VK_NULL_HANDLE;

    /**
     * Очередь Vulkan для показа изображений.
     *
     * Значение этого поля является мусором, если `this->handle_ == VK_NULL_HANDLE`
     */
    VkQueue presentQueue = VK_NULL_HANDLE;
  };

} // namespace vul::model
