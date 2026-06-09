//
// Created by pompei on 2026-06-08.
//

#include "app/Settings.h"
#include <CLI/CLI.hpp>

constexpr static const char *const ERROR   = "ERROR";
constexpr static const char *const WARNING = "WARNING";
constexpr static const char *const INFO    = "INFO";
constexpr static const char *const VERBOSE = "VERBOSE";

bool app::Settings::readCommandArguments(const int argCount, const char *const args[])
{
  CLI::App app{"Probe of Vulkan"};

  app.add_flag("--validation", validation_, "Enable VK_LAYER_KHRONOS_validation");

  std::string logLevel = "";

  app.add_option("--log", logLevel, "Log level")->check(CLI::IsMember({ERROR, WARNING, INFO, VERBOSE}));

  try {
    app.parse(argCount, args);

    applyLogLevel(logLevel);

    return false;
  } catch (const CLI::CallForHelp &) {
    std::cout << app.help() << '\n';
    return true; // не продолжать выполнение
  } catch (const CLI::ParseError &e) {
    std::cerr << "K5PwRGl6fx :: ARG_ERR " << e.what() << '\n';
    return true; // ошибка
  }
}

void app::Settings::applyLogLevel(const std::string &logLevel)
{
  if (logLevel == ERROR) {
    isLogLevelError_   = true;
    isLogLevelWarning_ = false;
    isLogLevelInfo_    = false;
    isLogLevelVerbose_ = false;
    return;
  }
  if (logLevel == WARNING) {
    isLogLevelError_   = true;
    isLogLevelWarning_ = true;
    isLogLevelInfo_    = false;
    isLogLevelVerbose_ = false;
    return;
  }
  if (logLevel == INFO) {
    isLogLevelError_   = true;
    isLogLevelWarning_ = true;
    isLogLevelInfo_    = true;
    isLogLevelVerbose_ = false;
    return;
  }
  if (logLevel == VERBOSE) {
    isLogLevelError_   = true;
    isLogLevelWarning_ = true;
    isLogLevelInfo_    = true;
    isLogLevelVerbose_ = true;
    return;
  }
}
