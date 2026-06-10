// ReSharper disable CppUseStructuredBinding
#include "di/Context.h"

int main(const int argCount, char *args[])
{

  di::Context context;

  if (context.settings()->readCommandArguments(argCount, args)) {
    return EXIT_FAILURE;
  }

  util::Log::initialize(context.settings());

  context.application()->run();

  return EXIT_SUCCESS;
}
