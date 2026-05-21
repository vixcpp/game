/**
 *
 *  @file EventBus.hpp
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
#ifndef VIX_GAME_EVENT_BUS_HPP
#define VIX_GAME_EVENT_BUS_HPP

#include <atomic>
#include <cstddef>
#include <functional>
#include <mutex>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include <vix/game/Event.hpp>
#include <vix/game/EventType.hpp>
#include <vix/game/GameResult.hpp>

namespace vix::game
{
  /**
   * @brief Unique identifier for an event listener.
   */
  using EventListenerId = std::uint64_t;

  /**
   * @brief Invalid event listener identifier.
   */
  inline constexpr EventListenerId invalid_event_listener_id = 0;

  /**
   * @brief Check whether an event listener id is valid.
   *
   * @param id Listener id.
   * @return true if valid.
   */
  [[nodiscard]] inline constexpr bool is_valid_event_listener_id(
      EventListenerId id) noexcept
  {
    return id != invalid_event_listener_id;
  }

  /**
   * @brief Synchronous event dispatcher.
   *
   * EventBus allows systems to subscribe to events and dispatch events in a
   * deterministic way. V1 uses synchronous dispatch because it keeps ordering
   * explicit and easy to reason about.
   *
   * EventBus supports:
   * - listeners for a specific EventType
   * - listeners for a custom event name
   * - global listeners for all events
   */
  class EventBus
  {
  public:
    /**
     * @brief Event listener callback.
     */
    using Listener = std::function<void(const Event &)>;

    /**
     * @brief Construct an empty event bus.
     */
    EventBus() = default;

    EventBus(const EventBus &) = delete;
    EventBus &operator=(const EventBus &) = delete;

    /**
     * @brief Destroy the event bus.
     */
    ~EventBus() = default;

    /**
     * @brief Subscribe to a built-in event type.
     *
     * @param type Event type to listen to.
     * @param listener Callback invoked when the event is dispatched.
     * @return Listener id, or an error if the listener is empty.
     */
    [[nodiscard]] GameResult<EventListenerId> subscribe(
        EventType type,
        Listener listener);

    /**
     * @brief Subscribe to a custom event name.
     *
     * @param name Custom event name.
     * @param listener Callback invoked when the event is dispatched.
     * @return Listener id, or an error if the listener is empty.
     */
    [[nodiscard]] GameResult<EventListenerId> subscribe(
        const std::string &name,
        Listener listener);

    /**
     * @brief Subscribe to all events.
     *
     * @param listener Callback invoked for every dispatched event.
     * @return Listener id, or an error if the listener is empty.
     */
    [[nodiscard]] GameResult<EventListenerId> subscribe_all(
        Listener listener);

    /**
     * @brief Remove a listener.
     *
     * @param id Listener id returned by subscribe().
     * @return true if a listener was removed.
     */
    [[nodiscard]] bool unsubscribe(EventListenerId id);

    /**
     * @brief Dispatch an event to matching listeners.
     *
     * The event is copied internally so the bus can assign sequence and
     * timestamp metadata before invoking listeners.
     *
     * @param event Event to dispatch.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult dispatch(Event event);

    /**
     * @brief Remove all listeners.
     */
    void clear();

    /**
     * @brief Return the number of registered listeners.
     */
    [[nodiscard]] std::size_t listener_count() const;

    /**
     * @brief Return the total number of dispatched events.
     */
    [[nodiscard]] std::uint64_t dispatched_count() const noexcept;

  private:
    /**
     * @brief Internal listener record.
     */
    struct ListenerRecord
    {
      EventListenerId id{invalid_event_listener_id};
      Listener listener{};
    };

    /**
     * @brief Add a listener record to a list.
     *
     * @param listeners Destination listener list.
     * @param listener Listener callback.
     * @return Listener id, or an error if listener is empty.
     */
    [[nodiscard]] GameResult<EventListenerId> add_listener(
        std::vector<ListenerRecord> &listeners,
        Listener listener);

    /**
     * @brief Dispatch an event to a list of listeners.
     *
     * @param listeners Listener list.
     * @param event Event to dispatch.
     */
    static void dispatch_to(
        const std::vector<ListenerRecord> &listeners,
        const Event &event);

  private:
    /**
     * @brief Mutex protecting listener containers.
     */
    mutable std::mutex mutex_{};

    /**
     * @brief Listeners registered for built-in event types.
     */
    std::unordered_map<EventType, std::vector<ListenerRecord>> typed_listeners_{};

    /**
     * @brief Listeners registered for custom event names.
     */
    std::unordered_map<std::string, std::vector<ListenerRecord>> named_listeners_{};

    /**
     * @brief Listeners invoked for every event.
     */
    std::vector<ListenerRecord> global_listeners_{};

    /**
     * @brief Next listener id.
     */
    std::atomic<EventListenerId> next_listener_id_{1};

    /**
     * @brief Next event sequence number.
     */
    std::atomic<std::uint64_t> next_sequence_{1};

    /**
     * @brief Total dispatched event count.
     */
    std::atomic<std::uint64_t> dispatched_count_{0};
  };

} // namespace vix::game

#endif // VIX_GAME_EVENT_BUS_HPP
