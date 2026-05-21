/**
 *
 *  @file NullWindow.cpp
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

#include <vix/game/NullWindow.hpp>
#include <utility>
#include <vix/game/GameError.hpp>

namespace vix::game
{
  GameBoolResult NullWindow::open(
      const WindowConfig &config)
  {
    auto valid = config.validate();
    if (!valid)
    {
      return valid.error();
    }

    title_ = config.title;
    width_ = config.width;
    height_ = config.height;
    open_ = true;
    close_requested_ = false;
    events_.clear();

    return true;
  }

  void NullWindow::close() noexcept
  {
    open_ = false;
    close_requested_ = true;
  }

  std::vector<WindowEvent> NullWindow::poll_events()
  {
    std::vector<WindowEvent> out;
    out.swap(events_);
    return out;
  }

  void NullWindow::present()
  {
  }

  bool NullWindow::open() const noexcept
  {
    return open_;
  }

  bool NullWindow::close_requested() const noexcept
  {
    return close_requested_;
  }

  const std::string &NullWindow::title() const noexcept
  {
    return title_;
  }

  void NullWindow::set_title(std::string value)
  {
    title_ = std::move(value);
  }

  std::uint32_t NullWindow::width() const noexcept
  {
    return width_;
  }

  std::uint32_t NullWindow::height() const noexcept
  {
    return height_;
  }

  void NullWindow::resize(
      std::uint32_t width,
      std::uint32_t height)
  {
    if (width == 0 || height == 0)
    {
      return;
    }

    width_ = width;
    height_ = height;

    events_.push_back(WindowEvent::resized(width_, height_));
  }

  const char *NullWindow::backend_name() const noexcept
  {
    return "null";
  }

  void *NullWindow::native_handle() noexcept
  {
    return nullptr;
  }

  const void *NullWindow::native_handle() const noexcept
  {
    return nullptr;
  }

  void NullWindow::push_event(WindowEvent event)
  {
    events_.push_back(event);

    if (event.is_close_request())
    {
      close_requested_ = true;
    }

    if (event.type == WindowEventType::Resized)
    {
      if (event.width > 0 && event.height > 0)
      {
        width_ = event.width;
        height_ = event.height;
      }
    }
  }

  void NullWindow::request_close()
  {
    close_requested_ = true;
    events_.push_back(WindowEvent::close_requested());
  }

} // namespace vix::game
