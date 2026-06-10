//
// Created by pompei on 2026-06-09.
//

#include "SwapChain.h"

namespace vul {
  void SwapChain::create(const VkPhysicalDevice vkPhysicalDevice, const VkSurfaceKHR vkSdkSurface, SDL_Window *window)
  {
    const model::SwapChainSupport swapChainSupport = model::querySwapChainSupport(vkPhysicalDevice, vkSdkSurface);
    // Выбранный формат поверхности Vulkan.
    const VkSurfaceFormatKHR surfaceFormat         = chooseSwapSurfaceFormat(swapChainSupport.formats);
    // Выбранный режим показа Vulkan.
    const VkPresentModeKHR presentMode             = chooseSwapPresentMode(swapChainSupport.presentModes);
    // Выбранный размер swap-chain Vulkan.
    const VkExtent2D extent                        = chooseSwapExtent(swapChainSupport.capabilities, window);
    uint32_t         imageCount                    = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount) {
      imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    // Параметры создания swap-chain Vulkan.
    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType            = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR; // Тип структуры создания swap-chain.
    createInfo.surface          = vkSdkSurface;                                // Surface окна, для которого создается swap-chain.
    createInfo.minImageCount    = imageCount;                                  // Минимальное количество изображений в swap-chain.
    createInfo.imageFormat      = surfaceFormat.format;                        // Формат пикселей изображений swap-chain.
    createInfo.imageColorSpace  = surfaceFormat.colorSpace;                    // Цветовое пространство изображений swap-chain.
    createInfo.imageExtent      = extent;                                      // Размер изображений swap-chain в пикселях.
    createInfo.imageArrayLayers = 1;                                           // Количество слоев изображения, для обычного 2D окна нужен один.
    createInfo.imageUsage       = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;         // Изображения будут использоваться как color attachment.

    const model::QueueFamilyIndices indices              = model::findQueueFamilies(vkPhysicalDevice, vkSdkSurface);
    const uint32_t                  queueFamilyIndices[] = {*indices.graphicsFamily, *indices.presentFamily};

    if (indices.graphicsFamily != indices.presentFamily) {
      createInfo.imageSharingMode      = VK_SHARING_MODE_CONCURRENT; // Изображения доступны нескольким семействам очередей.
      createInfo.queueFamilyIndexCount = 2;                          // Количество семейств очередей, которым нужен доступ.
      createInfo.pQueueFamilyIndices   = queueFamilyIndices;         // Индексы графического и present семейств очередей.
    } else {
      createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE; // Изображения принадлежат одному семейству очередей.
    }

    createInfo.preTransform   = swapChainSupport.capabilities.currentTransform; // Текущее преобразование surface перед показом.
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;              // Альфа-канал окна не смешивается с другими окнами.
    createInfo.presentMode    = presentMode;                                    // Режим показа изображений на экран.
    createInfo.clipped        = VK_TRUE;                                        // Разрешаем не рисовать скрытые части окна.
    createInfo.oldSwapchain   = VK_NULL_HANDLE;                                 // Старый swap-chain отсутствует при первом создании.

    // Создаем swap-chain Vulkan.
    if (vkCreateSwapchainKHR(vkDevice_, &createInfo, nullptr, &swapChain_) != VK_SUCCESS) {
      throw std::runtime_error("xV5qN8cTpJ :: failed to create swap chain");
    }

    // Запрашиваем количество изображений swap-chain Vulkan.
    vkGetSwapchainImagesKHR(vkDevice_, swapChain_, &imageCount, nullptr);
    swapChainImages_.resize(imageCount);

    // Получаем изображения swap-chain Vulkan.
    vkGetSwapchainImagesKHR(vkDevice_, swapChain_, &imageCount, swapChainImages_.data());

    swapChainImageFormat_ = surfaceFormat.format;
    swapChainExtent_      = extent;
  }

  VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats)
  {
    for (const auto &availableFormat : formats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }

    return formats[0];
  }

  VkPresentModeKHR SwapChain::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes)
  {
    for (const auto &availablePresentMode : presentModes) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, SDL_Window *window)
  {
    if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max()) {
      return capabilities.currentExtent;
    }

    int width  = 0;
    int height = 0;

    // Получаем размер окна SDL в пикселях.
    if (!SDL_GetWindowSizeInPixels(window, &width, &height)) {
      // Получаем текст ошибки SDL.
      throw std::runtime_error(std::string("mE9tH2wKsL :: ") + SDL_GetError());
    }

    // Фактический размер swap-chain Vulkan.
    VkExtent2D actualExtent = {static_cast<uint32_t>(width), static_cast<uint32_t>(height)};
    actualExtent.width      = std::clamp(actualExtent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
    actualExtent.height     = std::clamp(actualExtent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

    return actualExtent;
  }
} // namespace vul