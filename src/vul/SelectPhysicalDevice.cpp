//
// Created by pompei on 2026-06-09.
//

#include "SelectPhysicalDevice.h"

#include <set>

namespace vul {

  void SelectPhysicalDevice::select() const
  {
    const VkInstance vkInstance = descriptorStore_->vkInstance();

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

    if (deviceCount == 0) {
      throw std::runtime_error("Xm4Pq9rT2k :: No Vulkan-capable physical devices found");
    }

    if (log_->hasInfo()) {
      log_->info("8nd8ta7Wu1", "Physical devices found: {}", deviceCount);
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    if (setting_->isLogLevelVerbose()) {
      for (uint32_t i = 0; i < deviceCount; ++i) {
        print_->printPhysicalDeviceProperties(i, devices[i]);
      }
    }

    VkPhysicalDevice bestDevice = VK_NULL_HANDLE;
    uint64_t         bestScore  = 10uLL;
    for (const VkPhysicalDevice device : devices) {
      if (const uint64_t score = scoreDevice(device); score > bestScore) {
        bestScore  = score;
        bestDevice = device;
      }
    }

    if (bestDevice == VK_NULL_HANDLE) {
      throw std::runtime_error("Yp3Lr8wNbX :: No suitable physical device found (requires graphics queue)");
    }

    descriptorStore_->selectVkPhysicalDevice(bestDevice);

    if (log_->hasInfo()) {
      VkPhysicalDeviceProperties props;
      vkGetPhysicalDeviceProperties(bestDevice, &props);

      log_->info("igIRqIKRBd", "Selected physical device: {}", props.deviceName);
    }
  }

  uint64_t SelectPhysicalDevice::scoreDevice(const VkPhysicalDevice vkPhysicalDevice) const
  {
    if (!isDeviceSuitable(vkPhysicalDevice)) return 0;

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(vkPhysicalDevice, &props);

    VkPhysicalDeviceMemoryProperties mem;
    vkGetPhysicalDeviceMemoryProperties(vkPhysicalDevice, &mem);

    uint64_t vRamBytes = 0;
    for (uint32_t h = 0; h < mem.memoryHeapCount; ++h) {
      if (mem.memoryHeaps[h].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) {
        vRamBytes += mem.memoryHeaps[h].size;
      }
    }

    uint64_t typeScore = 0;
    switch (props.deviceType) {
      case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: typeScore = 1'000'000'000'000ULL; break;
      case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: typeScore = 100'000'000'000ULL; break;
      case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: typeScore = 10'000'000'000ULL; break;
      default: typeScore = 0ULL; break;
    }

    return typeScore + vRamBytes;
  }

  SwapChainSupport SelectPhysicalDevice::querySwapChainSupport(const VkPhysicalDevice device) const
  {
    const VkSurfaceKHR vkSdkSurface = descriptorStore_->vkSdkSurface();

    SwapChainSupport details;
    // Получаем ограничения Vulkan surface для swap-chain.
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, vkSdkSurface, &details.capabilities);

    uint32_t formatCount = 0;
    // Запрашиваем количество форматов Vulkan surface.
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSdkSurface, &formatCount, nullptr);
    if (formatCount != 0) {
      details.formats.resize(formatCount);
      // Получаем доступные форматы Vulkan surface.
      vkGetPhysicalDeviceSurfaceFormatsKHR(device, vkSdkSurface, &formatCount, details.formats.data());
    }

    uint32_t presentModeCount = 0;
    // Запрашиваем количество режимов показа Vulkan surface.
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSdkSurface, &presentModeCount, nullptr);
    if (presentModeCount != 0) {
      details.presentModes.resize(presentModeCount);
      // Получаем доступные режимы показа Vulkan surface.
      vkGetPhysicalDeviceSurfacePresentModesKHR(device, vkSdkSurface, &presentModeCount, details.presentModes.data());
    }

    return details;
  }

  bool SelectPhysicalDevice::isDeviceSuitable(const VkPhysicalDevice vkPhysicalDevice) const
  {

    const VkSurfaceKHR vkSdkSurface = descriptorStore_->vkSdkSurface();

    const model::QueueFamilyIndices indices             = model::findQueueFamilies(vkPhysicalDevice, vkSdkSurface);
    const bool                      extensionsSupported = checkDeviceExtensionSupport(vkPhysicalDevice);
    bool                            swapChainAdequate   = false;

    if (extensionsSupported) {
      const SwapChainSupport swapChainSupport = querySwapChainSupport(vkPhysicalDevice);
      swapChainAdequate                       = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty();
    }

    return indices.complete() && extensionsSupported && swapChainAdequate;
  }

  bool SelectPhysicalDevice::checkDeviceExtensionSupport(const VkPhysicalDevice vkPhysicalDevice)
  {
    uint32_t extensionCount = 0;
    // Запрашиваем количество расширений устройства Vulkan.
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, nullptr);

    // Список доступных расширений устройства Vulkan.
    std::vector<VkExtensionProperties> availableExtensions(extensionCount);
    // Получаем список расширений устройства Vulkan.
    vkEnumerateDeviceExtensionProperties(vkPhysicalDevice, nullptr, &extensionCount, availableExtensions.data());

    std::set<std::string> requiredExtensions(model::K_DEVICE_EXTENSIONS.begin(), model::K_DEVICE_EXTENSIONS.end());
    // ReSharper disable once CppUseStructuredBinding
    for (const VkExtensionProperties &extension : availableExtensions) {
      requiredExtensions.erase(extension.extensionName);
    }

    return requiredExtensions.empty();
  }
} // namespace vul
