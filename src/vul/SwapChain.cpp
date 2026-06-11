//
// Created by pompei on 2026-06-09.
//

#include "SwapChain.h"

#include "../util/util.h"
#include "model/SwapChainSupport.h"

namespace vul {

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
} // namespace vul
