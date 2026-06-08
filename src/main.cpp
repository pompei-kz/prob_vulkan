// ReSharper disable CppUseStructuredBinding
#include "prints.h"
#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.h>

VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback(VkDebugUtilsMessageSeverityFlagBitsEXT      severity,
                                             VkDebugUtilsMessageTypeFlagsEXT             type,
                                             const VkDebugUtilsMessengerCallbackDataEXT *data,
                                             void                                       *userData)
{
  std::cerr << data->pMessage << '\n';

  return VK_FALSE;
}

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(std::string("4E7viRAzp3 :: failed to init SDL: ") + SDL_GetError());
  }

  std::unordered_set<std::string> instanceExtensions;

  prints::loadInstanceExtensions(true, &instanceExtensions);

  VkApplicationInfo appInfo{};
  appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName   = "prob_vulkan";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName        = "No Engine";
  appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion         = VK_API_VERSION_1_3;

  uint32_t           sdlExtCount   = 0;
  const char *const *sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&sdlExtCount);

  std::vector<std::string> extensions;
  for (uint32_t i = 0; i < sdlExtCount; ++i) {
    extensions.push_back(sdlExtensions[i]);
  }

  std::vector<std::string> validationLayers;

  if (instanceExtensions.contains(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
    extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    validationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
  }

  std::vector<const char *> cExtensions;
  for (const std::string &s : extensions) {
    cExtensions.push_back(s.c_str());
  }

  VkInstanceCreateInfo createInfo{};
  createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo        = &appInfo;
  createInfo.enabledExtensionCount   = cExtensions.size();
  createInfo.ppEnabledExtensionNames = cExtensions.data();
  createInfo.enabledLayerCount       = 0;

  VkInstance instance;
  if (vkCreateInstance(&createInfo, nullptr, &instance) != VK_SUCCESS) {
    throw std::runtime_error("c9Hp80dwaM :: failed to create Vulkan instance");
  }

  std::cout << "qClyNZccLo :: Vulkan instance created successfully\n";

  uint32_t deviceCount = 0;
  vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
  std::cout << "8nd8ta7Wu1 :: Physical devices found: " << deviceCount << "\n";

  std::vector<VkPhysicalDevice> devices(deviceCount);
  vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

  for (uint32_t i = 0; i < deviceCount; ++i) {
    prints::printPhysicalDeviceProperties(i, devices[i]);
  }

  vkDestroyInstance(instance, nullptr);
  SDL_Quit();
  return 0;
}
