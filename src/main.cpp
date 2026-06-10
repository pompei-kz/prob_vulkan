// ReSharper disable CppUseStructuredBinding
#include "di/Context.h"

#include <SDL3/SDL.h>

int main(int argCount, char *args[])
{

  di::Context context;

  if (context.settings()->readCommandArguments(argCount, args)) {
    return EXIT_FAILURE;
  }

  util::Log::initialize(context.settings());

  context.mainWindow()->create();
  context.instanceInit()->initTopObjects();

  // Working...
  context.mainWindow()->mainLoop();

  context.descriptorStore()->destroy();
  context.firstInit()->destroy();

  return EXIT_SUCCESS;
}
