//
// Created by pompei on 2026-06-08.
//

#pragma once
#include <string>

namespace app {
  /// BEAN settings
  class Settings
  {

    bool validation_ = false;

    bool isLogLevelError_   = false;
    bool isLogLevelWarning_ = false;
    bool isLogLevelInfo_    = false;
    bool isLogLevelVerbose_ = false;

  public:
    bool readCommandArguments(const int argCount, const char *const args[]);

    [[nodiscard]] bool validation() const
    {
      return validation_;
    }

    [[nodiscard]] bool isLogLevelError() const
    {
      return isLogLevelError_;
    }

    [[nodiscard]] bool isLogLevelWarning() const
    {
      return isLogLevelWarning_;
    }

    [[nodiscard]] bool isLogLevelInfo() const
    {
      return isLogLevelInfo_;
    }

    [[nodiscard]] bool isLogLevelVerbose() const
    {
      return isLogLevelVerbose_;
    }

  private:
    void applyLogLevel(const std::string &logLevel);
  };
} // namespace app
