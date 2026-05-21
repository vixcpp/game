/**
 *
 *  @file WindowConfig.cpp
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

#include <vix/game/WindowConfig.hpp>

#include <utility>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  WindowConfig WindowConfig::defaults() noexcept
  {
    return WindowConfig{};
  }

  WindowConfig WindowConfig::headless_config() noexcept
  {
    WindowConfig config;
    config.title = "Vix Game Headless";
    config.width = 1;
    config.height = 1;
    config.fullscreen = false;
    config.resizable = false;
    config.visible = false;
    config.vsync = false;
    config.headless = true;
    return config;
  }

  GameBoolResult WindowConfig::validate() const
  {
    if (title.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "window title cannot be empty");
    }

    if (!valid_size())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "window size must be greater than zero");
    }

    return true;
  }

  WindowConfig &WindowConfig::set_title(std::string value)
  {
    title = std::move(value);
    return *this;
  }

  WindowConfig &WindowConfig::set_size(
      std::uint32_t w,
      std::uint32_t h) noexcept
  {
    width = w;
    height = h;
    return *this;
  }

  bool WindowConfig::valid_size() const noexcept
  {
    return width > 0 && height > 0;
  }

} // namespace vix::game
