// ReSharper disable CppUseStructuredBinding
#include "vul/Print.h"
#include "di/ContextDi.h"

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
  std::cerr << data->pMessage << std::endl;

  return VK_FALSE;
}

int main()
{
  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(std::string("4E7viRAzp3 :: failed to init SDL: ") + SDL_GetError());
  }

  di::ContextDi context;

  app::AppInitializer *appInitializer = context.get_appInitializer();

  appInitializer->init();

  return 0;
}
