//
// Created by pompei on 2026-06-09.
//

// ReSharper disable CppParameterMayBeConst
#include "HandleStore.h"

#include <SDL3/SDL_vulkan.h>

namespace vul {
  HandleStore::~HandleStore()
  {
    resetVkInstance(VK_NULL_HANDLE);
  }

  void HandleStore::mark_SDL_Initialized()
  {
    is_SDL_initialized = true;
  }

  [[nodiscard]] bool HandleStore::is_SDL_Initialized() const
  {
    return is_SDL_initialized;
  }

  void DestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT messenger, const VkAllocationCallbacks *allocator)
  {
    if (const auto func = reinterpret_cast<PFN_vkDestroyDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT"))) {
      func(instance, messenger, allocator);
    }
  }

  void HandleStore::resetVkMessenger(VkDebugUtilsMessengerEXT vkMessenger)
  {
    if (vkDebugMessenger_ && vkInstance_) {
      DestroyDebugUtilsMessengerEXT(vkInstance_, vkDebugMessenger_, nullptr);
      vkDebugMessenger_ = VK_NULL_HANDLE;
    }
    if (vkMessenger) {
      vkDebugMessenger_ = vkMessenger;
    }
  }

  void HandleStore::resetVkSdkSurface(VkSurfaceKHR vkSdkSurface)
  {
    if (vkSdkSurface_) {
      SDL_Vulkan_DestroySurface(vkInstance_, vkSdkSurface_, nullptr);
      vkSdkSurface_ = VK_NULL_HANDLE;
    }
    vkSdkSurface_ = vkSdkSurface;
  }

  void HandleStore::resetVkInstance(VkInstance vkInstance)
  {
    resetDevice(nullptr);

    resetVkSdkSurface(VK_NULL_HANDLE);
    selectVkPhysicalDevice(VK_NULL_HANDLE);
    resetVkMessenger(VK_NULL_HANDLE);
    if (vkInstance_) {
      vkDestroyInstance(vkInstance_, nullptr);
      vkInstance_ = VK_NULL_HANDLE;
    }
    vkInstance_ = vkInstance;
  }

} // namespace vul
