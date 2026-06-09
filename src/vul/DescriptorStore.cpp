//
// Created by pompei on 2026-06-09.
//

// ReSharper disable CppParameterMayBeConst
#include "DescriptorStore.h"

namespace vul {
  DescriptorStore::~DescriptorStore()
  {
    storeVkInstance(VK_NULL_HANDLE);
  }

  void DescriptorStore::mark_SDL_Initialized()
  {
    is_SDL_initialized = true;
  }

  [[nodiscard]] bool DescriptorStore::is_SDL_Initialized() const
  {
    return is_SDL_initialized;
  }

  void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *allocator)
  {
    if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"))) {
      func(instance, messenger, allocator);
    }
  }

  void DescriptorStore::storeVkMessenger(VkDebugUtilsMessengerEXT vkMessenger)
  {
    if (vkDebugMessenger_ && vkInstance_) {
      DestroyDebugUtilsMessengerEXT(vkInstance_, vkDebugMessenger_, nullptr);
      vkDebugMessenger_ = VK_NULL_HANDLE;
    }
    if (vkMessenger) {
      vkDebugMessenger_ = vkMessenger;
    }
  }

  void DescriptorStore::storeVkInstance(VkInstance vkInstance)
  {
    storeVkPhysicalDevice(VK_NULL_HANDLE);
    storeVkMessenger(VK_NULL_HANDLE);
    if (vkInstance_) {
      vkDestroyInstance(vkInstance_, nullptr);
      vkInstance_ = VK_NULL_HANDLE;
    }
    vkInstance_ = vkInstance;
  }

  void DescriptorStore::storeVkPhysicalDevice(VkPhysicalDevice vkPhysicalDevice)
  {
    vkPhysicalDevice_ = vkPhysicalDevice;
  }

  [[nodiscard]] inline VkInstance DescriptorStore::vkInstance() const
  {
    return vkInstance_;
  }

  [[nodiscard]] inline VkPhysicalDevice DescriptorStore::vkPhysicalDevice() const
  {
    return vkPhysicalDevice_;
  }
} // namespace vul
