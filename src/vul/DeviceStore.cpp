//
// Created by pompei on 2026-06-09.
//

#include "DeviceStore.h"

#include "../util/util.h"
#include "model/QueueFamilyIndices.h"
#include "util/Log.h"

#include <set>
#include <stdexcept>
#include <vector>

namespace vul {

  void DeviceStore::resetHandle(const VkDevice handle)
  {
    if (handle_) {
      resetDepthImage(nullptr);

      // Уничтожаем логическое устройство Vulkan.
      vkDestroyDevice(handle_, nullptr);
      handle_ = VK_NULL_HANDLE;
    }
    handle_ = handle;
  }

  ImageStore *DeviceStore::depthImage() const
  {
    if (!depthImage_) {
      throw std::runtime_error("Rm3yXzYf2Q :: Depth image not initialized");
    }
    return depthImage_.get();
  }

} // namespace vul
