//
// Created by pompei on 2026-06-11.
//

#pragma once
#include "TopStore.h"
#include "app/MainWindow.h"
#include "model/QueueFamilyIndices.h"
#include "model/SwapChainSupport.h"
#include "util/Log.h"
#include "util/util.h"

#include <SDL3/SDL_video.h>
#include <stdexcept>
#include <vector>
#include <vulkan/vulkan.h>

namespace vul {

  class SwapChainStore_Worker
  /// BEAN swapChainStore_Worker
  /// DEPS vul::TopStore app::MainWindow
  {
    di::Getter<TopStore>        &topStore_;
    di::Getter<app::MainWindow> &mainWindow_;

  public:
    SwapChainStore_Worker(di::Getter<TopStore> &topStore, di::Getter<app::MainWindow> &mainWindow)
        : topStore_(topStore)
        , mainWindow_(mainWindow)
    {}

    void create() const;
    void createImageViews() const;

  private:
    static VkSurfaceFormatKHR       chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR> &formats);
    static VkPresentModeKHR         chooseSwapPresentMode(const std::vector<VkPresentModeKHR> &presentModes);
    [[nodiscard]] static VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR &capabilities, SDL_Window *window);
  };

} // namespace vul
