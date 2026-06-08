#pragma once
#include <string>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.h>

namespace prints {

  const char *deviceTypeName(VkPhysicalDeviceType t);
  void        printQueueFamilies(const int tab, const VkPhysicalDevice physicalDevice);
  std::string VkQueueFlags_toStr(const VkQueueFlags flags);
  void        printPhysicalDeviceProperties(const uint32_t deviceIndex, const VkPhysicalDevice physicalDevice);
  void        loadInstanceExtensions(const bool print, std::unordered_set<std::string> *targetExtensions);

} // namespace prints
