//
// Created by pompei on 2026-06-09.
//

#pragma once
#include "app/Settings.h"
#include "di/Getter.h"

#include <format>

namespace vul {
  class Log
  /// BEAN log app::Settings
  {
    di::Getter<app::Settings> &setting_;

  public:
    explicit Log(di::Getter<app::Settings> &setting);

    void error0(const std::string_view placeId, const std::string_view message) const;
    void warn0(const std::string_view placeId, const std::string_view message) const;
    void info0(const std::string_view placeId, const std::string_view message) const;
    void verbose0(const std::string_view placeId, const std::string_view message) const;

    [[nodiscard]] bool hasError() const;
    [[nodiscard]] bool hasWarning() const;
    [[nodiscard]] bool hasInfo() const;
    [[nodiscard]] bool hasVerbose() const;

    template <typename... Args> //
    void error(const std::string_view placeId, std::format_string<Args...> fmt, Args &&...args)
    {
      error0(placeId, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args> //
    void warn(const std::string_view placeId, std::format_string<Args...> fmt, Args &&...args)
    {
      warn0(placeId, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args> //
    void info(const std::string_view placeId, std::format_string<Args...> fmt, Args &&...args)
    {
      info0(placeId, std::format(fmt, std::forward<Args>(args)...));
    }

    template <typename... Args> //
    void verbose(const std::string_view placeId, std::format_string<Args...> fmt, Args &&...args)
    {
      verbose0(placeId, std::format(fmt, std::forward<Args>(args)...));
    }
  };
} // namespace vul
