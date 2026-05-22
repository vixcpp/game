/**
 *
 *  @file SDLOpenGLRenderer.cpp
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

#include <vix/game/backends/sdl/SDLOpenGLRenderer.hpp>

#include <algorithm>

#include <GL/gl.h>

#include <vix/game/GameError.hpp>

namespace vix::game::sdl
{
  SDLOpenGLRenderer::~SDLOpenGLRenderer()
  {
    shutdown();
  }

  GameBoolResult SDLOpenGLRenderer::init(Window &window)
  {
    if (!window.open())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "cannot initialize SDL OpenGL renderer with a closed window");
    }

    auto *native_window = static_cast<SDL_Window *>(window.native_handle());
    if (native_window == nullptr)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "SDL OpenGL renderer requires an SDL window native handle");
    }

    if (context_ != nullptr)
    {
      shutdown();
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    context_ = SDL_GL_CreateContext(native_window);
    if (context_ == nullptr)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          SDL_GetError());
    }

    if (SDL_GL_MakeCurrent(native_window, context_) != 0)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          SDL_GetError());
    }

    SDL_GL_SetSwapInterval(1);

    window_ = native_window;
    width_ = window.width();
    height_ = window.height();

    if (width_ == 0 || height_ == 0)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "SDL OpenGL renderer size must be greater than zero");
    }

    glViewport(
        0,
        0,
        static_cast<GLsizei>(width_),
        static_cast<GLsizei>(height_));

    initialized_ = true;
    frame_active_ = false;

    return true;
  }

  GameBoolResult SDLOpenGLRenderer::upload_texture(const Asset &asset)
  {
    if (!initialized_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "SDL OpenGL renderer is not initialized");
    }

    if (!asset.valid())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "texture asset id is invalid");
    }

    return make_game_error(
        GameErrorCode::InvalidState,
        "SDL OpenGL texture upload is not implemented yet");
  }

  void SDLOpenGLRenderer::shutdown() noexcept
  {
    if (context_ != nullptr)
    {
      SDL_GL_DeleteContext(context_);
      context_ = nullptr;
    }

    window_ = nullptr;
    initialized_ = false;
    frame_active_ = false;
    width_ = 0;
    height_ = 0;
  }

  void SDLOpenGLRenderer::begin_frame()
  {
    if (!initialized_)
    {
      return;
    }

    frame_active_ = true;
  }

  void SDLOpenGLRenderer::end_frame()
  {
    if (!initialized_ || window_ == nullptr)
    {
      return;
    }

    SDL_GL_SwapWindow(window_);
    frame_active_ = false;
  }

  void SDLOpenGLRenderer::clear(Color color)
  {
    if (!initialized_)
    {
      return;
    }

    const auto clamped = color.clamped();

    glClearColor(
        std::clamp(clamped.r, 0.0F, 1.0F),
        std::clamp(clamped.g, 0.0F, 1.0F),
        std::clamp(clamped.b, 0.0F, 1.0F),
        std::clamp(clamped.a, 0.0F, 1.0F));

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  }

  void SDLOpenGLRenderer::draw_sprite(const DrawSpriteCommand &command)
  {
    (void)command;
  }

  void SDLOpenGLRenderer::resize(
      std::uint32_t width,
      std::uint32_t height)
  {
    if (width == 0 || height == 0)
    {
      return;
    }

    width_ = width;
    height_ = height;

    if (initialized_)
    {
      glViewport(
          0,
          0,
          static_cast<GLsizei>(width_),
          static_cast<GLsizei>(height_));
    }
  }

  bool SDLOpenGLRenderer::initialized() const noexcept
  {
    return initialized_;
  }

  std::uint32_t SDLOpenGLRenderer::width() const noexcept
  {
    return width_;
  }

  std::uint32_t SDLOpenGLRenderer::height() const noexcept
  {
    return height_;
  }

  const char *SDLOpenGLRenderer::backend_name() const noexcept
  {
    return "sdl-opengl";
  }

  void *SDLOpenGLRenderer::native_handle() noexcept
  {
    return context_;
  }

  const void *SDLOpenGLRenderer::native_handle() const noexcept
  {
    return context_;
  }

  bool SDLOpenGLRenderer::frame_active() const noexcept
  {
    return frame_active_;
  }

} // namespace vix::game::sdl
