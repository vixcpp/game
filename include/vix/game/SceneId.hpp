/**
 *
 *  @file SceneId.hpp
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
#ifndef VIX_GAME_SCENE_ID_HPP
#define VIX_GAME_SCENE_ID_HPP

#include <cstdint>

namespace vix::game
{
  /**
   * @brief Unique identifier type for scenes.
   *
   * SceneId identifies a scene registered inside SceneManager.
   * A value of 0 is reserved for an invalid or unassigned scene.
   */
  using SceneId = std::uint64_t;

  /**
   * @brief Invalid scene identifier.
   */
  inline constexpr SceneId invalid_scene_id = 0;

  /**
   * @brief Check whether a scene id is valid.
   *
   * @param id Scene identifier.
   * @return true if the id is valid.
   */
  [[nodiscard]] inline constexpr bool is_valid_scene_id(SceneId id) noexcept
  {
    return id != invalid_scene_id;
  }

} // namespace vix::game

#endif // VIX_GAME_SCENE_ID_HPP
