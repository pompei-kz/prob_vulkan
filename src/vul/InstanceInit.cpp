//
// Created by pompei on 2026-06-09.
//

#include "InstanceInit.h"

#include "Print.h"
#include "util/util.h"

#include <SDL3/SDL_init.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
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
                                               // ReSharper disable once CppParameterNeverUsed
                                               void *userData)
  {
    std::ostream *out;
    std::string   severityStr;

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
      out         = &std::cerr;
      severityStr = " ERROR";
    } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
      out         = &std::cerr;
      severityStr = " WARNING";
    } else if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
      out         = &std::cout;
      severityStr = " INFO";
    } else {
      out         = &std::cout;
      severityStr = " VERBOSE";
    }

    *out << util::nowStr() << severityStr << " " << messageTypeToStr(type) << " " << data->pMessage << std::endl;

    return VK_FALSE;
  }
} // namespace

namespace vul {

  InstanceInit::InstanceInit(di::Getter<DescriptorStore> &descriptorStore, di::Getter<app::Settings> &setting, di::Getter<Print> &print)
      : descriptorStore_(descriptorStore)
      , setting_(setting)
      , print_(print)
  {}

  void InstanceInit::initSDL() const
  {
    if (descriptorStore_->is_SDL_Initialized()) return;

    if (!SDL_Init(SDL_INIT_VIDEO)) {
      throw std::runtime_error(std::string("4E7viRAzp3 :: failed to init SDL: ") + SDL_GetError());
    }

    descriptorStore_->mark_SDL_Initialized();
  }

  void InstanceInit::initTopInstance() const
  {
    initSDL();

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
    if (vkCreateInstance(&createInfo, nullptr, &vkInstance) != VK_SUCCESS) {
      throw std::runtime_error("c9Hp80dwaM :: failed to create Vulkan instance");
    }

    descriptorStore_->storeVkInstance(vkInstance);

    if (setting_->isLogLevelInfo()) {
      std::cout << "qClyNZccLo :: Vulkan instance created successfully\n";
    }

    VkDebugUtilsMessengerEXT vkMessenger;

    VkDebugUtilsMessengerCreateInfoEXT info{};
    info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;

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

    const auto func = reinterpret_cast<PFN_vkCreateDebugUtilsMessengerEXT>(vkGetInstanceProcAddr(vkInstance, "vkCreateDebugUtilsMessengerEXT"));

    if (const VkResult vkResult = func(vkInstance, &info, nullptr, &vkMessenger); vkResult != VK_SUCCESS) {
      std::cerr << "dEt7M1FfiK :: Cannot call func `vkCreateDebugUtilsMessengerEXT`" << std::endl;
    }

    descriptorStore_->storeVkMessenger(vkMessenger);

    uint32_t deviceCount = 0;
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, nullptr);

    if (setting_->isLogLevelInfo()) {
      std::cout << "8nd8ta7Wu1 :: Physical devices found: " << deviceCount << "\n";
    }

    std::vector<VkPhysicalDevice> devices(deviceCount);
    vkEnumeratePhysicalDevices(vkInstance, &deviceCount, devices.data());

    if (setting_->isLogLevelInfo()) {
      for (uint32_t i = 0; i < deviceCount; ++i) {
        print_->printPhysicalDeviceProperties(i, devices[i]);
      }
    }
  }

} // namespace vul
