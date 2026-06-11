//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "ImageStore.h"
#include "SwapChainStore.h"
#include "model/Queues.h"

#include <memory>
#include <vulkan/vulkan.h>

namespace vul {

  class DeviceStore
  {
    model::QueueFamilyIndices queueFamilyIndices_{};

    /**
     * Vulkan logical device - used for all rendering operations to the screen.
     */
    VkDevice handle_ = VK_NULL_HANDLE;

    model::Queues queues_{};

    std::unique_ptr<ImageStore> depthImage_{};

  public:
    ~DeviceStore() { resetHandle(VK_NULL_HANDLE); }

    VkDevice handle() const { return handle_; }

    const model::QueueFamilyIndices &queueFamilyIndices() const { return queueFamilyIndices_; }

    void setQueueFamilyIndices(const model::QueueFamilyIndices &queueFamilyIndices) { queueFamilyIndices_ = queueFamilyIndices; }

    void setQueues(const model::Queues &queues) { queues_ = queues; }

    void resetHandle(const VkDevice handle);

    void resetDepthImage(std::unique_ptr<ImageStore> depthImage) { depthImage_ = std::move(depthImage); }

    [[nodiscard]] ImageStore *depthImage() const;
  };
} // namespace vul
