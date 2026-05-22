/**
 *
 *  @file SDLOpenGLRenderer.hpp
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
#ifndef VIX_GAME_BACKENDS_SDL_SDL_OPENGL_RENDERER_HPP
#define VIX_GAME_BACKENDS_SDL_SDL_OPENGL_RENDERER_HPP

#include <cstdint>

#include <SDL2/SDL.h>

#include <vix/game/Asset.hpp>
#include <vix/game/Color.hpp>
#include <vix/game/DrawSpriteCommand.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/RendererBackend.hpp>
#include <vix/game/Window.hpp>

namespace vix::game::sdl
{
  /**
   * @brief OpenGL renderer backend using an SDL window and SDL OpenGL context.
   *
   * This backend is intentionally named SDLOpenGLRenderer because the OpenGL
   * context is created through SDL. It is not the future generic OpenGL backend.
   */
  class SDLOpenGLRenderer final : public RendererBackend
  {
  public:
    SDLOpenGLRenderer() = default;

    SDLOpenGLRenderer(const SDLOpenGLRenderer &) = delete;
    SDLOpenGLRenderer &operator=(const SDLOpenGLRenderer &) = delete;

    ~SDLOpenGLRenderer() override;

    [[nodiscard]] GameBoolResult init(Window &window) override;

    [[nodiscard]] GameBoolResult upload_texture(const Asset &asset) override;

    void shutdown() noexcept override;

    void begin_frame() override;

    void end_frame() override;

    void clear(Color color) override;

    void draw_sprite(const DrawSpriteCommand &command) override;

    void resize(
        std::uint32_t width,
        std::uint32_t height) override;

    [[nodiscard]] bool initialized() const noexcept override;

    [[nodiscard]] std::uint32_t width() const noexcept override;

    [[nodiscard]] std::uint32_t height() const noexcept override;

    [[nodiscard]] const char *backend_name() const noexcept override;

    [[nodiscard]] void *native_handle() noexcept override;

    [[nodiscard]] const void *native_handle() const noexcept override;

    [[nodiscard]] bool frame_active() const noexcept;

  private:
    SDL_Window *window_{nullptr};
    SDL_GLContext context_{nullptr};

    bool initialized_{false};
    bool frame_active_{false};

    std::uint32_t width_{0};
    std::uint32_t height_{0};
  };

} // namespace vix::game::sdl

#endif // VIX_GAME_BACKENDS_SDL_SDL_OPENGL_RENDERER_HPP
