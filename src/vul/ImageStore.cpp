//
// Created by pompei on 2026-06-11.
//

#include "ImageStore.h"

namespace vul {

  void ImageStore::resetImageView(VkImageView imageView)
  {
    if (imageView_) {
      vkDestroyImageView(device_, imageView_, nullptr);
      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("HaZYIweByi", "Depth Image: vkDestroyImageView(), H={}", static_cast<void *>(imageView_));
      }
      imageView_ = VK_NULL_HANDLE;
    }
    imageView_ = imageView;
  }

  void ImageStore::resetMemory(VkDeviceMemory memory)
  {
    if (memory_) {
      resetImageView(nullptr);
      vkFreeMemory(device_, memory_, nullptr);
      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("E6aSGiJCtX", "Depth Image: vkFreeMemory(), H={}", static_cast<void *>(memory_));
      }
      memory_ = VK_NULL_HANDLE;
    }
    memory_ = memory;
  }

  void ImageStore::resetHandle(const VkImage handle)
  {
    if (handle_) {
      resetImageView(nullptr);
      resetMemory /*it also resets `imageView_`*/ (nullptr);
      vkDestroyImage(device_, handle_, nullptr);
      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("6ZaHZo8uPv", "Depth Image: vkDestroyImage(), H={}", static_cast<void *>(handle_));
      }
      handle_ = nullptr;
    }
    handle_ = handle;
  }
} // namespace vul