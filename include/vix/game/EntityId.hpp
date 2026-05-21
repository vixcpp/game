/**
 *
 *  @file EntityId.hpp
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
#ifndef VIX_GAME_ENTITY_ID_HPP
#define VIX_GAME_ENTITY_ID_HPP

#include <cstdint>

namespace vix::game
{
  /**
   * @brief Unique identifier type for entities.
   *
   * EntityId identifies an entity inside a Registry.
   * A value of 0 is reserved for an invalid or unassigned entity.
   */
  using EntityId = std::uint64_t;

  /**
   * @brief Invalid entity identifier.
   */
  inline constexpr EntityId invalid_entity_id = 0;

  /**
   * @brief Check whether an entity id is valid.
   *
   * @param id Entity identifier.
   * @return true if the id is valid.
   */
  [[nodiscard]] inline constexpr bool is_valid_entity_id(EntityId id) noexcept
  {
    return id != invalid_entity_id;
  }

} // namespace vix::game

#endif // VIX_GAME_ENTITY_ID_HPP
