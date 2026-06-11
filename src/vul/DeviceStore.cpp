//
// Created by pompei on 2026-06-09.
//

#include "DeviceStore.h"

#include "../util/util.h"
#include "model/QueueFamilyIndices.h"

#include <set>
#include <stdexcept>
#include <vector>

namespace vul {

  void DeviceStore::resetHandle(const VkDevice handle)
  {
    if (handle_) {
      // Уничтожаем логическое устройство Vulkan.
      vkDestroyDevice(handle_, nullptr);
      handle_ = VK_NULL_HANDLE;
    }
    handle_ = handle;
  }

} // namespace vul