/**
 *
 *  @file GameContext.cpp
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

#include <vix/game/GameContext.hpp>
#include <vix/game/App.hpp>

namespace vix::game
{
  GameContext::GameContext(App &app) noexcept
      : app_(&app),
        input_(),
        window_(),
        renderer_(),
        renderer2d_(renderer_)
  {
  }

  GameContext &GameContext::attach(App &app) noexcept
  {
    app_ = &app;

    if (!renderer2d_.attached())
    {
      renderer2d_.attach(renderer_);
    }

    return *this;
  }

  void GameContext::detach() noexcept
  {
    app_ = nullptr;
    renderer2d_.detach();
  }

  bool GameContext::attached() const noexcept
  {
    return app_ != nullptr;
  }

  App &GameContext::app()
  {
    return *app_;
  }

  const App &GameContext::app() const
  {
    return *app_;
  }

  SceneManager &GameContext::scenes()
  {
    return app().scenes();
  }

  const SceneManager &GameContext::scenes() const
  {
    return app().scenes();
  }

  EventBus &GameContext::events()
  {
    return app().events();
  }

  const EventBus &GameContext::events() const
  {
    return app().events();
  }

  AssetManager &GameContext::assets()
  {
    return app().assets();
  }

  const AssetManager &GameContext::assets() const
  {
    return app().assets();
  }

  AsyncAssetLoader &GameContext::async_assets()
  {
    return app().async_assets();
  }

  const AsyncAssetLoader &GameContext::async_assets() const
  {
    return app().async_assets();
  }

  JobSystem &GameContext::jobs()
  {
    return app().jobs();
  }

  const JobSystem &GameContext::jobs() const
  {
    return app().jobs();
  }

  InputSystem &GameContext::input() noexcept
  {
    return input_;
  }

  const InputSystem &GameContext::input() const noexcept
  {
    return input_;
  }

  Window &GameContext::window() noexcept
  {
    return window_;
  }

  const Window &GameContext::window() const noexcept
  {
    return window_;
  }

  Renderer &GameContext::renderer() noexcept
  {
    return renderer_;
  }

  const Renderer &GameContext::renderer() const noexcept
  {
    return renderer_;
  }

  Renderer2D &GameContext::renderer2d() noexcept
  {
    return renderer2d_;
  }

  const Renderer2D &GameContext::renderer2d() const noexcept
  {
    return renderer2d_;
  }

  void GameContext::begin_frame()
  {
    input_.begin_frame();
  }

  void GameContext::end_frame()
  {
  }

  void GameContext::clear()
  {
    input_.clear();
    renderer2d_.clear_commands();
  }

} // namespace vix::game
