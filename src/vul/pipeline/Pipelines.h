//
// Created by pompei on 2026-06-10.
//

#pragma once
#include "Pipeline.h"

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>
#include <vector>

namespace vul::pipeline {

  class Pipelines
  {
    std::vector<std::string>                                   ids_;
    std::unordered_map<std::string, std::unique_ptr<Pipeline>> map_;

  public:
    Pipelines()                             = default;
    Pipelines(const Pipelines &)            = delete;
    Pipelines &operator=(const Pipelines &) = delete;
    Pipelines(Pipelines &&)                 = delete;
    Pipelines &operator=(Pipelines &&)      = delete;

    Pipeline *findById(const std::string &id)
    {
      const auto it = map_.find(id);
      if (it == map_.end()) {
        return nullptr;
      }
      return it->second.get();
    }

    void clear()
    {
      ids_.clear();
      map_.clear();
    }

    void put(const std::string &id, std::unique_ptr<Pipeline> pipeline)
    {
      if (!pipeline) {
        map_.erase(id);
        return;
      }

      map_.insert_or_assign(id, std::move(pipeline));
    }
  };

} // namespace vul::pipeline