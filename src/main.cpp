#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(std::string("4E7viRAzp3 :: failed to init SDL: ") + SDL_GetError());
  }

  VkApplicationInfo appInfo{};
  appInfo.sType              = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  appInfo.pApplicationName   = "prob_vulkan";
  appInfo.applicationVersion = VK_MAKE_VERSION(1, 0, 0);
  appInfo.pEngineName        = "No Engine";
  appInfo.engineVersion      = VK_MAKE_VERSION(1, 0, 0);
  appInfo.apiVersion         = VK_API_VERSION_1_3;

  uint32_t           sdlExtCount   = 0;
  const char *const *sdlExtensions = SDL_Vulkan_GetInstanceExtensions(&sdlExtCount);

  VkInstanceCreateInfo createInfo{};
  createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  createInfo.pApplicationInfo        = &appInfo;
  createInfo.enabledExtensionCount   = sdlExtCount;
  createInfo.ppEnabledExtensionNames = sdlExtensions;
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

  const auto deviceTypeName = [](VkPhysicalDeviceType t) -> const char * {
    switch (t) {
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: return "Integrated GPU";
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: return "Discrete GPU";
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: return "Virtual GPU";
    case VK_PHYSICAL_DEVICE_TYPE_CPU: return "CPU";
    default: return "Other";
    }
  };

  for (uint32_t i = 0; i < deviceCount; ++i) {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(devices[i], &props);

    VkPhysicalDeviceMemoryProperties mem;
    vkGetPhysicalDeviceMemoryProperties(devices[i], &mem);

    uint64_t vramBytes = 0;
    for (uint32_t h = 0; h < mem.memoryHeapCount; ++h) {
      if (mem.memoryHeaps[h].flags & VK_MEMORY_HEAP_DEVICE_LOCAL_BIT) vramBytes += mem.memoryHeaps[h].size;
    }

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, nullptr);
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
    vkGetPhysicalDeviceQueueFamilyProperties(devices[i], &queueFamilyCount, queueFamilies.data());

    std::cout << "\n[Device " << i << "]\n"
              << "  Name:         " << props.deviceName << "\n"
              << "  Type:         " << deviceTypeName(props.deviceType) << "\n"
              << "  API version:  " << VK_VERSION_MAJOR(props.apiVersion) << "." << VK_VERSION_MINOR(props.apiVersion) << "."
              << VK_VERSION_PATCH(props.apiVersion) << "\n"
              << "  Driver ver:   " << props.driverVersion << "\n"
              << "  Vendor ID:    0x" << std::hex << props.vendorID << std::dec << "\n"
              << "  Device ID:    0x" << std::hex << props.deviceID << std::dec << "\n"
              << "  V_RAM:         " << vramBytes / (1024 * 1024) << " MB\n"
              << "  Queue families: " << queueFamilyCount << "\n";

    for (uint32_t q = 0; q < queueFamilyCount; ++q) {
      const auto &qf = queueFamilies[q];
      std::cout << "    [" << q << "] count=" << qf.queueCount << " flags=";
      if (qf.queueFlags & VK_QUEUE_GRAPHICS_BIT) std::cout << "GRAPHICS ";
      if (qf.queueFlags & VK_QUEUE_COMPUTE_BIT) std::cout << "COMPUTE ";
      if (qf.queueFlags & VK_QUEUE_TRANSFER_BIT) std::cout << "TRANSFER ";
      if (qf.queueFlags & VK_QUEUE_SPARSE_BINDING_BIT) std::cout << "SPARSE ";
      std::cout << "\n";
    }
  }

  vkDestroyInstance(instance, nullptr);
  SDL_Quit();
  return 0;
}
