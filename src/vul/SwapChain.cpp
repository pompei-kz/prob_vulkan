//
// Created by pompei on 2026-06-09.
//

#include "SwapChain.h"

#include "util/Log.h"

namespace vul {

  SwapChain::~SwapChain()
  {
    resetHandle(VK_NULL_HANDLE);
  }

  void SwapChain::resetHandle(const VkSwapchainKHR handle)
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

  void SwapChain::resetImageViews(const std::vector<VkImageView> &imageViews)
  {
    for (const VkImageView imageView : imageViews_) {
      vkDestroyImageView(vkDevice_, imageView, nullptr);

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("5Qq5BzHAij", "vkDestroyImageView");
      }
    }
    imageViews_ = imageViews;
  }

  void SwapChain::setEnvironment(const std::vector<VkImage> &images, const VkFormat imageFormat, const VkExtent2D extent)
  {
    images_      = images;
    imageFormat_ = imageFormat;
    extent_      = extent;
  }
} // namespace vul
