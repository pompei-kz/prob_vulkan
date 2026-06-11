//
// Created by pompei on 2026-06-09.
//

#include "SwapChainStore.h"

#include "util/Log.h"

namespace vul {

  SwapChainStore::~SwapChainStore()
  {
    resetHandle(VK_NULL_HANDLE);
  }

  void SwapChainStore::resetHandle(const VkSwapchainKHR handle)
  {
    if (handle_) {
      resetImageViews(std::vector<VkImageView>());
      vkDestroySwapchainKHR(vkDevice_, handle_, nullptr);

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("ekHpQm9sK4", "vkDestroySwapChainKHR");
      }

      handle_ = VK_NULL_HANDLE;
    }
    handle_ = handle;
  }

  void SwapChainStore::resetImageViews(const std::vector<VkImageView> &imageViews)
  {
    for (const VkImageView imageView : imageViews_) {
      vkDestroyImageView(vkDevice_, imageView, nullptr);

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("5Qq5BzHAij", "vkDestroyImageView({})", static_cast<void *>(imageView));
      }
    }
    imageViews_ = imageViews;
  }

  void SwapChainStore::setEnvironment(const std::vector<VkImage> &images, const VkFormat imageFormat, const VkExtent2D extent)
  {
    images_      = images;
    imageFormat_ = imageFormat;
    extent_      = extent;
  }
} // namespace vul
