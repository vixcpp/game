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
#include <vix/game/GameError.hpp>
#include <vix/game/NullRenderer.hpp>
#include <vix/game/NullWindow.hpp>
#include <stdexcept>
#include <utility>

namespace vix::game
{
  GameContext::GameContext(App &app) noexcept
      : app_(&app),
      input_(),
      window_(),
      renderer_(),
      renderer2d_(renderer_),
      initialized_(false)
  {
  }

  App &GameContext::app()
  {
    require_attached("access the app");
    return *app_;
  }

  const App &GameContext::app() const
  {
    require_attached("access the app");
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

  GameBoolResult GameContext::set_window_backend(
      std::unique_ptr<WindowBackend> backend)
  {
    if (initialized_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "cannot replace the window backend after runtime initialization; configure it before App::init()");
    }

    if (!backend)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "window backend cannot be null");
    }

    window_.set_backend(std::move(backend));
    return true;
  }

  GameBoolResult GameContext::set_renderer_backend(
      std::unique_ptr<RendererBackend> backend)
  {
    if (initialized_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "cannot replace the renderer backend after runtime initialization; configure it before App::init()");
    }

    if (!backend)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "renderer backend cannot be null");
    }

    renderer_.set_backend(std::move(backend));

    if (!renderer2d_.attached())
    {
      renderer2d_.attach(renderer_);
    }

    return true;
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

  bool GameContext::initialized() const noexcept
  {
    return initialized_;
  }

  GameBoolResult GameContext::init()
  {
    require_attached("initialize runtime backends");

    if (initialized_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "runtime context is already initialized");
    }

    if (!window_.has_backend())
    {
      window_.set_backend(std::make_unique<NullWindow>());
    }

    if (!renderer_.has_backend())
    {
      renderer_.set_backend(std::make_unique<NullRenderer>());
    }

    const auto window_config = app().config().headless
        ? WindowConfig::headless_config()
        : app().config().window;

    auto window_result = window_.open(window_config);
    if (!window_result)
    {
      return window_result.error();
    }

    auto renderer_result = renderer_.init(window_);
    if (!renderer_result)
    {
      window_.close();
      return renderer_result.error();
    }

    renderer2d_.attach(renderer_);
    renderer2d_.set_camera(Camera2D::from_size(window_.width(), window_.height()));
    initialized_ = true;
    return true;
  }

  void GameContext::shutdown() noexcept
  {
    if (!initialized_)
    {
      return;
    }

    clear();
    renderer_.shutdown();
    window_.close();
    initialized_ = false;
  }

  void GameContext::require_attached(const char *operation) const
  {
    if (!app_)
    {
      throw std::logic_error(std::string("cannot ") + operation + "; game context is detached");
    }
  }

} // namespace vix::game
