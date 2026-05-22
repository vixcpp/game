/**
 *
 *  @file GameRuntime.cpp
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

#include <vix/game/GameRuntime.hpp>
#include <vix/game/App.hpp>
#include <vix/game/GameError.hpp>
#include <vix/game/Camera2D.hpp>
#include <vix/game/WindowEvent.hpp>
#include <vix/game/Registry.hpp>

namespace vix::game
{
  GameRuntime::GameRuntime(App &app) noexcept
      : context_(app),
        initialized_(false)
  {
  }

  GameRuntime &GameRuntime::attach(App &app) noexcept
  {
    context_.attach(app);
    return *this;
  }

  void GameRuntime::detach() noexcept
  {
    shutdown();
    context_.detach();
  }

  bool GameRuntime::attached() const noexcept
  {
    return context_.attached();
  }

  GameBoolResult GameRuntime::init()
  {
    if (initialized_)
    {
      return true;
    }

    if (!attached())
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "game runtime is not attached to an app");
    }

    initialized_ = true;
    return true;
  }

  void GameRuntime::shutdown() noexcept
  {
    if (!initialized_)
    {
      return;
    }

    context_.clear();
    initialized_ = false;
  }

  bool GameRuntime::initialized() const noexcept
  {
    return initialized_;
  }

  void GameRuntime::begin_frame(const Frame &frame)
  {
    (void)frame;

    diagnostics_.frame_index = frame.index;
    diagnostics_.delta_ms = frame.delta_ms();
    context_.begin_frame();

    auto events = context_.window().poll_events();

    for (const auto &event : events)
    {
      if (event.is_close_request())
      {
        context_.app().stop();
      }

      if (event.type == WindowEventType::Resized &&
          event.width > 0 &&
          event.height > 0)
      {
        context_.renderer().resize(event.width, event.height);
        context_.renderer2d().set_camera(
            Camera2D::from_size(event.width, event.height));
      }

      context_.input().handle_window_event(event);
    }

    auto &renderer2d = context_.renderer2d();

    if (renderer2d.attached())
    {
      renderer2d.begin_frame();
    }
  }
  void GameRuntime::update(const Frame &frame)
  {
    context_.scenes().update(frame);
  }

  void GameRuntime::fixed_update(const Frame &frame)
  {
    context_.scenes().fixed_update(frame);
  }

  void GameRuntime::render(const Frame &frame)
  {
    (void)frame;

    auto &renderer2d = context_.renderer2d();

    if (!renderer2d.attached())
    {
      return;
    }

    if (!renderer2d.frame_active())
    {
      return;
    }

    diagnostics_.sprite_command_count = renderer2d.command_count();

    renderer2d.end_frame();
  }

  void GameRuntime::end_frame(const Frame &frame)
  {
    (void)frame;

    diagnostics_.window_backend = context_.window().backend_name();
    diagnostics_.renderer_backend = context_.renderer().backend_name();
    diagnostics_.window_width = context_.window().width();
    diagnostics_.window_height = context_.window().height();
    diagnostics_.loaded_asset_count = context_.assets().size();
    diagnostics_.scene_count = context_.scenes().size();
    diagnostics_.active_scene = context_.scenes().active_name();

    context_.end_frame();
  }

  GameContext &GameRuntime::context() noexcept
  {
    return context_;
  }

  const GameContext &GameRuntime::context() const noexcept
  {
    return context_;
  }

  RuntimeDiagnostics GameRuntime::diagnostics() const
  {
    return diagnostics_;
  }

} // namespace vix::game
