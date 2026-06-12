//
// Created by pompei on 2026-06-12.
//

#include "UboBufferStore.h"
namespace vul::pipeline {
  void UboBufferStore::resetMemory(const VkDeviceMemory memory)
  {
    if (memory_) {
      resetMemoryRef(nullptr);
      vkFreeMemory(device_, memory_, nullptr);
      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("sZDmhVO33h", "{}: vkFreeMemory(), H={}", name_, static_cast<void *>(memory_));
      }
    }
    memory_ = memory;
  }

  void UboBufferStore::resetBuffer(const VkBuffer buffer)
  {
    if (buffer_) {
      vkDestroyBuffer(device_, buffer_, nullptr);
      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("FV3XSc6XFA", "{}: vkDestroyBuffer(), H={}", name_, static_cast<void *>(buffer_));
      }
    }
    buffer_ = buffer;
  }

  void UboBufferStore::resetDescriptorPool(const VkDescriptorPool descriptorPool)
  {
    if (descriptorPool_) {
      vkDestroyDescriptorPool(device_, descriptorPool_, nullptr);
      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("iah66xp7a8", "{}: vkDestroyDescriptorPool(), H={}", name_, static_cast<void *>(descriptorPool_));
      }
    }
    descriptorPool_ = descriptorPool;
  }

  void UboBufferStore::resetMemoryRef(void *memoryRef)
  {
    if (memoryRef_) {
      vkUnmapMemory(device_, memory_);
      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("obN79x4SoH", "{}: vkUnmapMemory(), H={}, ref={}", name_, static_cast<void *>(memory_), memoryRef_);
      }
    }
    memoryRef_ = memoryRef;
  }

  void UboBufferStore::populateUboBufferStore(const std::string &pipelineID, const VkDeviceSize bufferSize)
  {
    const std::string &uboName = name_;

    {
      VkBufferCreateInfo ci{};
      ci.sType       = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
      ci.size        = bufferSize;
      ci.usage       = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT;
      ci.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

      VkBuffer cameraBuffer;

      if (const VkResult result = vkCreateBuffer(device_, &ci, nullptr, &cameraBuffer); result != VK_SUCCESS) {
        throw std::runtime_error(std::format("MkCSfF5YQF :: {}: failed to create camera uniform buffer: VkResult = {}", //
                                             uboName,
                                             util::VkResult_to_str(result)));
      }

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("gd1f36ObfI",
                                  "Pipeline_ShapeGroup[{}]: {}: vkCreateBuffer() called OK, H={:#x}",
                                  pipelineID,
                                  uboName,
                                  reinterpret_cast<uint64_t>(cameraBuffer));
      }

      resetBuffer(cameraBuffer);
    }
    {
      VkMemoryRequirements mr{};

      vkGetBufferMemoryRequirements(device_, buffer(), &mr);

      VkMemoryAllocateInfo mai{};
      mai.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      mai.allocationSize  = mr.size;
      mai.memoryTypeIndex = findMemoryType("fNlIRzHz1z",
                                           physicalDevice_,
                                           mr.memoryTypeBits,
                                           VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

      VkDeviceMemory cameraMemory;

      if (const VkResult result = vkAllocateMemory(device_, &mai, nullptr, &cameraMemory); result != VK_SUCCESS) {
        throw std::runtime_error(std::format("o5qwu80uA4 :: {}: ERR vkAllocateMemory(): VkResult = {}", //
                                             uboName,
                                             util::VkResult_to_str(result)));
      }

      resetMemory(cameraMemory);

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("HWiZrZgWlj",
                                  "Pipeline_ShapeGroup[{}]: {}: vkAllocateMemory() called OK, H={}",
                                  pipelineID,
                                  uboName,
                                  static_cast<void *>(cameraMemory));
      }
    }
    {
      if (const VkResult result = vkBindBufferMemory(device_, buffer(), memory(), 0); result != VK_SUCCESS) {
        throw std::runtime_error(std::format("tMVbN9ifZf :: {}: ERR vkBindBufferMemory(): VkResult = {}", //
                                             uboName,
                                             util::VkResult_to_str(result)));
      }
    }
    {
      void *cameraUboMemoryRef = nullptr;
      if (const VkResult result = vkMapMemory(device_, memory(), 0, VK_WHOLE_SIZE, 0, &cameraUboMemoryRef); result != VK_SUCCESS) {
        throw std::runtime_error(std::format("8NoV6r7782 :: {}: ERR vkMapMemory(): VkResult = ", uboName, util::VkResult_to_str(result)));
      }

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("62inNEWxJI", //
                                  "Pipeline_ShapeGroup[{}]: {}: vkMapMemory() called OK, H={}, ref={}",
                                  pipelineID,
                                  uboName,
                                  static_cast<void *>(memory()),
                                  cameraUboMemoryRef);
      }

      resetMemoryRef(cameraUboMemoryRef);
    }

    {
      VkDescriptorPoolSize poolSize{};
      poolSize.type            = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
      poolSize.descriptorCount = 1;

      VkDescriptorPoolCreateInfo ci{};
      ci.sType         = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
      ci.maxSets       = 1;
      ci.poolSizeCount = 1;
      ci.pPoolSizes    = &poolSize;

      VkDescriptorPool descriptorPool;

      if (const VkResult result = vkCreateDescriptorPool(device_, &ci, nullptr, &descriptorPool); result != VK_SUCCESS) {
        throw std::runtime_error(std::format("n9jX9twEz0 :: {}: ERROR calling vkCreateDescriptorPool(): VkResult = ", //
                                             uboName,
                                             util::VkResult_to_str(result)));
      }

      resetDescriptorPool(descriptorPool);

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("i0sUm2ch5l",
                                  "Pipeline_ShapeGroup[{}]: {}: vkCreateDescriptorPool() called OK, H={}",
                                  pipelineID,
                                  uboName,
                                  static_cast<void *>(descriptorPool));
      }
    }
  }
} // namespace vul::pipeline