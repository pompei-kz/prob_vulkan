// ReSharper disable CppUseStructuredBinding
#include "di/Context.h"

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>
#include <iostream>
#include <stdexcept>
#include <unordered_set>
#include <vector>
#include <vulkan/vulkan.h>

int main(int argCount, char *args[])
{

  di::Context context;

  if (context.get_settings()->readCommandArguments(argCount, args)) {
    return EXIT_FAILURE;
  }

  context.get_instanceInit()->initTopObjects();

  context.get_descriptorStore()->destroy();
  context.get_firstInit()->destroy();

  return EXIT_SUCCESS;
}
