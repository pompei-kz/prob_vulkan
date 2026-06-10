//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "model/QueueFamilyIndices.h"
#include "model/SwapChainSupport.h"

#include <SDL3/SDL_video.h>
#include <algorithm>
#include <iostream>
#include <limits>
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
    VkSwapchainKHR swapChain_ = VK_NULL_HANDLE;

    /**
     * Изображения swap-chain Vulkan.
     */
    std::vector<VkImage> swapChainImages_;

    /**
     * Формат изображений swap-chain Vulkan.
     */
    VkFormat swapChainImageFormat_ = VK_FORMAT_UNDEFINED;

    /**
     * Размер изображений swap-chain Vulkan.
     */
    VkExtent2D swapChainExtent_{};

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
      storeSwapChain(VK_NULL_HANDLE);
    }

    void storeSwapChain(const VkSwapchainKHR swapChain)
    {
      if (swapChain_) {
        vkDestroySwapchainKHR(vkDevice_, swapChain_, nullptr);
        std::cout << "ekHpQm9sK4 :: vkDestroySwapchainKHR --------------------------------------------------------------------"<<std::endl;
        swapChain_ = VK_NULL_HANDLE;
      }
      swapChain_ = swapChain;
    }

    void create(const VkPhysicalDevice vkPhysicalDevice, const VkSurfaceKHR vkSdkSurface, SDL_Window *window);

  private:
    static VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);

    static VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes);

    [[nodiscard]] static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, SDL_Window *window);
  };

} // namespace vul
