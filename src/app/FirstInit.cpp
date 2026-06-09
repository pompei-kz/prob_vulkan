//
// Created by pompei on 2026-06-09.
//

#include "FirstInit.h"

#include <SDL3/SDL_init.h>
#include <stdexcept>

void app::FirstInit::init()
{
  if (initialized_) return;

  if (destroyed_) {
    throw std::runtime_error("sb5peMcn3I :: initialize of destroyed the first initiator");
  }

  if (!SDL_Init(SDL_INIT_VIDEO)) {
    throw std::runtime_error(std::string("NfTe15Se5d :: failed to init SDL: ") + SDL_GetError());
  }

  initialized_ = true;
}

void app::FirstInit::destroy()
{
  if (destroyed_) return;

  SDL_Quit();

  destroyed_ = true;
}
