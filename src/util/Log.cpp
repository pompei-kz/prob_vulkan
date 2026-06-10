//
// Created by pompei on 2026-06-09.
//

#include "Log.h"
#include "util/util.h"
#include <iostream>

namespace {
  util::Log *statis_log;
}

namespace util {
  Log::Log() {}

  Log *Log::get()
  {
    if (statis_log == nullptr) {
      throw std::runtime_error("5bmrSfNLEb :: Log is not initialized");
    }
    return statis_log;
  }

  void Log::initialize(const app::Settings *settings)
  {
    statis_log                     = new Log();
    statis_log->isLogLevelError_   = settings->isLogLevelError();
    statis_log->isLogLevelWarning_ = settings->isLogLevelWarning();
    statis_log->isLogLevelInfo_    = settings->isLogLevelInfo();
    statis_log->isLogLevelVerbose_ = settings->isLogLevelVerbose();
  }

  void Log::error0(const std::string_view placeId, const std::string_view message) const
  {
    if (hasError()) {
      std::cerr << util::nowStr() << " ERROR " << placeId << " :: " << message << std::endl;
    }
  }

  void Log::warn0(const std::string_view placeId, const std::string_view message) const
  {
    if (hasWarning()) {
      std::cerr << util::nowStr() << " WARN " << placeId << " :: " << message << std::endl;
    }
  }

  void Log::info0(const std::string_view placeId, const std::string_view message) const
  {
    if (hasInfo()) {
      std::cout << util::nowStr() << " INFO " << placeId << " :: " << message << std::endl;
    }
  }

  void Log::verbose0(const std::string_view placeId, const std::string_view message) const
  {
    if (hasVerbose()) {
      std::cout << util::nowStr() << " VERB " << placeId << " :: " << message << std::endl;
    }
  }

  [[nodiscard]] inline bool Log::hasError() const
  {
    return isLogLevelError_;
  }

  [[nodiscard]] inline bool Log::hasWarning() const
  {
    return isLogLevelWarning_;
  }

  [[nodiscard]] inline bool Log::hasInfo() const
  {
    return isLogLevelInfo_;
  }

  [[nodiscard]] inline bool Log::hasVerbose() const
  {
    return isLogLevelVerbose_;
  }
} // namespace vul
