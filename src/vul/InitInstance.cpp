//
// Created by pompei on 2026-06-09.
//

#include "InitInstance.h"

#include "Print.h"
#include "util/util.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <sstream>
#include <unordered_set>

namespace {

  std::string messageTypeToStr(const VkDebugUtilsMessageTypeFlagsEXT type)
  {
    std::string str;

    if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT) str += "GENERAL ";
    if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT) str += "VALIDATION ";
    if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT) str += "PERFORMANCE ";
    if (type & VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT) str += "DEVICE_ADDRESS_BINDING ";

    if (str.empty()) return "(NONE)";

    return str;
  }

  VKAPI_ATTR VkBool32 VKAPI_CALL debugCallback(const VkDebugUtilsMessageSeverityFlagBitsEXT severity,
                                               const VkDebugUtilsMessageTypeFlagsEXT        type,
                                               const VkDebugUtilsMessengerCallbackDataEXT  *data,
                                               void                                        *userData)
  {

    if (!userData) return VK_TRUE;

    util::Log         *log = static_cast<util::Log *>(userData);
    std::ostringstream ss;

    ss << messageTypeToStr(type) << " " << data->pMessage;

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
      log->error("z52BtjTM1A", "{}", ss.str());
    } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      log->warn("5kgI0mMfGJ", "{}", ss.str());
    } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
      log->info("yP46Dh9i5J", "{}", ss.str());
    } else {
      log->verbose("haXHF2ksfM", "{}", ss.str());
    }

    return VK_FALSE;
  }
} // namespace

namespace vul {

  void InitInstance::initialize() const
  {
    firstInit_->init();

    createVkInstance();

    if (setting_->validation()) {
      initVkMessenger();
    }

    createVkSdkSurface();

    selectPhysicalDevice_->select();

    createDevice();

    createSwapChain();

    handleStore_->swapChain()->createImageViews();
  }

  void InitInstance::createVkInstance() const
  {
    std::unordered_set<std::string> instanceExtensions;

    print_->loadInstanceExtensions(setting_->isLogLevelVerbose(), &instanceExtensions);

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
    extensions.reserve(extensions.size());
    for (uint32_t i = 0; i < sdlExtCount; ++i) {
      extensions.emplace_back(sdlExtensions[i]);
    }

    std::vector<std::string> validationLayers;

    if (setting_->validation() && instanceExtensions.contains(VK_EXT_DEBUG_UTILS_EXTENSION_NAME)) {
      extensions.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
      validationLayers.emplace_back("VK_LAYER_KHRONOS_validation");
    }

    std::vector<const char *> cExtensions;
    cExtensions.reserve(extensions.size());
    for (const std::string &s : extensions) {
      cExtensions.push_back(s.c_str());
    }

    std::vector<const char *> cValidationLayers;
    cValidationLayers.reserve(validationLayers.size());
    for (const std::string &s : validationLayers) {
      cValidationLayers.push_back(s.c_str());
    }

    VkInstanceCreateInfo createInfo{};
    createInfo.sType                   = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    createInfo.pApplicationInfo        = &appInfo;
    createInfo.enabledExtensionCount   = cExtensions.size();
    createInfo.ppEnabledExtensionNames = cExtensions.data();
    createInfo.enabledLayerCount       = cValidationLayers.size();
    createInfo.ppEnabledLayerNames     = cValidationLayers.data();

    VkInstance vkInstance;
    if (const VkResult result = vkCreateInstance(&createInfo, nullptr, &vkInstance); result != VK_SUCCESS) {
      throw std::runtime_error(std::string("c9Hp80dwaM :: failed to create Vulkan instance: VkResult = ") + util::VkResult_to_str(result));
    }

    handleStore_->storeVkInstance(vkInstance);

    if (util::Log::get()->hasInfo()) util::Log::get()->info("qClyNZccLo", "Vulkan instance created successfully");
  }

  void InitInstance::initVkMessenger() const
  {
    const VkInstance vkInstance = handleStore_->vkInstance();

    VkDebugUtilsMessengerEXT vkMessenger;

    VkDebugUtilsMessengerCreateInfoEXT info{};
    info.sType     = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
    info.pUserData = util::Log::get();

    {
      VkDebugUtilsMessageSeverityFlagsEXT flags = 0;
      if (setting_->isLogLevelError()) flags |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
      if (setting_->isLogLevelWarning()) flags |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT;
      if (setting_->isLogLevelInfo()) flags |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT;
      if (setting_->isLogLevelVerbose()) flags |= VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT;
      info.messageSeverity = flags;
    }

    info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                       VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_DEVICE_ADDRESS_BINDING_BIT_EXT;

    info.pfnUserCallback = debugCallback;

    constexpr char funcName[] = "vkCreateDebugUtilsMessengerEXT";

    const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(vkInstance, funcName));

    if (const VkResult vkResult = func(vkInstance, &info, nullptr, &vkMessenger); vkResult != VK_SUCCESS) {
      util::Log::get()->error("dEt7M1FfiK", "Cannot call func `{}`", funcName);
    }

    handleStore_->storeVkMessenger(vkMessenger);

    if (util::Log::get()->hasInfo()) util::Log::get()->info("7pa1htAJ0T", "VkDebugUtilsMessengerEXT created successfully");
  }

  void InitInstance::createVkSdkSurface() const
  {
    SDL_Window      *mainWindow   = mainWindow_->windowPtr();
    const VkInstance vkInstance   = handleStore_->vkInstance();
    VkSurfaceKHR     vkSdkSurface = VK_NULL_HANDLE;

    // Создаем Vulkan surface для окна SDL.
    if (!SDL_Vulkan_CreateSurface(mainWindow, vkInstance, nullptr, &vkSdkSurface)) {
      throw std::runtime_error(std::string("idXh7XaHT6 :: ERROR IN `SDL_Vulkan_CreateSurface()`: ") + SDL_GetError());
    }

    handleStore_->storeVkSdkSurface(vkSdkSurface);
  }

  void InitInstance::createDevice() const
  {
    const VkSurfaceKHR     vkSdkSurface     = handleStore_->vkSdkSurface();
    const VkPhysicalDevice vkPhysicalDevice = handleStore_->selectedVkPhysicalDevice();

    std::unique_ptr<Device> device = std::make_unique<Device>();
    device->create(vkPhysicalDevice, vkSdkSurface);
    handleStore_->storeDevice(device.release());
  }

  void InitInstance::createSwapChain() const
  {
    const VkSurfaceKHR     vkSdkSurface     = handleStore_->vkSdkSurface();
    const VkPhysicalDevice vkPhysicalDevice = handleStore_->selectedVkPhysicalDevice();
    Device                *device           = handleStore_->device();
    SDL_Window            *mainWindow       = mainWindow_->windowPtr();

    std::unique_ptr<SwapChain> swapChain = std::make_unique<SwapChain>(device->handle());

    swapChain->create(vkPhysicalDevice, vkSdkSurface, mainWindow);

    handleStore_->storeSwapChain(swapChain.release());
  }

} // namespace vul
