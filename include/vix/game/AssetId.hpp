/**
 *
 *  @file AssetId.hpp
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
#ifndef VIX_GAME_ASSET_ID_HPP
#define VIX_GAME_ASSET_ID_HPP

#include <cstdint>

namespace vix::game
{
  /**
   * @brief Unique identifier type for assets.
   *
   * AssetId identifies an asset managed by AssetManager.
   * A value of 0 is reserved for an invalid or unassigned asset.
   */
  using AssetId = std::uint64_t;

  /**
   * @brief Invalid asset identifier.
   */
  inline constexpr AssetId invalid_asset_id = 0;

  /**
   * @brief Check whether an asset id is valid.
   *
   * @param id Asset identifier.
   * @return true if the id is valid.
   */
  [[nodiscard]] inline constexpr bool is_valid_asset_id(AssetId id) noexcept
  {
    return id != invalid_asset_id;
  }

} // namespace vix::game

#endif // VIX_GAME_ASSET_ID_HPP
