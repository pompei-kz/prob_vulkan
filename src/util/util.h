//
// Created by pompei on 2026-06-08.
//

#pragma once
#include <string>
#include <vulkan/vulkan.h>

namespace util {
  std::string nowStr();

  const char* VkResult_to_str(VkResult result);
}
