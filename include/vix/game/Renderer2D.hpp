/**
 *
 *  @file Renderer2D.hpp
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
#ifndef VIX_GAME_RENDERER_2D_HPP
#define VIX_GAME_RENDERER_2D_HPP

#include <cstddef>
#include <vector>

#include <vix/game/Camera2D.hpp>
#include <vix/game/Color.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/Renderer.hpp>
#include <vix/game/Sprite.hpp>
#include <vix/game/Transform2D.hpp>
#include <vix/game/DrawSpriteCommand.hpp>

namespace vix::game
{
  /**
   * @brief Simple backend-independent 2D renderer facade.
   *
   * Renderer2D is a high-level layer built on top of Renderer.
   *
   * V2 does not implement real GPU sprite rendering yet. It provides:
   * - frame lifecycle forwarding
   * - clear color support
   * - active camera
   * - sprite draw command collection
   *
   * This gives Render2DSystem and future backends a stable API without forcing
   * OpenGL, Vulkan, SDL, or another graphics dependency into vix/game.
   */
  class Renderer2D
  {
  public:
    /**
     * @brief Construct an empty Renderer2D.
     */
    Renderer2D() = default;

    /**
     * @brief Construct Renderer2D from a renderer.
     *
     * @param renderer Backend-independent renderer facade.
     */
    explicit Renderer2D(Renderer &renderer) noexcept;

    Renderer2D(const Renderer2D &) = delete;
    Renderer2D &operator=(const Renderer2D &) = delete;

    /**
     * @brief Destroy the 2D renderer.
     */
    ~Renderer2D() = default;

    /**
     * @brief Attach a renderer.
     *
     * @param renderer Backend-independent renderer facade.
     * @return Reference to this renderer.
     */
    Renderer2D &attach(Renderer &renderer) noexcept;

    /**
     * @brief Detach the renderer.
     */
    void detach() noexcept;

    /**
     * @brief Return true if a renderer is attached.
     */
    [[nodiscard]] bool attached() const noexcept;

    /**
     * @brief Begin a 2D frame.
     */
    void begin_frame();

    /**
     * @brief End a 2D frame.
     */
    void end_frame();

    /**
     * @brief Clear the render target.
     *
     * @param color Clear color.
     */
    void clear(Color color = Color::black());

    /**
     * @brief Submit a sprite draw command.
     *
     * @param transform Sprite transform.
     * @param sprite Sprite data.
     */
    void draw_sprite(
        const Transform2D &transform,
        const Sprite &sprite);

    /**
     * @brief Submit a sprite draw command with explicit camera.
     *
     * @param camera Camera used for this draw.
     * @param transform Sprite transform.
     * @param sprite Sprite data.
     */
    void draw_sprite(
        const Camera2D &camera,
        const Transform2D &transform,
        const Sprite &sprite);

    /**
     * @brief Set the active camera.
     *
     * @param camera New active camera.
     * @return Reference to this renderer.
     */
    Renderer2D &set_camera(Camera2D camera) noexcept;

    /**
     * @brief Return the active camera.
     */
    [[nodiscard]] const Camera2D &camera() const noexcept;

    /**
     * @brief Return true if a 2D frame is active.
     */
    [[nodiscard]] bool frame_active() const noexcept;

    /**
     * @brief Return the current draw commands.
     */
    [[nodiscard]] const std::vector<DrawSpriteCommand> &commands() const noexcept;

    /**
     * @brief Return the number of submitted draw commands.
     */
    [[nodiscard]] std::size_t command_count() const noexcept;

    /**
     * @brief Return true if no draw command was submitted.
     */
    [[nodiscard]] bool empty() const noexcept;

    /**
     * @brief Clear submitted draw commands.
     */
    void clear_commands();

    /**
     * @brief Return the attached renderer.
     */
    [[nodiscard]] Renderer *renderer() noexcept;

    /**
     * @brief Return the attached renderer.
     */
    [[nodiscard]] const Renderer *renderer() const noexcept;

  private:
    /**
     * @brief Attached renderer.
     */
    Renderer *renderer_{nullptr};

    /**
     * @brief Active camera.
     */
    Camera2D camera_{};

    /**
     * @brief Submitted sprite commands.
     */
    std::vector<DrawSpriteCommand> commands_{};

    /**
     * @brief Whether begin_frame() was called without a matching end_frame().
     */
    bool frame_active_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_RENDERER_2D_HPP
