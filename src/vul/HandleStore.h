//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "Device.h"
#include "pipeline/Pipeline.h"
#include "pipeline/Pipelines.h"

#include <memory>
#include <vulkan/vulkan.h>

namespace vul {

  /**
   * Stores handle of Vulkan and destroys its
   */
  class HandleStore
  /// BEAN handleStore
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

    std::unique_ptr<Device> device_;

    std::unique_ptr<SwapChain> swapChain_;

    pipeline::Pipelines pipelines_;

  public:
    ~HandleStore();

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
    void storeVkInstance(VkInstance vkInstance);

    /**
     * Stores handle of VkDebugUtilsMessenger.
     *
     * Old handle will be destroyed.
     *
     * It is assumed that this handle is genuine - sent from the Vulkan driver.
     *
     * @param vkMessenger handle of VkDebugUtilsMessenger
     */
    void storeVkMessenger(VkDebugUtilsMessengerEXT vkMessenger);

    /**
     * Stores the Vulkan surface handle for the window.
     *
     * @param vkSdkSurface Vulkan surface handle for the window.
     */
    void storeVkSdkSurface(VkSurfaceKHR vkSdkSurface);

    [[nodiscard]] VkSurfaceKHR VkSdkSurface() const
    {
      return vkSdkSurface_;
    }

    /**
     * Stores handle of the selected VkPhysicalDevice.
     *
     * Vulkan driver owns VkPhysicalDevice — it cannot be destroyed explicitly.
     *
     * @param selectedVkPhysicalDevice handle of the selected physical device
     */
    void selectVkPhysicalDevice(const VkPhysicalDevice selectedVkPhysicalDevice)
    {
      selectedVkPhysicalDevice_ = selectedVkPhysicalDevice;
    }

    [[nodiscard]] VkInstance vkInstance() const
    {
      return vkInstance_;
    }

    [[nodiscard]] VkPhysicalDevice selectedVkPhysicalDevice() const
    {
      return selectedVkPhysicalDevice_;
    }

    void destroy()
    {
      storeVkInstance(VK_NULL_HANDLE);
    }

    [[nodiscard]] VkSurfaceKHR vkSdkSurface() const
    {
      return vkSdkSurface_;
    }

    Device *device() const
    {
      return device_.get();
    }

    void storeDevice(Device *device)
    {
      if (device_.get() == device) return;

      storeSwapChain(nullptr);
      pipelines_.clear();
      device_.reset(device);
    }

    void storeSwapChain(SwapChain *swapChain)
    {
      swapChain_.reset(swapChain);
    }

    SwapChain *swapChain() const
    {
      return swapChain_.get();
    }

    pipeline::Pipelines *pipelines()
    {
      return &pipelines_;
    }
  };
} // namespace vul
