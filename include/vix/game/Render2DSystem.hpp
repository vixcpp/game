/**
 *
 *  @file Render2DSystem.hpp
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
#ifndef VIX_GAME_RENDER_2D_SYSTEM_HPP
#define VIX_GAME_RENDER_2D_SYSTEM_HPP

#include <vix/game/Frame.hpp>
#include <vix/game/Renderer2D.hpp>
#include <vix/game/Sprite.hpp>
#include <vix/game/System.hpp>
#include <vix/game/Transform2D.hpp>

namespace vix::game
{
  /**
   * @brief ECS system that submits visible sprites to Renderer2D.
   *
   * Render2DSystem iterates over entities that have:
   * - Transform2D
   * - Sprite
   *
   * It does not own the renderer. It only references Renderer2D and submits
   * draw commands during on_update().
   */
  class Render2DSystem final : public System
  {
  public:
    /**
     * @brief Construct an empty render system.
     */
    Render2DSystem() = default;

    /**
     * @brief Construct a render system with a renderer.
     *
     * @param renderer 2D renderer used to submit draw commands.
     */
    explicit Render2DSystem(Renderer2D &renderer) noexcept;

    Render2DSystem(const Render2DSystem &) = delete;
    Render2DSystem &operator=(const Render2DSystem &) = delete;

    /**
     * @brief Destroy the render system.
     */
    ~Render2DSystem() override = default;

    /**
     * @brief Attach a renderer.
     *
     * @param renderer 2D renderer used to submit draw commands.
     * @return Reference to this system.
     */
    Render2DSystem &set_renderer(Renderer2D &renderer) noexcept;

    /**
     * @brief Detach the current renderer.
     */
    void clear_renderer() noexcept;

    /**
     * @brief Return true if a renderer is attached.
     */
    [[nodiscard]] bool has_renderer() const noexcept;

    /**
     * @brief Submit renderable sprites to Renderer2D.
     *
     * @param frame Current frame.
     */
    void on_update(const Frame &frame) override;

  private:
    /**
     * @brief 2D renderer used by this system.
     */
    Renderer2D *renderer_{nullptr};
  };

} // namespace vix::game

#endif // VIX_GAME_RENDER_2D_SYSTEM_HPP
