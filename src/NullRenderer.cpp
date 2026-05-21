/**
 *
 *  @file NullRenderer.cpp
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

#include <vix/game/NullRenderer.hpp>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  GameBoolResult NullRenderer::init(Window &window)
  {
    if (!window.open())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "cannot initialize renderer with a closed window");
    }

    width_ = window.width();
    height_ = window.height();

    if (width_ == 0 || height_ == 0)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "renderer size must be greater than zero");
    }

    initialized_ = true;
    frame_active_ = false;
    clear_color_ = Color::black();

    return true;
  }

  void NullRenderer::shutdown() noexcept
  {
    initialized_ = false;
    frame_active_ = false;
    width_ = 0;
    height_ = 0;
    clear_color_ = Color::black();
  }

  void NullRenderer::begin_frame()
  {
    if (!initialized_)
    {
      return;
    }

    frame_active_ = true;
  }

  void NullRenderer::end_frame()
  {
    if (!initialized_)
    {
      return;
    }

    frame_active_ = false;
  }

  void NullRenderer::clear(Color color)
  {
    if (!initialized_)
    {
      return;
    }

    clear_color_ = color.clamped();
  }

  void NullRenderer::resize(
      std::uint32_t width,
      std::uint32_t height)
  {
    if (width == 0 || height == 0)
    {
      return;
    }

    width_ = width;
    height_ = height;
  }

  bool NullRenderer::initialized() const noexcept
  {
    return initialized_;
  }

  std::uint32_t NullRenderer::width() const noexcept
  {
    return width_;
  }

  std::uint32_t NullRenderer::height() const noexcept
  {
    return height_;
  }

  const char *NullRenderer::backend_name() const noexcept
  {
    return "null";
  }

  void *NullRenderer::native_handle() noexcept
  {
    return nullptr;
  }

  const void *NullRenderer::native_handle() const noexcept
  {
    return nullptr;
  }

  bool NullRenderer::frame_active() const noexcept
  {
    return frame_active_;
  }

  Color NullRenderer::clear_color() const noexcept
  {
    return clear_color_;
  }

} // namespace vix::game
