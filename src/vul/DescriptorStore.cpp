//
// Created by pompei on 2026-06-09.
//

// ReSharper disable CppParameterMayBeConst
#include "DescriptorStore.h"

#include <SDL3/SDL_vulkan.h>

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

  void DescriptorStore::storeVkSdkSurface(VkSurfaceKHR vkSdkSurface)
  {
    if (vkSdkSurface_) {
      SDL_Vulkan_DestroySurface(vkInstance_, vkSdkSurface_, nullptr);
      vkSdkSurface_ = VK_NULL_HANDLE;
    }
    vkSdkSurface_ = vkSdkSurface;
  }

  void DescriptorStore::storeVkInstance(VkInstance vkInstance)
  {
    device_.reset();

    storeVkSdkSurface(VK_NULL_HANDLE);
    selectVkPhysicalDevice(VK_NULL_HANDLE);
    storeVkMessenger(VK_NULL_HANDLE);
    if (vkInstance_) {
      vkDestroyInstance(vkInstance_, nullptr);
      vkInstance_ = VK_NULL_HANDLE;
    }
    vkInstance_ = vkInstance;
  }

} // namespace vul
