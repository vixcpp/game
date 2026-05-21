/**
 *
 *  @file GameResult.hpp
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
#ifndef VIX_GAME_GAME_RESULT_HPP
#define VIX_GAME_GAME_RESULT_HPP

#include <vix/error/Result.hpp>
#include <vix/game/GameError.hpp>

namespace vix::game
{
  /**
   * @brief Standard result type used by vix/game APIs.
   *
   * GameResult<T> is an alias over the shared Vix Result<T> type.
   * It allows game APIs to return either:
   * - a success value of type T
   * - a structured vix::error::Error
   *
   * @tparam T Success value type.
   */
  template <typename T>
  using GameResult = vix::error::Result<T>;

  /**
   * @brief Result type for game operations returning only success/failure.
   */
  using GameBoolResult = GameResult<bool>;

  /**
   * @brief Result type for game operations returning a string.
   */
  using GameStringResult = GameResult<std::string>;

} // namespace vix::game

#endif // VIX_GAME_GAME_RESULT_HPP
