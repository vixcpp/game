/**
 *
 *  @file DrawSpriteCommand.hpp
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
#ifndef VIX_GAME_DRAW_SPRITE_COMMAND_HPP
#define VIX_GAME_DRAW_SPRITE_COMMAND_HPP

#include <vix/game/Camera2D.hpp>
#include <vix/game/Sprite.hpp>
#include <vix/game/Transform2D.hpp>

namespace vix::game
{
  /**
   * @brief One backend-independent 2D sprite draw command.
   *
   * DrawSpriteCommand stores the data needed by a renderer backend to draw one
   * sprite.
   */
  struct DrawSpriteCommand
  {
    /**
     * @brief Sprite transform.
     */
    Transform2D transform{};

    /**
     * @brief Sprite data.
     */
    Sprite sprite{};

    /**
     * @brief Camera used when the command was submitted.
     */
    Camera2D camera{};

    /**
     * @brief Return true if this command can be rendered.
     */
    [[nodiscard]] constexpr bool valid() const noexcept
    {
      return sprite.renderable() && camera.valid();
    }
  };

} // namespace vix::game

#endif // VIX_GAME_DRAW_SPRITE_COMMAND_HPP
