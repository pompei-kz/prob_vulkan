//
// Created by pompei on 2026-06-11.
//

#include "SwapChainStore_Worker.h"

#include <stdexcept>

namespace vul {

  void SwapChainStore_Worker::create() const
  {
    const VkPhysicalDevice vkPhysicalDevice = topStore_->selectedVkPhysicalDevice();
    const VkSurfaceKHR     vkSdkSurface     = topStore_->vkSdkSurface();
    SDL_Window            *window           = mainWindow_->windowPtr();
    const DeviceStore     *deviceStore      = topStore_->device();

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
    const uint32_t                  queueFamilyIndices[] = {*indices.indexGraphics, *indices.indexPresent};

    if (indices.indexGraphics != indices.indexPresent) {
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

    std::unique_ptr<SwapChainStore> swapChainStore = std::make_unique<SwapChainStore>(deviceStore->handle());

    {
      VkSwapchainKHR swapChainLocal;
      // Создаем swap-chain Vulkan.
      if (const VkResult result = vkCreateSwapchainKHR(deviceStore->handle(), &createInfo, nullptr, &swapChainLocal); result != VK_SUCCESS) {
        throw std::runtime_error(std::string("xV5qN8cTpJ :: failed to create swap chain: VkResult = ") + util::VkResult_to_str(result));
      }
      swapChainStore->resetHandle(swapChainLocal);
    }

    {
      std::vector<VkImage> swapChainImages;
      // Запрашиваем количество изображений swap-chain Vulkan.
      vkGetSwapchainImagesKHR(deviceStore->handle(), swapChainStore->handle(), &imageCount, nullptr);
      swapChainImages.resize(imageCount);

      // Получаем изображения swap-chain Vulkan.
      vkGetSwapchainImagesKHR(deviceStore->handle(), swapChainStore->handle(), &imageCount, swapChainImages.data());

      swapChainStore->setEnvironment(swapChainImages, surfaceFormat.format, extent);
    }

    topStore_->resetSwapChain(std::move(swapChainStore));

    if (util::Log::get()->hasVerbose()) {
      util::Log::get()->verbose("tYrHJ2OvO3", "Swap chain created with {}", imageCount);
      util::Log::get()->verbose("bO4usS2XZ1", "Swap chain image format {}", util::VkFormat_to_str(surfaceFormat.format));
      util::Log::get()->verbose("kpPnGzPKiV", "Swap chain color space  {}", util::VkColorSpaceKHR_to_str(surfaceFormat.colorSpace));
    }
  }

  void SwapChainStore_Worker::createImageViews() const
  {
    const VkDevice             device         = topStore_->device()->handle();
    SwapChainStore            *swapChainStore = topStore_->swapChain();
    const std::vector<VkImage> images         = swapChainStore->images();

    std::vector<VkImageView> imageViews;
    imageViews.resize(images.size());

    for (size_t i = 0; i < images.size(); ++i) {
      // Параметры создания image view Vulkan.
      VkImageViewCreateInfo createInfo{};
      createInfo.sType                           = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO; // Тип структуры создания image view.
      createInfo.image                           = images[i];                                // Изображение swap-chain, для которого создается
      createInfo.viewType                        = VK_IMAGE_VIEW_TYPE_2D;                    // Представление изображения как 2D texture.
      createInfo.format                          = swapChainStore->imageFormat();            // Формат данных изображения.
      createInfo.components.r                    = VK_COMPONENT_SWIZZLE_IDENTITY;            // Красный канал остается без перестановки.
      createInfo.components.g                    = VK_COMPONENT_SWIZZLE_IDENTITY;            // Зеленый канал остается без перестановки.
      createInfo.components.b                    = VK_COMPONENT_SWIZZLE_IDENTITY;            // Синий канал остается без перестановки.
      createInfo.components.a                    = VK_COMPONENT_SWIZZLE_IDENTITY;            // Альфа-канал остается без перестановки.
      createInfo.subresourceRange.aspectMask     = VK_IMAGE_ASPECT_COLOR_BIT;                // View обращается к цветовой части изображения.
      createInfo.subresourceRange.baseMipLevel   = 0;                                        // Первый mip level для view.
      createInfo.subresourceRange.levelCount     = 1;                                        // Количество mip levels в view.
      createInfo.subresourceRange.baseArrayLayer = 0;                                        // Первый слой массива изображений.
      createInfo.subresourceRange.layerCount     = 1;                                        // Количество слоев изображения в view.

      // Создаем image view Vulkan для изображения swap-chain.
      if (const VkResult result = vkCreateImageView(device, &createInfo, nullptr, &imageViews[i]); result != VK_SUCCESS) {
        throw std::runtime_error(std::string("rB1mF6zQeW :: failed to create swap chain image views: VkResult = ") + util::VkResult_to_str(result));
      }

      if (util::Log::get()->hasVerbose()) {
        util::Log::get()->verbose("l1uCgx9vH6", "Created swap chain image view {} : {}", i, static_cast<void *>(imageViews[i]));
      }
    }

    swapChainStore->resetImageViews(imageViews);
  }

  VkSurfaceFormatKHR SwapChainStore_Worker::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats)
  {
    for (const auto &availableFormat : formats) {
      if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB && availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
        return availableFormat;
      }
    }

    return formats[0];
  }

  VkPresentModeKHR SwapChainStore_Worker::chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes)
  {
    for (const auto &availablePresentMode : presentModes) {
      if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
        return availablePresentMode;
      }
    }

    return VK_PRESENT_MODE_FIFO_KHR;
  }

  [[nodiscard]] VkExtent2D SwapChainStore_Worker::chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, SDL_Window *window)
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
