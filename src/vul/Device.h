//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "SwapChain.h"
#include "model/Queues.h"

#include <vulkan/vulkan.h>

namespace vul {

  class Device
  {
    model::QueueFamilyIndices queueFamilyIndices_{};

    /**
     * Vulkan logical device - used for all rendering operations to the screen.
     */
    VkDevice handle_ = VK_NULL_HANDLE;

    model::Queues queues_{};

  public:
    ~Device() { resetHandle(VK_NULL_HANDLE); }

    VkDevice handle() const { return handle_; }

    const model::QueueFamilyIndices &queueFamilyIndices() const { return queueFamilyIndices_; }

    void setQueueFamilyIndices(const model::QueueFamilyIndices &queueFamilyIndices) { queueFamilyIndices_ = queueFamilyIndices; }

    void setQueues(const model::Queues &queues) { queues_ = queues; }

    void resetHandle(const VkDevice handle);
  };
} // namespace vul
