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
  context.instanceInit()->initialize();

  context.mainWindow()->installCmdExecutor([&context](const cmd::CmdPtr &cmdPtr) { context.executeCmd()->execute_Cmd(cmdPtr); });

  const cmd::CmdPtr startCmd = context.backStart()->start();

  //
  //
  //
  context.mainWindow()->mainLoop(startCmd);
  //
  //
  //

  context.descriptorStore()->destroy();
  context.firstInit()->destroy();

  return EXIT_SUCCESS;
}
