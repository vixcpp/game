/**
 *
 *  @file App.cpp
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

#include <vix/game/App.hpp>
#include <vix/game/AsyncAssetLoader.hpp>
#include <vix/game/AssetManager.hpp>
#include <vix/game/Event.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>
#include <vix/game/GameError.hpp>
#include <vix/game/JobSystem.hpp>
#include <vix/game/SceneManager.hpp>
#include <vix/log/GlobalLog.hpp>

namespace vix::game
{
  App::App()
      : App(AppConfig::defaults())
  {
  }

  App::App(AppConfig config)
      : config_(std::move(config)),
        loop_(config_.time_step),
        runtime_(*this),
        scenes_(),
        events_(),
        assets_(),
        jobs_(),
        async_assets_(),
        initialized_(false),
        shutdown_done_(false),
        running_(false)
  {
    create_systems();
  }

  App::~App()
  {
    shutdown();
  }

  GameBoolResult App::init()
  {
    if (initialized_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "app is already initialized; call run() or shutdown() instead");
    }

    if (shutdown_done_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "app has been shut down and cannot be initialized again");
    }

    auto valid = config_.validate();
    if (!valid)
    {
      return valid.error();
    }

    configure_loop();

    auto runtime_result = runtime_.init();
    if (!runtime_result)
    {
      return runtime_result.error();
    }

    if (config_.log_startup)
    {
      vix::log::info("game app initialized title={}", config_.title);
    }

    initialized_ = true;
    if (events_)
    {
      (void)events_->dispatch(
          Event(EventType::AppStarted)
              .set_source("app")
              .set_field("title", config_.title));
    }

    return true;
  }

  GameBoolResult App::run()
  {
    if (!initialized_)
    {
      auto result = init();
      if (!result)
      {
        return result.error();
      }
    }

    if (running())
    {
      return make_game_error(
          GameErrorCode::AppAlreadyRunning,
          "app is already running");
    }

    // Reassert App's callbacks so direct GameLoop customization cannot bypass
    // the canonical runtime lifecycle.
    configure_loop();

    running_ = true;
    stopping_ = false;
    runtime_.begin_run();

    auto result = loop_.run();

    runtime_.end_run();
    running_ = false;

    if (shutdown_requested_)
    {
      shutdown_requested_ = false;
      shutdown();
    }

    return result;
  }

  void App::stop() noexcept
  {
    if (running_ && !stopping_ && events_)
    {
      (void)events_->dispatch(Event(EventType::AppStopping).set_source("app"));
    }

    stopping_ = true;
    loop_.stop();
  }

  void App::shutdown() noexcept
  {
    if (shutdown_done_)
    {
      return;
    }

    if (running_)
    {
      shutdown_requested_ = true;
      stop();
      return;
    }

    stop();

    runtime_.shutdown();

    if (scenes_)
    {
      scenes_->clear();
    }

    if (jobs_)
    {
      jobs_->shutdown();
    }

    if (assets_)
    {
      assets_->clear();
    }

    if (events_)
    {
      (void)events_->dispatch(Event(EventType::AppStopped).set_source("app"));
      events_->clear();
    }

    if (initialized_ && config_.log_shutdown)
    {
      vix::log::info("game app shutdown title={}", config_.title);
    }

    initialized_ = false;
    shutdown_done_ = true;
  }

  bool App::initialized() const noexcept
  {
    return initialized_;
  }

  bool App::running() const noexcept
  {
    return running_;
  }

  const AppConfig &App::config() const noexcept
  {
    return config_;
  }

  GameBoolResult App::set_config(AppConfig config)
  {
    if (initialized_ || running() || shutdown_done_)
    {
      return make_game_error(
          GameErrorCode::InvalidState,
          "cannot change app config after initialization");
    }

    auto valid = config.validate();
    if (!valid)
    {
      return valid.error();
    }

    config_ = std::move(config);
    loop_.set_time_step(config_.time_step);

    return true;
  }

  GameLoop &App::loop() noexcept
  {
    return loop_;
  }

  const GameLoop &App::loop() const noexcept
  {
    return loop_;
  }

  GameRuntime &App::runtime() noexcept
  {
    return runtime_;
  }

  const GameRuntime &App::runtime() const noexcept
  {
    return runtime_;
  }

  SceneManager &App::scenes()
  {
    return *scenes_;
  }

  const SceneManager &App::scenes() const
  {
    return *scenes_;
  }

  EventBus &App::events()
  {
    return *events_;
  }

  const EventBus &App::events() const
  {
    return *events_;
  }

  AssetManager &App::assets()
  {
    return *assets_;
  }

  const AssetManager &App::assets() const
  {
    return *assets_;
  }

  JobSystem &App::jobs()
  {
    return *jobs_;
  }

  const JobSystem &App::jobs() const
  {
    return *jobs_;
  }

  AsyncAssetLoader &App::async_assets()
  {
    return *async_assets_;
  }

  const AsyncAssetLoader &App::async_assets() const
  {
    return *async_assets_;
  }

  App &App::set_title(std::string title)
  {
    config_.title = std::move(title);
    return *this;
  }

  App &App::set_target_fps(std::uint32_t fps) noexcept
  {
    config_.set_target_fps(fps);
    loop_.set_time_step(config_.time_step);
    return *this;
  }

  void App::create_systems()
  {
    if (!events_)
    {
      events_ = std::make_unique<EventBus>();
    }

    if (!scenes_)
    {
      scenes_ = std::make_unique<SceneManager>(*this);
    }

    if (!assets_)
    {
      assets_ = std::make_unique<AssetManager>(config_.asset_root);
      assets_->set_event_bus(events_.get());
    }

    if (!jobs_)
    {
      jobs_ = std::make_unique<JobSystem>();
      jobs_->set_event_bus(events_.get());
    }

    if (!async_assets_)
    {
      async_assets_ = std::make_unique<AsyncAssetLoader>(*assets_, *jobs_);
    }
  }

  void App::configure_loop()
  {
    loop_.set_time_step(config_.time_step);

    loop_.set_update_callback(
        [this](const Frame &frame)
        {
          update(frame);
        });

    loop_.set_fixed_update_callback(
        [this](const Frame &frame)
        {
          fixed_update(frame);
        });
  }

  void App::update(const Frame &frame)
  {
    if (events_)
    {
      (void)events_->dispatch(
          Event(EventType::FrameStarted)
              .set_source("app")
              .set_field("frame", std::to_string(frame.index)));
    }

    runtime_.begin_frame(frame);
    runtime_.update(frame);

    if (update_callback_)
    {
      update_callback_(frame);
    }

    runtime_.render(frame);
    runtime_.end_frame(frame);

    if (events_)
    {
      (void)events_->dispatch(
          Event(EventType::FrameEnded)
              .set_source("app")
              .set_field("frame", std::to_string(frame.index)));
    }
  }

  void App::fixed_update(const Frame &frame)
  {
    runtime_.fixed_update(frame);

    if (fixed_update_callback_)
    {
      fixed_update_callback_(frame);
    }
  }

} // namespace vix::game
