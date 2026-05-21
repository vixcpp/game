/**
 *
 *  @file Event.hpp
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
#ifndef VIX_GAME_EVENT_HPP
#define VIX_GAME_EVENT_HPP

#include <cstdint>
#include <string>
#include <unordered_map>
#include <utility>

#include <vix/game/EventType.hpp>
#include <vix/time/Timestamp.hpp>

namespace vix::game
{
  /**
   * @brief Lightweight event object dispatched through EventBus.
   *
   * Event represents something that happened inside the game runtime:
   * app lifecycle changes, frame lifecycle, scene changes, asset loading,
   * job completion, or user-defined events.
   *
   * V1 keeps the payload simple with string metadata fields. More advanced
   * typed payloads can be added later without changing the basic event model.
   */
  struct Event
  {
    /**
     * @brief Built-in event type.
     */
    EventType type{EventType::Unknown};

    /**
     * @brief Optional custom event name.
     *
     * Used mainly when type == EventType::Custom.
     */
    std::string name{};

    /**
     * @brief Optional source that emitted the event.
     *
     * Examples:
     * - "app"
     * - "scene"
     * - "asset_manager"
     * - "job_system"
     */
    std::string source{};

    /**
     * @brief Optional target affected by the event.
     *
     * Examples:
     * - scene id
     * - asset id
     * - entity id
     * - job id
     */
    std::string target{};

    /**
     * @brief String metadata attached to the event.
     */
    std::unordered_map<std::string, std::string> fields{};

    /**
     * @brief Wall-clock timestamp when the event was created.
     */
    vix::time::Timestamp created_at{};

    /**
     * @brief Numeric sequence assigned by EventBus.
     *
     * A value of 0 means the event has not been dispatched by a bus yet.
     */
    std::uint64_t sequence{0};

    /**
     * @brief Create an empty event.
     */
    Event() = default;

    /**
     * @brief Create an event from a built-in type.
     *
     * @param event_type Event type.
     */
    explicit Event(EventType event_type)
        : type(event_type),
          name(to_string(event_type))
    {
    }

    /**
     * @brief Create a custom event.
     *
     * @param custom_name Custom event name.
     * @return Custom event.
     */
    [[nodiscard]] static Event custom(std::string custom_name)
    {
      Event event;
      event.type = EventType::Custom;
      event.name = std::move(custom_name);
      return event;
    }

    /**
     * @brief Create an event from a built-in type.
     *
     * @param event_type Event type.
     * @return Event object.
     */
    [[nodiscard]] static Event make(EventType event_type)
    {
      return Event(event_type);
    }

    /**
     * @brief Set the event source.
     *
     * @param value Source name.
     * @return Reference to this event.
     */
    Event &set_source(std::string value)
    {
      source = std::move(value);
      return *this;
    }

    /**
     * @brief Set the event target.
     *
     * @param value Target identifier.
     * @return Reference to this event.
     */
    Event &set_target(std::string value)
    {
      target = std::move(value);
      return *this;
    }

    /**
     * @brief Add or replace a metadata field.
     *
     * @param key Field key.
     * @param value Field value.
     * @return Reference to this event.
     */
    Event &set_field(std::string key, std::string value)
    {
      fields[std::move(key)] = std::move(value);
      return *this;
    }

    /**
     * @brief Return true if the event has a metadata field.
     *
     * @param key Field key.
     * @return true if the field exists.
     */
    [[nodiscard]] bool has_field(const std::string &key) const
    {
      return fields.find(key) != fields.end();
    }

    /**
     * @brief Return a metadata field or a default value.
     *
     * @param key Field key.
     * @param fallback Value returned if the field does not exist.
     * @return Field value or fallback.
     */
    [[nodiscard]] std::string field_or(
        const std::string &key,
        std::string fallback = {}) const
    {
      const auto it = fields.find(key);
      if (it == fields.end())
      {
        return fallback;
      }

      return it->second;
    }

    /**
     * @brief Return the stable readable name of this event.
     *
     * @return Custom name if set, otherwise built-in type name.
     */
    [[nodiscard]] std::string event_name() const
    {
      if (!name.empty())
      {
        return name;
      }

      return to_string(type);
    }

    /**
     * @brief Check whether this is a custom user event.
     */
    [[nodiscard]] constexpr bool is_custom() const noexcept
    {
      return type == EventType::Custom;
    }
  };

} // namespace vix::game

#endif // VIX_GAME_EVENT_HPP
