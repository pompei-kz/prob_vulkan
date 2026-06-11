//
// Created by pompei on 2026-06-11.
//

#pragma once
#include "util/Log.h"

#include <vulkan/vulkan.h>

namespace vul {

  class ImageStore
  {
    const VkDevice device_;
    VkImage        handle_    = VK_NULL_HANDLE;
    VkDeviceMemory memory_    = VK_NULL_HANDLE;
    VkImageView    imageView_ = VK_NULL_HANDLE;
    VkFormat       format_    = VK_FORMAT_UNDEFINED;

  public:
    explicit ImageStore(const VkDevice device)
        : device_(device)
    {}

    ~ImageStore() { resetHandle(nullptr); }

    void resetImageView(VkImageView imageView);

    void resetMemory(VkDeviceMemory memory);

    void resetHandle(const VkImage handle);

    [[nodiscard]] VkImage handle() const { return handle_; }

    [[nodiscard]] VkImageView imageView() const { return imageView_; }

    [[nodiscard]] VkFormat format() const { return format_; }

    void setFormat(const VkFormat format) { format_ = format; }

    [[nodiscard]] VkDeviceMemory memory() const { return memory_; }
  };

} // namespace vul
