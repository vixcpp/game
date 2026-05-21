/**
 *
 *  @file EventBus.cpp
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

#include <vix/game/EventBus.hpp>

#include <algorithm>
#include <exception>

#include <vix/game/GameError.hpp>
#include <vix/time/Timestamp.hpp>

namespace vix::game
{
  GameResult<EventListenerId> EventBus::subscribe(
      EventType type,
      Listener listener)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return add_listener(typed_listeners_[type], std::move(listener));
  }

  GameResult<EventListenerId> EventBus::subscribe(
      const std::string &name,
      Listener listener)
  {
    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "event name cannot be empty");
    }

    std::lock_guard<std::mutex> lock(mutex_);
    return add_listener(named_listeners_[name], std::move(listener));
  }

  GameResult<EventListenerId> EventBus::subscribe_all(Listener listener)
  {
    std::lock_guard<std::mutex> lock(mutex_);
    return add_listener(global_listeners_, std::move(listener));
  }

  bool EventBus::unsubscribe(EventListenerId id)
  {
    if (!is_valid_event_listener_id(id))
    {
      return false;
    }

    std::lock_guard<std::mutex> lock(mutex_);

    auto remove_from = [id](std::vector<ListenerRecord> &listeners)
    {
      const auto old_size = listeners.size();

      listeners.erase(
          std::remove_if(
              listeners.begin(),
              listeners.end(),
              [id](const ListenerRecord &record)
              {
                return record.id == id;
              }),
          listeners.end());

      return listeners.size() != old_size;
    };

    bool removed = remove_from(global_listeners_);

    for (auto &[type, listeners] : typed_listeners_)
    {
      (void)type;
      removed = remove_from(listeners) || removed;
    }

    for (auto &[name, listeners] : named_listeners_)
    {
      (void)name;
      removed = remove_from(listeners) || removed;
    }

    return removed;
  }

  GameBoolResult EventBus::dispatch(Event event)
  {
    event.sequence = next_sequence_.fetch_add(1);
    event.created_at = vix::time::Timestamp::now();

    std::vector<ListenerRecord> global;
    std::vector<ListenerRecord> typed;
    std::vector<ListenerRecord> named;

    {
      std::lock_guard<std::mutex> lock(mutex_);

      global = global_listeners_;

      const auto typed_it = typed_listeners_.find(event.type);
      if (typed_it != typed_listeners_.end())
      {
        typed = typed_it->second;
      }

      const std::string event_name = event.event_name();
      const auto named_it = named_listeners_.find(event_name);
      if (named_it != named_listeners_.end())
      {
        named = named_it->second;
      }
    }

    try
    {
      dispatch_to(global, event);
      dispatch_to(typed, event);
      dispatch_to(named, event);
    }
    catch (const std::exception &e)
    {
      return make_game_error(
          GameErrorCode::EventDispatchFailed,
          e.what());
    }
    catch (...)
    {
      return make_game_error(
          GameErrorCode::EventDispatchFailed,
          "event listener threw an unknown exception");
    }

    dispatched_count_.fetch_add(1);
    return true;
  }

  void EventBus::clear()
  {
    std::lock_guard<std::mutex> lock(mutex_);

    typed_listeners_.clear();
    named_listeners_.clear();
    global_listeners_.clear();
  }

  std::size_t EventBus::listener_count() const
  {
    std::lock_guard<std::mutex> lock(mutex_);

    std::size_t count = global_listeners_.size();

    for (const auto &[type, listeners] : typed_listeners_)
    {
      (void)type;
      count += listeners.size();
    }

    for (const auto &[name, listeners] : named_listeners_)
    {
      (void)name;
      count += listeners.size();
    }

    return count;
  }

  std::uint64_t EventBus::dispatched_count() const noexcept
  {
    return dispatched_count_.load();
  }

  GameResult<EventListenerId> EventBus::add_listener(
      std::vector<ListenerRecord> &listeners,
      Listener listener)
  {
    if (!listener)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "event listener cannot be empty");
    }

    const EventListenerId id = next_listener_id_.fetch_add(1);

    ListenerRecord record;
    record.id = id;
    record.listener = std::move(listener);

    listeners.push_back(std::move(record));
    return id;
  }

  void EventBus::dispatch_to(
      const std::vector<ListenerRecord> &listeners,
      const Event &event)
  {
    for (const auto &record : listeners)
    {
      if (record.listener)
      {
        record.listener(event);
      }
    }
  }

} // namespace vix::game
