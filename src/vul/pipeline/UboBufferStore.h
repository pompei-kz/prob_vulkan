//
// Created by pompei on 2026-06-12.
//

#pragma once
#include "util/Log.h"
#include "util/util.h"
#include "vul/ImageStore_Worker.h"

#include <vulkan/vulkan_core.h>

namespace vul::pipeline {
  class UboBufferStore
  {
    const VkDevice         device_;
    const VkPhysicalDevice physicalDevice_;
    const std::string      name_;

    VkBuffer         buffer_         = VK_NULL_HANDLE;
    VkDeviceMemory   memory_         = VK_NULL_HANDLE;
    void            *memoryRef_      = nullptr;
    VkDescriptorPool descriptorPool_ = VK_NULL_HANDLE;

  public:
    explicit UboBufferStore(const VkDevice device, const VkPhysicalDevice physicalDevice, const std::string &name)
        : device_(device)
        , physicalDevice_(physicalDevice)
        , name_(name)
    {}

    ~UboBufferStore()
    {
      resetDescriptorPool(VK_NULL_HANDLE);
      resetMemoryRef(nullptr);
      resetMemory(VK_NULL_HANDLE);
      resetBuffer(VK_NULL_HANDLE);
    }

    [[nodiscard]] VkBuffer         buffer() const { return buffer_; }
    [[nodiscard]] VkDeviceMemory   memory() const { return memory_; }
    [[nodiscard]] void            *memoryRef() const { return memoryRef_; }
    [[nodiscard]] VkDescriptorPool descriptorPool() const { return descriptorPool_; }

    void resetMemory(const VkDeviceMemory memory);

    void resetBuffer(const VkBuffer buffer);

    void resetDescriptorPool(const VkDescriptorPool descriptorPool);

    void resetMemoryRef(void *memoryRef);

    void populateUboBufferStore(const std::string &pipelineID, const VkDeviceSize bufferSize);
  };

} // namespace vul::pipeline
