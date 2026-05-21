/**
 *
 *  @file version.hpp
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
#ifndef VIX_GAME_VERSION_HPP
#define VIX_GAME_VERSION_HPP

namespace vix::game
{
  /**
   * @brief Major version number.
   */
  inline constexpr int version_major = 0;

  /**
   * @brief Minor version number.
   */
  inline constexpr int version_minor = 1;

  /**
   * @brief Patch version number.
   */
  inline constexpr int version_patch = 0;

  /**
   * @brief Optional pre-release identifier.
   *
   * Empty string means stable.
   */
  inline constexpr const char *version_prerelease = "";

  /**
   * @brief Optional build metadata.
   *
   * Empty string means no additional metadata.
   */
  inline constexpr const char *version_metadata = "";

  /**
   * @brief Human-readable semantic version string.
   */
  inline constexpr const char *version_string = "0.1.0";

  /**
   * @brief ABI version number.
   *
   * Increment this when binary-incompatible changes are introduced.
   */
  inline constexpr int abi_version = 0;

} // namespace vix::game

#endif // VIX_GAME_VERSION_HPP
