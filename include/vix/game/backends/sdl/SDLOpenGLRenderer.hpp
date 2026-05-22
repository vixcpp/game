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

#include <array>
#include <cstdint>
#include <unordered_map>

#include <SDL2/SDL.h>

#include <vix/game/Asset.hpp>
#include <vix/game/AssetId.hpp>
#include <vix/game/Color.hpp>
#include <vix/game/DrawSpriteCommand.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/RendererBackend.hpp>
#include <vix/game/Window.hpp>

namespace vix::game::sdl
{
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

    [[nodiscard]] std::uint64_t sprite_count() const noexcept;

    [[nodiscard]] std::uint64_t uploaded_texture_count() const noexcept;

  private:
    struct TextureRecord
    {
      std::uint32_t id{0};
      std::uint32_t width{0};
      std::uint32_t height{0};
    };

    struct Vertex
    {
      float x{0.0F};
      float y{0.0F};
      float u{0.0F};
      float v{0.0F};
      float r{1.0F};
      float g{1.0F};
      float b{1.0F};
      float a{1.0F};
    };

  private:
    [[nodiscard]] GameBoolResult create_pipeline();

    [[nodiscard]] GameBoolResult create_shader_program();

    [[nodiscard]] GameBoolResult create_buffers();

    [[nodiscard]] GameBoolResult load_gl_functions();

    [[nodiscard]] std::array<float, 16> projection_matrix() const noexcept;

    void destroy_pipeline() noexcept;

    void destroy_textures() noexcept;

    void use_pipeline();

  private:
    SDL_Window *window_{nullptr};
    SDL_GLContext context_{nullptr};

    bool initialized_{false};
    bool frame_active_{false};

    std::uint32_t width_{0};
    std::uint32_t height_{0};

    std::uint32_t shader_program_{0};
    std::uint32_t vao_{0};
    std::uint32_t vbo_{0};
    std::uint32_t ebo_{0};

    int projection_location_{-1};
    int texture_location_{-1};

    std::uint64_t sprite_count_{0};
    std::uint64_t uploaded_texture_count_{0};

    std::unordered_map<AssetId, TextureRecord> textures_{};
  };

} // namespace vix::game::sdl

#endif // VIX_GAME_BACKENDS_SDL_SDL_OPENGL_RENDERER_HPP
