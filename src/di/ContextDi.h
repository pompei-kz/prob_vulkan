#pragma once
#include "app/AppInitializer.h"
#include "di/Getter.h"

#include <shared_mutex>

namespace di {
  class ContextDi
  {
    template <typename T> static T *finish_create(T *result)
    {
      if constexpr (requires { result->initialize(); }) {
        result->initialize();
      }

      return result;
    }

    mutable std::shared_mutex mutex_;

    di::Getter<app::AppInitializer> appInitializer_{&mutex_, [this] { return create_appInitializer(); }};
    di::Getter<app::Settings>       settings_{&mutex_, [this] { return create_settings(); }};
    di::Getter<vul::Print>          print_{&mutex_, [this] { return create_print(); }};

  public:
    app::Settings *get_settings()
    {
      return settings_.get();
    }

    app::AppInitializer *get_appInitializer()
    {
      return appInitializer_.get();
    }

  private:
    app::AppInitializer *create_appInitializer();
    app::Settings       *create_settings();
    vul::Print          *create_print();
  };
} // namespace di
