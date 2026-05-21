/**
 *
 *  @file JobSystem.hpp
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
#ifndef VIX_GAME_JOB_SYSTEM_HPP
#define VIX_GAME_JOB_SYSTEM_HPP

#include <cstddef>
#include <functional>
#include <memory>
#include <utility>

#include <vix/game/Event.hpp>
#include <vix/game/EventBus.hpp>
#include <vix/game/EventType.hpp>
#include <vix/game/GameError.hpp>
#include <vix/game/GameResult.hpp>
#include <vix/game/JobHandle.hpp>
#include <vix/game/JobPriority.hpp>
#include <vix/threadpool/CancellationSource.hpp>
#include <vix/threadpool/TaskOptions.hpp>
#include <vix/threadpool/ThreadPool.hpp>
#include <vix/threadpool/ThreadPoolConfig.hpp>
#include <vix/threadpool/ThreadPoolStats.hpp>

namespace vix::game
{
  /**
   * @brief Background job system for vix/game.
   *
   * JobSystem is the game-facing wrapper around vix::threadpool.
   *
   * It is used for:
   * - background asset loading
   * - scene preparation
   * - expensive CPU work
   * - future parallel updates
   *
   * V1 keeps the API simple and synchronous from the caller perspective:
   * submit work, receive a JobHandle, optionally wait or cancel.
   */
  class JobSystem
  {
  public:
    /**
     * @brief Job function type.
     */
    using Job = std::function<void()>;

    /**
     * @brief Construct a job system with default configuration.
     */
    JobSystem();

    /**
     * @brief Construct a job system with an explicit worker count.
     *
     * @param worker_count Number of worker threads.
     */
    explicit JobSystem(std::size_t worker_count);

    JobSystem(const JobSystem &) = delete;
    JobSystem &operator=(const JobSystem &) = delete;

    /**
     * @brief Destroy the job system.
     */
    ~JobSystem();

    /**
     * @brief Set the event bus used for job lifecycle events.
     *
     * @param bus Event bus pointer. May be nullptr.
     */
    void set_event_bus(EventBus *bus) noexcept;

    /**
     * @brief Start the job system.
     *
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult start();

    /**
     * @brief Request the job system to stop.
     */
    void stop() noexcept;

    /**
     * @brief Shut down the job system.
     *
     * This function is idempotent.
     */
    void shutdown() noexcept;

    /**
     * @brief Return true if the job system is running.
     */
    [[nodiscard]] bool running() const noexcept;

    /**
     * @brief Submit a background job.
     *
     * @param job Job function to execute.
     * @param priority Job priority.
     * @return Job handle, or a structured error.
     */
    [[nodiscard]] GameResult<JobHandle> submit(
        Job job,
        JobPriority priority = JobPriority::Normal);

    /**
     * @brief Submit a detached background job.
     *
     * Detached jobs do not return a handle.
     *
     * @param job Job function to execute.
     * @param priority Job priority.
     * @return true on success, or a structured error.
     */
    [[nodiscard]] GameBoolResult submit_detached(
        Job job,
        JobPriority priority = JobPriority::Normal);

    /**
     * @brief Return threadpool statistics.
     */
    [[nodiscard]] vix::threadpool::ThreadPoolStats stats() const;

    /**
     * @brief Return the worker count configured for the system.
     */
    [[nodiscard]] std::size_t worker_count() const noexcept;

    /**
     * @brief Return the underlying threadpool.
     */
    [[nodiscard]] vix::threadpool::ThreadPool &native();

    /**
     * @brief Return the underlying threadpool.
     */
    [[nodiscard]] const vix::threadpool::ThreadPool &native() const;

  private:
    /**
     * @brief Create threadpool configuration from the current settings.
     */
    [[nodiscard]] vix::threadpool::ThreadPoolConfig make_config() const;

    /**
     * @brief Build task options for a game job.
     *
     * @param priority Job priority.
     * @param detached Whether the job is detached.
     * @return Threadpool task options.
     */
    [[nodiscard]] vix::threadpool::TaskOptions make_options(
        JobPriority priority,
        bool detached) const;

    /**
     * @brief Dispatch a job lifecycle event if an event bus is available.
     *
     * @param type Event type.
     * @param id Job id.
     */
    void dispatch_job_event(EventType type, JobId id);

  private:
    /**
     * @brief Event bus used for job events.
     */
    EventBus *events_{nullptr};

    /**
     * @brief Worker count.
     */
    std::size_t worker_count_{0};

    /**
     * @brief Threadpool used internally.
     */
    std::unique_ptr<vix::threadpool::ThreadPool> pool_{};

    /**
     * @brief Whether shutdown was already requested.
     */
    bool shutdown_done_{false};
  };

} // namespace vix::game

#endif // VIX_GAME_JOB_SYSTEM_HPP
