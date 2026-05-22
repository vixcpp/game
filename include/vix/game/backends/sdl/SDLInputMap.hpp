/**
 *
 *  @file SDLInputMap.hpp
 *  @author Gaspard Kirira
 *
 *  Copyright 2026, Gaspard Kirira.
 *  All rights reserved.
 *  https://github.com/vixcpp/game
 *
 *  Use of this source code is governed by a MIT license
 *  that can be found in the LICENSE file.
 *
 *  Vix.cpp
 *
 */
#ifndef VIX_GAME_BACKENDS_SDL_SDL_INPUT_MAP_HPP
#define VIX_GAME_BACKENDS_SDL_SDL_INPUT_MAP_HPP

#include <cstdint>

#include <SDL2/SDL.h>

#include <vix/game/InputButton.hpp>
#include <vix/game/InputKey.hpp>

namespace vix::game::sdl
{
  /**
   * @brief Convert an SDL keycode to a backend-independent input key.
   */
  [[nodiscard]] InputKey sdl_key_to_input_key(SDL_Keycode key) noexcept;

  /**
   * @brief Convert an SDL mouse button to a backend-independent input button.
   */
  [[nodiscard]] InputButton sdl_button_to_input_button(std::uint8_t button) noexcept;

} // namespace vix::game::sdl

#endif // VIX_GAME_BACKENDS_SDL_SDL_INPUT_MAP_HPP
