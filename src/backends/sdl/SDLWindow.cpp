/**
 *
 *  @file SDLWindow.cpp
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

#include <vix/game/backends/sdl/SDLWindow.hpp>

#include <utility>

#include <vix/game/GameError.hpp>
#include <vix/game/backends/sdl/SDLInputMap.hpp>

namespace vix::game::sdl
{
  SDLWindow::~SDLWindow()
  {
    close();
  }

  GameBoolResult SDLWindow::open(
      const WindowConfig &config)
  {
    auto valid = config.validate();
    if (!valid)
    {
      return valid.error();
    }

    if (window_ != nullptr)
    {
      close();
    }

    if (SDL_WasInit(SDL_INIT_VIDEO) == 0)
    {
      if (SDL_InitSubSystem(SDL_INIT_VIDEO) != 0)
      {
        return make_game_error(
            GameErrorCode::InvalidState,
            SDL_GetError());
      }

      sdl_video_initialized_ = true;
    }

    title_ = config.title;
    width_ = config.width;
    height_ = config.height;
    close_requested_ = false;

    const auto flags = make_window_flags(config);

    window_ = SDL_CreateWindow(
        title_.c_str(),
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        static_cast<int>(width_),
        static_cast<int>(height_),
        flags);

    if (window_ == nullptr)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          SDL_GetError());
    }

    return true;
  }

  void SDLWindow::close() noexcept
  {
    if (window_ != nullptr)
    {
      SDL_DestroyWindow(window_);
      window_ = nullptr;
    }

    close_requested_ = true;

    if (sdl_video_initialized_)
    {
      SDL_QuitSubSystem(SDL_INIT_VIDEO);
      sdl_video_initialized_ = false;
    }
  }

  std::vector<WindowEvent> SDLWindow::poll_events()
  {
    std::vector<WindowEvent> events;

    SDL_Event event;
    while (SDL_PollEvent(&event) != 0)
    {
      switch (event.type)
      {
      case SDL_QUIT:
        close_requested_ = true;
        events.push_back(WindowEvent::close_requested());
        break;

      case SDL_WINDOWEVENT:
        if (event.window.event == SDL_WINDOWEVENT_CLOSE)
        {
          close_requested_ = true;
          events.push_back(WindowEvent::close_requested());
        }
        else if (event.window.event == SDL_WINDOWEVENT_SIZE_CHANGED ||
                 event.window.event == SDL_WINDOWEVENT_RESIZED)
        {
          const auto w = static_cast<std::uint32_t>(event.window.data1);
          const auto h = static_cast<std::uint32_t>(event.window.data2);

          if (w > 0 && h > 0)
          {
            width_ = w;
            height_ = h;
            events.push_back(WindowEvent::resized(width_, height_));
          }
        }
        else if (event.window.event == SDL_WINDOWEVENT_FOCUS_GAINED)
        {
          WindowEvent out;
          out.type = WindowEventType::FocusGained;
          events.push_back(out);
        }
        else if (event.window.event == SDL_WINDOWEVENT_FOCUS_LOST)
        {
          WindowEvent out;
          out.type = WindowEventType::FocusLost;
          events.push_back(out);
        }
        break;

      case SDL_KEYDOWN:
        if (event.key.repeat == 0)
        {
          events.push_back(
              WindowEvent::key_pressed(sdl_key_to_input_key(event.key.keysym.sym)));
        }
        break;

      case SDL_KEYUP:
        events.push_back(
            WindowEvent::key_released(sdl_key_to_input_key(event.key.keysym.sym)));
        break;

      case SDL_MOUSEBUTTONDOWN:
        events.push_back(
            WindowEvent::button_pressed(sdl_button_to_input_button(event.button.button)));
        break;

      case SDL_MOUSEBUTTONUP:
        events.push_back(
            WindowEvent::button_released(sdl_button_to_input_button(event.button.button)));
        break;

      case SDL_MOUSEMOTION:
        events.push_back(
            WindowEvent::pointer_moved(
                static_cast<double>(event.motion.x),
                static_cast<double>(event.motion.y),
                static_cast<double>(event.motion.xrel),
                static_cast<double>(event.motion.yrel)));
        break;

      case SDL_MOUSEWHEEL:
        events.push_back(
            WindowEvent::wheel_moved(
                static_cast<double>(event.wheel.x),
                static_cast<double>(event.wheel.y)));
        break;

      default:
        break;
      }
    }

    return events;
  }

  void SDLWindow::present()
  {
  }

  bool SDLWindow::open() const noexcept
  {
    return window_ != nullptr;
  }

  bool SDLWindow::close_requested() const noexcept
  {
    return close_requested_;
  }

  const std::string &SDLWindow::title() const noexcept
  {
    return title_;
  }

  void SDLWindow::set_title(std::string value)
  {
    title_ = std::move(value);

    if (window_ != nullptr)
    {
      SDL_SetWindowTitle(window_, title_.c_str());
    }
  }

  std::uint32_t SDLWindow::width() const noexcept
  {
    return width_;
  }

  std::uint32_t SDLWindow::height() const noexcept
  {
    return height_;
  }

  void SDLWindow::resize(
      std::uint32_t width,
      std::uint32_t height)
  {
    if (width == 0 || height == 0)
    {
      return;
    }

    width_ = width;
    height_ = height;

    if (window_ != nullptr)
    {
      SDL_SetWindowSize(
          window_,
          static_cast<int>(width_),
          static_cast<int>(height_));
    }
  }

  const char *SDLWindow::backend_name() const noexcept
  {
    return "sdl";
  }

  void *SDLWindow::native_handle() noexcept
  {
    return window_;
  }

  const void *SDLWindow::native_handle() const noexcept
  {
    return window_;
  }

  std::uint32_t SDLWindow::make_window_flags(
      const WindowConfig &config) noexcept
  {
    std::uint32_t flags = SDL_WINDOW_SHOWN;

    if (!config.visible)
    {
      flags = SDL_WINDOW_HIDDEN;
    }

    if (config.resizable)
    {
      flags |= SDL_WINDOW_RESIZABLE;
    }

    if (config.fullscreen)
    {
      flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
    }

    if (config.opengl)
    {
      flags |= SDL_WINDOW_OPENGL;
    }

    return flags;
  }
} // namespace vix::game::sdl
