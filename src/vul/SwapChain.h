//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "../util/Log.h"
#include "model/QueueFamilyIndices.h"

#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

namespace vul {

  class SwapChain
  {
    VkDevice vkDevice_;

    /**
     * Дескриптор цепочки обмена Vulkan.
     */
    VkSwapchainKHR handle_ = VK_NULL_HANDLE;

    /**
     * Изображения swap-chain Vulkan.
     */
    std::vector<VkImage> images_;

    /**
     * Формат изображений swap-chain Vulkan.
     */
    VkFormat imageFormat_ = VK_FORMAT_UNDEFINED;

    /**
     * Размер изображений swap-chain Vulkan.
     */
    VkExtent2D extent_{};

    /**
     * Image views Vulkan для изображений swap-chain.
     */
    std::vector<VkImageView> imageViews_;

  public:
    explicit SwapChain(const VkDevice vkDevice)
        : vkDevice_(vkDevice)
    {
      if (!vkDevice_) {
        throw std::runtime_error("MZ8mz9ATtR :: Vulkan device is VK_NULL_HANDLE");
      }
    }

    ~SwapChain()
    {
      resetHandle(VK_NULL_HANDLE);
    }

    void resetHandle(const VkSwapchainKHR handle);
    void resetImageViews(const std::vector<VkImageView> &imageViews);

    [[nodiscard]] VkSwapchainKHR handle() const
    {
      return handle_;
    }

    void setEnvironment(const std::vector<VkImage> &images, const VkFormat imageFormat, const VkExtent2D extent)
    {
      images_      = images;
      imageFormat_ = imageFormat;
      extent_      = extent;
    }

    std::vector<VkImage> images() const
    {
      return images_;
    }

    VkFormat imageFormat() const
    {
      return imageFormat_;
    }

    VkExtent2D extent() const
    {
      return extent_;
    }

    std::vector<VkImage> swapChainImages() const
    {
      return images_;
    }

    std::vector<VkImageView> imageViews() const
    {
      return imageViews_;
    }
  };

} // namespace vul
