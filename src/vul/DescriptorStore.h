//
// Created by pompei on 2026-06-09.
//

#pragma once
#include <vulkan/vulkan.h>

namespace vul {
  /**
   * Stores descriptor of Vulkan and destroys its
   */
  class DescriptorStore
  /// BEAN descriptorStore
  {
    bool is_SDL_initialized = false;

    VkDebugUtilsMessengerEXT vkDebugMessenger_ = VK_NULL_HANDLE;
    VkInstance               vkInstance_       = VK_NULL_HANDLE;

  public:
    ~DescriptorStore();

    void mark_SDL_Initialized();

    bool is_SDL_Initialized() const;

    /**
     * Stores descriptor of VkDebugUtilsMessenger.
     *
     * Old descriptor will be destroyed.
     *
     * It is assumed that this descriptor is genuine - sent from the Vulkan driver.
     *
     * @param vkMessenger descriptor of VkDebugUtilsMessenger
     */
    void storeVkMessenger(VkDebugUtilsMessengerEXT vkMessenger);

    /**
     *
     * Stores descriptor of VkInstance.
     *
     * Old descriptor will be destroyed with all its dependants.
     *
     * It is assumed that this descriptor is genuine - sent from the Vulkan driver.
     *
     * @param vkInstance descriptor of VkInstance
     */
    void storeVkInstance(VkInstance vkInstance);
  };
} // namespace vul
