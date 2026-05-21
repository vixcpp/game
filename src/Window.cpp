/**
 *
 *  @file Window.cpp
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

#include <vix/game/Window.hpp>

#include <utility>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  Window::Window(std::unique_ptr<WindowBackend> backend) noexcept
      : backend_(std::move(backend))
  {
  }

  Window::Window(
      std::unique_ptr<WindowBackend> backend,
      WindowConfig config)
      : backend_(std::move(backend))
  {
    (void)open(config);
  }

  Window::~Window()
  {
    close();
  }

  Window &Window::set_backend(
      std::unique_ptr<WindowBackend> backend) noexcept
  {
    close();
    backend_ = std::move(backend);
    return *this;
  }

  bool Window::has_backend() const noexcept
  {
    return backend_ != nullptr;
  }

  GameBoolResult Window::open(const WindowConfig &config)
  {
    if (!backend_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "window backend is not set");
    }

    auto valid = config.validate();
    if (!valid)
    {
      return valid.error();
    }

    return backend_->open(config);
  }

  void Window::close() noexcept
  {
    if (backend_)
    {
      backend_->close();
    }
  }

  std::vector<WindowEvent> Window::poll_events()
  {
    if (!backend_)
    {
      return {};
    }

    return backend_->poll_events();
  }

  void Window::present()
  {
    if (backend_)
    {
      backend_->present();
    }
  }

  bool Window::open() const noexcept
  {
    return backend_ != nullptr && backend_->open();
  }

  bool Window::close_requested() const noexcept
  {
    return backend_ != nullptr && backend_->close_requested();
  }

  const std::string &Window::title() const noexcept
  {
    if (!backend_)
    {
      return empty_title();
    }

    return backend_->title();
  }

  void Window::set_title(std::string value)
  {
    if (backend_)
    {
      backend_->set_title(std::move(value));
    }
  }

  std::uint32_t Window::width() const noexcept
  {
    if (!backend_)
    {
      return 0;
    }

    return backend_->width();
  }

  std::uint32_t Window::height() const noexcept
  {
    if (!backend_)
    {
      return 0;
    }

    return backend_->height();
  }

  void Window::resize(
      std::uint32_t width,
      std::uint32_t height)
  {
    if (backend_)
    {
      backend_->resize(width, height);
    }
  }

  const char *Window::backend_name() const noexcept
  {
    if (!backend_)
    {
      return "none";
    }

    return backend_->backend_name();
  }

  void *Window::native_handle() noexcept
  {
    if (!backend_)
    {
      return nullptr;
    }

    return backend_->native_handle();
  }

  const void *Window::native_handle() const noexcept
  {
    if (!backend_)
    {
      return nullptr;
    }

    return backend_->native_handle();
  }

  WindowBackend *Window::backend() noexcept
  {
    return backend_.get();
  }

  const WindowBackend *Window::backend() const noexcept
  {
    return backend_.get();
  }

  const std::string &Window::empty_title() noexcept
  {
    static const std::string title;
    return title;
  }

} // namespace vix::game
