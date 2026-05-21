/**
 *
 *  @file EventType.hpp
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
#ifndef VIX_GAME_EVENT_TYPE_HPP
#define VIX_GAME_EVENT_TYPE_HPP

#include <cstdint>

namespace vix::game
{
  /**
   * @brief Built-in event types used by vix/game.
   *
   * EventType identifies the kind of event being dispatched through EventBus.
   * Custom user events can use EventType::Custom with a custom string name
   * inside Event.
   */
  enum class EventType : std::uint32_t
  {
    /**
     * @brief Unknown event.
     */
    Unknown = 0,

    /**
     * @brief Application started.
     */
    AppStarted,

    /**
     * @brief Application is stopping.
     */
    AppStopping,

    /**
     * @brief Application stopped.
     */
    AppStopped,

    /**
     * @brief Frame started.
     */
    FrameStarted,

    /**
     * @brief Frame ended.
     */
    FrameEnded,

    /**
     * @brief Scene was loaded.
     */
    SceneLoaded,

    /**
     * @brief Scene was unloaded.
     */
    SceneUnloaded,

    /**
     * @brief Active scene changed.
     */
    SceneChanged,

    /**
     * @brief Entity was created.
     */
    EntityCreated,

    /**
     * @brief Entity was destroyed.
     */
    EntityDestroyed,

    /**
     * @brief Asset was requested.
     */
    AssetRequested,

    /**
     * @brief Asset was loaded.
     */
    AssetLoaded,

    /**
     * @brief Asset loading failed.
     */
    AssetLoadFailed,

    /**
     * @brief Background job was started.
     */
    JobStarted,

    /**
     * @brief Background job completed successfully.
     */
    JobCompleted,

    /**
     * @brief Background job failed.
     */
    JobFailed,

    /**
     * @brief User-defined event.
     */
    Custom
  };

  /**
   * @brief Convert an event type to a stable readable name.
   *
   * @param type Event type.
   * @return Static event type name.
   */
  [[nodiscard]] inline constexpr const char *to_string(EventType type) noexcept
  {
    switch (type)
    {
    case EventType::Unknown:
      return "unknown";

    case EventType::AppStarted:
      return "app_started";
    case EventType::AppStopping:
      return "app_stopping";
    case EventType::AppStopped:
      return "app_stopped";

    case EventType::FrameStarted:
      return "frame_started";
    case EventType::FrameEnded:
      return "frame_ended";

    case EventType::SceneLoaded:
      return "scene_loaded";
    case EventType::SceneUnloaded:
      return "scene_unloaded";
    case EventType::SceneChanged:
      return "scene_changed";

    case EventType::EntityCreated:
      return "entity_created";
    case EventType::EntityDestroyed:
      return "entity_destroyed";

    case EventType::AssetRequested:
      return "asset_requested";
    case EventType::AssetLoaded:
      return "asset_loaded";
    case EventType::AssetLoadFailed:
      return "asset_load_failed";

    case EventType::JobStarted:
      return "job_started";
    case EventType::JobCompleted:
      return "job_completed";
    case EventType::JobFailed:
      return "job_failed";

    case EventType::Custom:
      return "custom";
    }

    return "unknown";
  }

  /**
   * @brief Check whether an event type is related to the application lifecycle.
   */
  [[nodiscard]] inline constexpr bool is_app_event(EventType type) noexcept
  {
    return type == EventType::AppStarted ||
           type == EventType::AppStopping ||
           type == EventType::AppStopped;
  }

  /**
   * @brief Check whether an event type is related to frame lifecycle.
   */
  [[nodiscard]] inline constexpr bool is_frame_event(EventType type) noexcept
  {
    return type == EventType::FrameStarted ||
           type == EventType::FrameEnded;
  }

  /**
   * @brief Check whether an event type is related to scenes.
   */
  [[nodiscard]] inline constexpr bool is_scene_event(EventType type) noexcept
  {
    return type == EventType::SceneLoaded ||
           type == EventType::SceneUnloaded ||
           type == EventType::SceneChanged;
  }

  /**
   * @brief Check whether an event type is related to assets.
   */
  [[nodiscard]] inline constexpr bool is_asset_event(EventType type) noexcept
  {
    return type == EventType::AssetRequested ||
           type == EventType::AssetLoaded ||
           type == EventType::AssetLoadFailed;
  }

  /**
   * @brief Check whether an event type is related to jobs.
   */
  [[nodiscard]] inline constexpr bool is_job_event(EventType type) noexcept
  {
    return type == EventType::JobStarted ||
           type == EventType::JobCompleted ||
           type == EventType::JobFailed;
  }

} // namespace vix::game

#endif // VIX_GAME_EVENT_TYPE_HPP
