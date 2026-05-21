/**
 *
 *  @file JobSystem.cpp
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

#include <vix/game/JobSystem.hpp>

#include <thread>

namespace vix::game
{
  JobSystem::JobSystem()
      : JobSystem(std::thread::hardware_concurrency())
  {
  }

  JobSystem::JobSystem(std::size_t worker_count)
      : events_(nullptr),
        worker_count_(worker_count == 0 ? 1 : worker_count),
        pool_(nullptr),
        shutdown_done_(false)
  {
  }

  JobSystem::~JobSystem()
  {
    shutdown();
  }

  void JobSystem::set_event_bus(EventBus *bus) noexcept
  {
    events_ = bus;
  }

  GameBoolResult JobSystem::start()
  {
    if (pool_)
    {
      return true;
    }

    pool_ = std::make_unique<vix::threadpool::ThreadPool>(make_config());
    shutdown_done_ = false;

    return true;
  }

  void JobSystem::stop() noexcept
  {
    if (pool_)
    {
      pool_->shutdown();
    }
  }

  void JobSystem::shutdown() noexcept
  {
    if (shutdown_done_)
    {
      return;
    }

    if (pool_)
    {
      pool_->shutdown();
      pool_.reset();
    }

    shutdown_done_ = true;
  }

  bool JobSystem::running() const noexcept
  {
    return pool_ != nullptr && pool_->running();
  }

  GameResult<JobHandle> JobSystem::submit(
      Job job,
      JobPriority priority)
  {
    if (!job)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "job cannot be empty");
    }

    auto started = start();
    if (!started)
    {
      return started.error();
    }

    auto options = make_options(priority, false);

    try
    {
      auto handle = pool_->handle(
          [job = std::move(job)]()
          {
            job();
          },
          options);

      const JobId id = handle.id();
      dispatch_job_event(EventType::JobStarted, id);

      return JobHandle(std::move(handle));
    }
    catch (...)
    {
      return make_game_error(
          GameErrorCode::JobRejected,
          "failed to submit job");
    }
  }

  GameBoolResult JobSystem::submit_detached(
      Job job,
      JobPriority priority)
  {
    if (!job)
    {
      return make_game_error(
          GameErrorCode::InvalidArgument,
          "job cannot be empty");
    }

    auto started = start();
    if (!started)
    {
      return started.error();
    }

    auto options = make_options(priority, true);

    try
    {
      pool_->post(
          [this, job = std::move(job)]()
          {
            try
            {
              job();
            }
            catch (...)
            {
              dispatch_job_event(EventType::JobFailed, invalid_job_id);
            }
          },
          options);

      return true;
    }
    catch (...)
    {
      return make_game_error(
          GameErrorCode::JobRejected,
          "failed to submit detached job");
    }
  }

  vix::threadpool::ThreadPoolStats JobSystem::stats() const
  {
    if (!pool_)
    {
      return {};
    }

    return pool_->stats();
  }

  std::size_t JobSystem::worker_count() const noexcept
  {
    return worker_count_;
  }

  vix::threadpool::ThreadPool &JobSystem::native()
  {
    return *pool_;
  }

  const vix::threadpool::ThreadPool &JobSystem::native() const
  {
    return *pool_;
  }

  vix::threadpool::ThreadPoolConfig JobSystem::make_config() const
  {
    vix::threadpool::ThreadPoolConfig config;
    config.thread_count = worker_count_;
    config.max_thread_count = worker_count_;
    return config.normalized();
  }

  vix::threadpool::TaskOptions JobSystem::make_options(
      JobPriority priority,
      bool detached) const
  {
    vix::threadpool::TaskOptions options;
    options.set_priority(to_task_priority(priority));
    options.set_detached(detached);
    return options;
  }

  void JobSystem::dispatch_job_event(EventType type, JobId id)
  {
    if (!events_)
    {
      return;
    }

    Event event(type);
    event.set_source("job_system");

    if (is_valid_job_id(id))
    {
      event.set_target(std::to_string(id));
      event.set_field("job_id", std::to_string(id));
    }

    (void)events_->dispatch(std::move(event));
  }

} // namespace vix::game
