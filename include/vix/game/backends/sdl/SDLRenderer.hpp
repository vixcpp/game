/**
 *
 *  @file SDLRenderer.hpp
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
#ifndef VIX_GAME_BACKENDS_SDL_SDL_RENDERER_HPP
#define VIX_GAME_BACKENDS_SDL_SDL_RENDERER_HPP

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
#include <vix/game/TextureHandle.hpp>

namespace vix::game::sdl
{
  /**
   * @brief SDL2 renderer backend for vix/game.
   *
   * SDLRenderer implements RendererBackend using SDL_Renderer.
   *
   * V4 starts with:
   * - renderer initialization from SDLWindow
   * - frame begin/end
   * - clear color
   * - resize tracking
   * - texture upload API placeholder
   * - sprite draw API placeholder
   */
  class SDLRenderer final : public RendererBackend
  {
  public:
    /**
     * @brief Construct an empty SDL renderer.
     */
    SDLRenderer() = default;

    SDLRenderer(const SDLRenderer &) = delete;
    SDLRenderer &operator=(const SDLRenderer &) = delete;

    /**
     * @brief Destroy the SDL renderer.
     */
    ~SDLRenderer() override;

    /**
     * @brief Initialize the SDL renderer for a window.
     *
     * @param window Target window.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult init(Window &window) override;

    /**
     * @brief Upload an image asset as a renderer texture.
     *
     * V4 initial version validates the asset. Real decoding is added later.
     *
     * @param asset Loaded image asset.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult upload_texture(const Asset &asset) override;

    /**
     * @brief Shut down the SDL renderer.
     */
    void shutdown() noexcept override;

    /**
     * @brief Begin rendering a frame.
     */
    void begin_frame() override;

    /**
     * @brief End rendering a frame and present.
     */
    void end_frame() override;

    /**
     * @brief Clear the SDL render target.
     *
     * @param color Clear color.
     */
    void clear(Color color) override;

    /**
     * @brief Draw one 2D sprite command.
     *
     * V4 initial version accepts the command but does not draw textures yet.
     *
     * @param command Sprite draw command.
     */
    void draw_sprite(const DrawSpriteCommand &command) override;

    /**
     * @brief Resize the virtual render target.
     *
     * @param width New width.
     * @param height New height.
     */
    void resize(
        std::uint32_t width,
        std::uint32_t height) override;

    /**
     * @brief Return true if initialized.
     */
    [[nodiscard]] bool initialized() const noexcept override;

    /**
     * @brief Return render width.
     */
    [[nodiscard]] std::uint32_t width() const noexcept override;

    /**
     * @brief Return render height.
     */
    [[nodiscard]] std::uint32_t height() const noexcept override;

    /**
     * @brief Return backend name.
     */
    [[nodiscard]] const char *backend_name() const noexcept override;

    /**
     * @brief Return native SDL_Renderer handle.
     */
    [[nodiscard]] void *native_handle() noexcept override;

    /**
     * @brief Return native SDL_Renderer handle.
     */
    [[nodiscard]] const void *native_handle() const noexcept override;

    /**
     * @brief Return true if a frame is currently active.
     */
    [[nodiscard]] bool frame_active() const noexcept;

    /**
     * @brief Return accepted sprite draw command count for current frame.
     */
    [[nodiscard]] std::uint64_t sprite_count() const noexcept;

    /**
     * @brief Return uploaded texture count.
     */
    [[nodiscard]] std::uint64_t uploaded_texture_count() const noexcept;

  private:
    /**
     * @brief Convert normalized color channel to SDL uint8 channel.
     */
    [[nodiscard]] static std::uint8_t to_u8(float value) noexcept;

  private:
    /**
     * @brief Native SDL renderer.
     */
    SDL_Renderer *renderer_{nullptr};

    /**
     * @brief Whether the renderer is initialized.
     */
    bool initialized_{false};

    /**
     * @brief Whether begin_frame() was called without end_frame().
     */
    bool frame_active_{false};

    /**
     * @brief Render width.
     */
    std::uint32_t width_{0};

    /**
     * @brief Render height.
     */
    std::uint32_t height_{0};

    /**
     * @brief Accepted sprite draw command count for current frame.
     */
    std::uint64_t sprite_count_{0};

    /**
     * @brief Uploaded texture count.
     */
    std::uint64_t uploaded_texture_count_{0};

    /**
     * @brief SDL textures by asset id.
     */
    std::unordered_map<AssetId, SDL_Texture *> textures_{};
  };

} // namespace vix::game::sdl

#endif // VIX_GAME_BACKENDS_SDL_SDL_RENDERER_HPP
