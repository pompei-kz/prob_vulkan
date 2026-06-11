//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "DeviceStore.h"
#include "pipeline/Pipeline.h"
#include "pipeline/Pipelines.h"

#include <memory>

namespace vul {

  /**
   * Stores handles of top Vulkan objects and destroys them
   */
  class TopStore
  /// BEAN topStore
  {
    bool is_SDL_initialized = false;

    /**
     * Дескриптор экземпляра Vulkan.
     */
    VkInstance vkInstance_ = VK_NULL_HANDLE;

    /**
     * Дескриптор системы вывода сообщений валидации Vulkan.
     */
    VkDebugUtilsMessengerEXT vkDebugMessenger_ = VK_NULL_HANDLE;

    /**
     * Дескриптор поверхности Vulkan для окна SDL.
     */
    VkSurfaceKHR vkSdkSurface_ = VK_NULL_HANDLE;

    /**
     * Выбранное физическое устройство Vulkan.
     */
    VkPhysicalDevice selectedVkPhysicalDevice_ = VK_NULL_HANDLE;

    std::unique_ptr<DeviceStore> deviceStore_;

    std::unique_ptr<SwapChainStore> swapChainStore_;

  public:
    ~TopStore();

    void mark_SDL_Initialized();

    [[nodiscard]] bool is_SDL_Initialized() const;

    /**
     *
     * Stores handle of VkInstance.
     *
     * Old handle will be destroyed with all its dependants.
     *
     * It is assumed that this handle is genuine - sent from the Vulkan driver.
     *
     * @param vkInstance handle of VkInstance
     */
    void resetVkInstance(VkInstance vkInstance);

    /**
     * Stores handle of VkDebugUtilsMessenger.
     *
     * Old handle will be destroyed.
     *
     * It is assumed that this handle is genuine - sent from the Vulkan driver.
     *
     * @param vkMessenger handle of VkDebugUtilsMessenger
     */
    void resetVkMessenger(VkDebugUtilsMessengerEXT vkMessenger);

    /**
     * Stores the Vulkan surface handle for the window.
     *
     * @param vkSdkSurface Vulkan surface handle for the window.
     */
    void resetVkSdkSurface(VkSurfaceKHR vkSdkSurface);

    [[nodiscard]] VkSurfaceKHR VkSdkSurface() const { return vkSdkSurface_; }

    /**
     * Stores handle of the selected VkPhysicalDevice.
     *
     * Vulkan driver owns VkPhysicalDevice — it cannot be destroyed explicitly.
     *
     * @param selectedVkPhysicalDevice handle of the selected physical device
     */
    void selectVkPhysicalDevice(const VkPhysicalDevice selectedVkPhysicalDevice) { selectedVkPhysicalDevice_ = selectedVkPhysicalDevice; }

    [[nodiscard]] VkInstance vkInstance() const { return vkInstance_; }

    [[nodiscard]] VkPhysicalDevice selectedVkPhysicalDevice() const { return selectedVkPhysicalDevice_; }

    void destroy() { resetVkInstance(VK_NULL_HANDLE); }

    [[nodiscard]] VkSurfaceKHR vkSdkSurface() const { return vkSdkSurface_; }

    DeviceStore *device() const { return deviceStore_.get(); }

    void resetDevice(std::unique_ptr<DeviceStore> deviceStore)
    {
      resetSwapChain(nullptr);
      deviceStore_ = std::move(deviceStore);
    }

    void resetSwapChain(std::unique_ptr<SwapChainStore> swapChainStore) { swapChainStore_ = std::move(swapChainStore); }

    SwapChainStore *swapChain() const { return swapChainStore_.get(); }

  };
} // namespace vul
