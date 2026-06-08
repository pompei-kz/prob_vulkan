// ReSharper disable CppUseStructuredBinding
#include "prints.h"

#include <cstring>
#include <iostream>
#include <string>
#include <vector>

namespace {
  void printVkQueueFlag0(std::string          &target, //
                         const VkQueueFlags    queueFlags,
                         const VkQueueFlagBits flagBit,
                         const char           *display)
  {
    if (queueFlags & flagBit) {
      if (target.length() > 0) {
        target += ' ';
      }
      target += display;
    }
  }
} // namespace

namespace prints {
  const char *deviceTypeName(VkPhysicalDeviceType t)
  {
    switch (t) {
      case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated GPU";
      case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "Discrete GPU";
      case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "Virtual GPU";
      case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
      default: return "Other";
    }
  }

  std::string VkQueueFlags_toStr(const VkQueueFlags flags)
  {
    std::string ret;
    printVkQueueFlag0(ret, flags, VK_QUEUE_GRAPHICS_BIT, "GRAPHICS");
    printVkQueueFlag0(ret, flags, VK_QUEUE_COMPUTE_BIT, "COMPUTE");
    printVkQueueFlag0(ret, flags, VK_QUEUE_TRANSFER_BIT, "TRANSFER");
    printVkQueueFlag0(ret, flags, VK_QUEUE_SPARSE_BINDING_BIT, "SPARSE_BINDING");
    printVkQueueFlag0(ret, flags, VK_QUEUE_PROTECTED_BIT, "PROTECTED");
    printVkQueueFlag0(ret, flags, VK_QUEUE_VIDEO_DECODE_BIT_KHR, "VIDEO_DECODE_KHR");
    printVkQueueFlag0(ret, flags, VK_QUEUE_VIDEO_ENCODE_BIT_KHR, "VIDEO_ENCODE_KHR");
    printVkQueueFlag0(ret, flags, VK_QUEUE_OPTICAL_FLOW_BIT_NV, "OPTICAL_FLOW_NV");
    return ret;
  }

  void printQueueFamilies(const int tab, const VkPhysicalDevice physicalDevice)
  {
    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, nullptr);

    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(physicalDevice, &queueFamilyCount, queueFamilies.data());

    const std::string spaces(tab, ' ');

    for (uint32_t q = 0; q < queueFamilyCount; ++q) {
      const VkQueueFamilyProperties &qfp = queueFamilies[q];

      std::cout << spaces << "[" << q << "] count=" << qfp.queueCount << " flags=" << VkQueueFlags_toStr(qfp.queueFlags) << std::endl;
    }
  }

  void printPhysicalDeviceProperties(const uint32_t deviceIndex, const VkPhysicalDevice physicalDevice)
  {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(physicalDevice, &props);

    VkPhysicalDeviceMemoryProperties mem;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice, &mem);

    uint64_t vRamBytes = 0;
    for (uint32_t h = 0; h < mem.memoryHeapCount; ++h) {
      if (mem.memoryHeaps[h].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) vRamBytes += mem.memoryHeaps[h].size;
    }

    std::cout << "\n[Device " << deviceIndex << "]\n"
              << "  Name:         " << props.deviceName << "\n"
              << "  Type:         " << prints::deviceTypeName(props.deviceType) << "\n"
              << "  API version:  " << VK_VERSION_MAJOR(props.apiVersion) << "." << VK_VERSION_MINOR(props.apiVersion) << "."
              << VK_VERSION_PATCH(props.apiVersion) << "\n"
              << "  Driver ver:   " << props.driverVersion << "\n"
              << "  Vendor ID:    0x" << std::hex << props.vendorID << std::dec << "\n"
              << "  Device ID:    0x" << std::hex << props.deviceID << std::dec << "\n"
              << "  V_RAM:        " << vRamBytes / (1024 * 1024) << " MB\n";

    printQueueFamilies(4, physicalDevice);
  }

  void loadInstanceExtensions(const bool print, std::unordered_set<std::string> *targetExtensions)
  {
    if (!print && !targetExtensions) {
      return;
    }

    uint32_t extensionCount = 0;
    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, nullptr);

    std::vector<VkExtensionProperties> extensions(extensionCount);

    vkEnumerateInstanceExtensionProperties(nullptr, &extensionCount, extensions.data());

    size_t maxLength = 0;
    for (const VkExtensionProperties &ext : extensions) {
      if (const auto len = std::strlen(ext.extensionName); maxLength < len) {
        maxLength = len;
      }
      if (targetExtensions) {
        targetExtensions->emplace(ext.extensionName);
      }
    }

    if (!print) return;

    std::cout << "ZT9Fn6bsPg :: Instance extensions:" << std::endl;
    for (const VkExtensionProperties &ext : extensions) {
      const size_t len = std::strlen(ext.extensionName);
      std::string  spaces(maxLength - len, ' ');
      std::cout << "  " << ext.extensionName << spaces << " ( specVersion = " << ext.specVersion << " )" << std::endl;
    }
    std::cout << std::endl;
  }

} // namespace prints
