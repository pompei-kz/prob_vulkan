//
// Created by pompei on 2026-06-09.
//

#include "Log.h"
#include "util/util.h"
#include <iostream>

namespace vul {
  Log::Log(di::Getter<app::Settings> &setting)
      : setting_(setting)
  {}

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
    return setting_->isLogLevelError();
  }

  [[nodiscard]] inline bool Log::hasWarning() const
  {
    return setting_->isLogLevelWarning();
  }

  [[nodiscard]] inline bool Log::hasInfo() const
  {
    return setting_->isLogLevelInfo();
  }

  [[nodiscard]] inline bool Log::hasVerbose() const
  {
    return setting_->isLogLevelVerbose();
  }
} // namespace vul
