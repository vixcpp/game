/**
 *
 *  @file ScriptRuntime.cpp
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

#include <vix/game/ScriptRuntime.hpp>

#include <utility>

#include <vix/game/GameError.hpp>

namespace vix::game
{
  ScriptRuntime::ScriptRuntime(GameContext &context) noexcept
      : context_(&context),
        callbacks_(),
        initialized_(false)
  {
  }

  ScriptRuntime &ScriptRuntime::attach(GameContext &context) noexcept
  {
    context_ = &context;
    return *this;
  }

  void ScriptRuntime::detach() noexcept
  {
    shutdown();
    context_ = nullptr;
  }

  bool ScriptRuntime::attached() const noexcept
  {
    return context_ != nullptr;
  }

  GameContext &ScriptRuntime::context()
  {
    return *context_;
  }

  const GameContext &ScriptRuntime::context() const
  {
    return *context_;
  }

  GameBoolResult ScriptRuntime::init()
  {
    if (initialized_)
    {
      return true;
    }

    if (!attached())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "script runtime is not attached to a game context");
    }

    initialized_ = true;
    return true;
  }

  void ScriptRuntime::shutdown() noexcept
  {
    if (!initialized_)
    {
      return;
    }

    clear();
    initialized_ = false;
  }

  bool ScriptRuntime::initialized() const noexcept
  {
    return initialized_;
  }

  GameBoolResult ScriptRuntime::register_callback(
      std::string name,
      ScriptCallback callback)
  {
    if (name.empty())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "script callback name cannot be empty");
    }

    if (!callback)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "script callback cannot be empty");
    }

    callbacks_[std::move(name)] = std::move(callback);
    return true;
  }

  GameBoolResult ScriptRuntime::unregister_callback(
      const std::string &name)
  {
    const auto erased = callbacks_.erase(name);
    if (erased == 0)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "script callback not found");
    }

    return true;
  }

  bool ScriptRuntime::has_callback(const std::string &name) const
  {
    return callbacks_.find(name) != callbacks_.end();
  }

  GameBoolResult ScriptRuntime::call(
      const std::string &name,
      const Frame &frame)
  {
    if (!attached())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "script runtime is not attached to a game context");
    }

    const auto it = callbacks_.find(name);
    if (it == callbacks_.end())
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "script callback not found");
    }

    it->second(context(), frame);
    return true;
  }

  void ScriptRuntime::update(const Frame &frame)
  {
    if (!attached())
    {
      return;
    }

    for (auto &[name, callback] : callbacks_)
    {
      (void)name;

      if (callback)
      {
        callback(context(), frame);
      }
    }
  }

  void ScriptRuntime::clear()
  {
    callbacks_.clear();
  }

  std::size_t ScriptRuntime::size() const noexcept
  {
    return callbacks_.size();
  }

  bool ScriptRuntime::empty() const noexcept
  {
    return callbacks_.empty();
  }

} // namespace vix::game
