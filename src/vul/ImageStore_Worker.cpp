//
// Created by pompei on 2026-06-11.
//

#include "ImageStore_Worker.h"

namespace vul {
  void ImageStore_Worker::createDepthImage() const
  {
    const auto [width, height] = topStore_->swapChain()->extent();

    DeviceStore   *deviceStore = topStore_->device();
    const VkDevice device      = deviceStore->handle();

    std::unique_ptr<ImageStore> imageStore = std::make_unique<ImageStore>(deviceStore->handle());

    imageStore->setFormat(VK_FORMAT_D32_SFLOAT /*depth image format*/);

    {
      VkImageCreateInfo ci{};
      ci.sType         = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO;
      ci.imageType     = VK_IMAGE_TYPE_2D;
      ci.extent.width  = width;
      ci.extent.height = height;
      ci.extent.depth  = 1;
      ci.mipLevels     = 1;
      ci.arrayLayers   = 1;
      ci.format        = imageStore->format();
      ci.tiling        = VK_IMAGE_TILING_OPTIMAL;
      ci.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      ci.usage         = VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT;
      ci.samples       = VK_SAMPLE_COUNT_1_BIT;
      ci.sharingMode   = VK_SHARING_MODE_EXCLUSIVE;

      VkImage depthImage;

      if (const VkResult vkResult = vkCreateImage(device, &ci, nullptr, &depthImage); vkResult != VK_SUCCESS) {
        util::Log::get()->error("AdAqQPvDWe", "Cannot create depth image: {}", util::VkResult_to_str(vkResult));
      }

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("W2MSgvB8UE", "vkCreateImage(Depth image) OK, image = {}", static_cast<void *>(depthImage));
      }

      imageStore->resetHandle(depthImage);
    }

    VkMemoryRequirements memReq{};
    vkGetImageMemoryRequirements(device, imageStore->handle(), &memReq);

    {
      VkMemoryAllocateInfo allocInfo{};
      allocInfo.sType           = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
      allocInfo.allocationSize  = memReq.size;
      allocInfo.memoryTypeIndex = findMemoryType(topStore_->selectedVkPhysicalDevice(), memReq.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);

      VkDeviceMemory memory;

      if (const VkResult result = vkAllocateMemory(device, &allocInfo, nullptr, &memory); result != VK_SUCCESS) {
        throw std::runtime_error(std::string("vZ5SuJTxtv :: vkAllocateMemory failed for depth buffer: VkResult = ") + util::VkResult_to_str(result));
      }

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("bSSZr6sRgz", "vkAllocateMemory(Depth image) OK, memory = {}", static_cast<void *>(memory));
      }

      imageStore->resetMemory(memory);
    }

    if (const VkResult result = vkBindImageMemory(device, imageStore->handle(), imageStore->memory(), 0); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("xxSmd3Dgf3 :: vkBindImageMemory failed for depth buffer: VkResult = ") + util::VkResult_to_str(result));
    }

    {
      VkImageViewCreateInfo viewInfo{};
      viewInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
      viewInfo.image                           = imageStore->handle();
      viewInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;
      viewInfo.format                          = imageStore->format();
      viewInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_DEPTH_BIT;
      viewInfo.subresourceRange.baseMipLevel   = 0;
      viewInfo.subresourceRange.levelCount     = 1;
      viewInfo.subresourceRange.baseArrayLayer = 0;
      viewInfo.subresourceRange.layerCount     = 1;

      VkImageView imageView;

      if (const VkResult result = vkCreateImageView(device, &viewInfo, nullptr, &imageView); result != VK_SUCCESS) {
        throw std::runtime_error(std::string("ig4yo9KzZ4 :: vkCreateImageView failed for depth buffer: VkResult = ") + util::VkResult_to_str(result));
      }

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("telPnIWnP1", "vkCreateImageView(Depth image) OK, imageView = {}", static_cast<void *>(imageView));
      }

      imageStore->resetImageView(imageView);
    }

    deviceStore->resetDepthImage(std::move(imageStore));

    if (util::Log::get()->hasVerbose()) {
      util::Log::get()->verbose("AZ1tWiWpUn", "Depth image created {}x{}", width, height);
    }
  }

  uint32_t ImageStore_Worker::findMemoryType(const VkPhysicalDevice physicalDevice,
                                             const uint32_t         typeFilter,
                                             // ReSharper disable once CppDFAConstantParameter
                                             const VkMemoryPropertyFlags properties)
  {
    VkPhysicalDeviceMemoryProperties memProperties{};

    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; ++i) {
      const bool typeSupported = (typeFilter & (1u << i)) != 0;

      // ReSharper disable once CppTooWideScopeInitStatement
      const bool hasRequiredProperties = (memProperties.memoryTypes[i].propertyFlags & properties) == properties;

      if (typeSupported && hasRequiredProperties) {
        return i;
      }
    }

    throw std::runtime_error("pc7BrPBNxO :: Failed to find suitable memory type");
  }
} // namespace vul