/**
 *
 *  @file GameError.hpp
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
#ifndef VIX_GAME_GAME_ERROR_HPP
#define VIX_GAME_GAME_ERROR_HPP

#include <string>

#include <vix/error/Error.hpp>
#include <vix/error/ErrorCategory.hpp>
#include <vix/error/ErrorCode.hpp>

namespace vix::game
{
  /**
   * @brief Game module specific error codes.
   *
   * These errors describe failures that can happen inside the game foundation:
   * app lifecycle, game loop, scenes, entities, assets, events, and jobs.
   */
  enum class GameErrorCode
  {
    None = 0,

    InvalidArgument,
    InvalidState,

    AppAlreadyRunning,
    AppNotRunning,

    GameLoopAlreadyRunning,
    GameLoopNotRunning,

    SceneNotFound,
    SceneAlreadyExists,
    SceneLoadFailed,
    SceneUnloadFailed,

    EntityNotFound,
    ComponentNotFound,
    SystemNotFound,

    AssetNotFound,
    AssetAlreadyLoaded,
    AssetLoadFailed,
    AssetInvalidPath,
    AssetUnsupportedType,

    EventDispatchFailed,

    JobRejected,
    JobCancelled,
    JobFailed,

    ConfigNotFound,
    ConfigInvalid,
    ConfigLoadFailed,

    Unknown
  };

  /**
   * @brief Return the default game error category.
   */
  [[nodiscard]] inline constexpr vix::error::ErrorCategory game_error_category() noexcept
  {
    return vix::error::ErrorCategory("game");
  }

  /**
   * @brief Convert a GameErrorCode to a generic Vix ErrorCode.
   *
   * This keeps vix/game aligned with the shared Vix error model.
   */
  [[nodiscard]] inline constexpr vix::error::ErrorCode to_error_code(GameErrorCode code) noexcept
  {
    using vix::error::ErrorCode;

    switch (code)
    {
    case GameErrorCode::None:
      return ErrorCode::Ok;

    case GameErrorCode::InvalidArgument:
    case GameErrorCode::AssetInvalidPath:
    case GameErrorCode::ConfigInvalid:
      return ErrorCode::InvalidArgument;

    case GameErrorCode::AssetUnsupportedType:
      return ErrorCode::NotSupported;

    case GameErrorCode::InvalidState:
    case GameErrorCode::AppAlreadyRunning:
    case GameErrorCode::AppNotRunning:
    case GameErrorCode::GameLoopAlreadyRunning:
    case GameErrorCode::GameLoopNotRunning:
      return ErrorCode::InvalidState;

    case GameErrorCode::SceneNotFound:
    case GameErrorCode::EntityNotFound:
    case GameErrorCode::ComponentNotFound:
    case GameErrorCode::SystemNotFound:
    case GameErrorCode::AssetNotFound:
    case GameErrorCode::ConfigNotFound:
      return ErrorCode::NotFound;

    case GameErrorCode::SceneAlreadyExists:
    case GameErrorCode::AssetAlreadyLoaded:
      return ErrorCode::AlreadyExists;

    case GameErrorCode::SceneLoadFailed:
    case GameErrorCode::SceneUnloadFailed:
    case GameErrorCode::AssetLoadFailed:
    case GameErrorCode::EventDispatchFailed:
    case GameErrorCode::JobFailed:
    case GameErrorCode::ConfigLoadFailed:
      return ErrorCode::InternalError;

    case GameErrorCode::JobRejected:
      return ErrorCode::ResourceExhausted;

    case GameErrorCode::JobCancelled:
      return ErrorCode::Cancelled;

    case GameErrorCode::Unknown:
      return ErrorCode::Unknown;
    }

    return ErrorCode::Unknown;
  }

  /**
   * @brief Convert a GameErrorCode to a stable readable name.
   */
  [[nodiscard]] inline const char *to_string(GameErrorCode code) noexcept
  {
    switch (code)
    {
    case GameErrorCode::None:
      return "none";

    case GameErrorCode::InvalidArgument:
      return "invalid_argument";
    case GameErrorCode::InvalidState:
      return "invalid_state";

    case GameErrorCode::AppAlreadyRunning:
      return "app_already_running";
    case GameErrorCode::AppNotRunning:
      return "app_not_running";

    case GameErrorCode::GameLoopAlreadyRunning:
      return "game_loop_already_running";
    case GameErrorCode::GameLoopNotRunning:
      return "game_loop_not_running";

    case GameErrorCode::SceneNotFound:
      return "scene_not_found";
    case GameErrorCode::SceneAlreadyExists:
      return "scene_already_exists";
    case GameErrorCode::SceneLoadFailed:
      return "scene_load_failed";
    case GameErrorCode::SceneUnloadFailed:
      return "scene_unload_failed";

    case GameErrorCode::EntityNotFound:
      return "entity_not_found";
    case GameErrorCode::ComponentNotFound:
      return "component_not_found";
    case GameErrorCode::SystemNotFound:
      return "system_not_found";

    case GameErrorCode::AssetNotFound:
      return "asset_not_found";
    case GameErrorCode::AssetAlreadyLoaded:
      return "asset_already_loaded";
    case GameErrorCode::AssetLoadFailed:
      return "asset_load_failed";
    case GameErrorCode::AssetInvalidPath:
      return "asset_invalid_path";
    case GameErrorCode::AssetUnsupportedType:
      return "asset_unsupported_type";

    case GameErrorCode::EventDispatchFailed:
      return "event_dispatch_failed";

    case GameErrorCode::JobRejected:
      return "job_rejected";
    case GameErrorCode::JobCancelled:
      return "job_cancelled";
    case GameErrorCode::JobFailed:
      return "job_failed";

    case GameErrorCode::ConfigNotFound:
      return "config_not_found";
    case GameErrorCode::ConfigInvalid:
      return "config_invalid";
    case GameErrorCode::ConfigLoadFailed:
      return "config_load_failed";

    case GameErrorCode::Unknown:
      return "unknown";
    }

    return "unknown";
  }

  /**
   * @brief Build a structured Vix error from a GameErrorCode.
   *
   * @param code Game-specific error code.
   * @param message Human-readable error message.
   */
  [[nodiscard]] inline vix::error::Error make_game_error(
      GameErrorCode code,
      std::string message)
  {
    return vix::error::Error(
        to_error_code(code),
        game_error_category(),
        std::move(message));
  }

} // namespace vix::game

#endif // VIX_GAME_GAME_ERROR_HPP
