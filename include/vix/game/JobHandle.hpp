/**
 *
 *  @file JobHandle.hpp
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
#ifndef VIX_GAME_JOB_HANDLE_HPP
#define VIX_GAME_JOB_HANDLE_HPP

#include <utility>

#include <vix/threadpool/TaskHandle.hpp>
#include <vix/threadpool/TaskId.hpp>
#include <vix/threadpool/TaskStatus.hpp>

namespace vix::game
{
  /**
   * @brief Unique identifier type for jobs.
   *
   * JobId maps to the threadpool task identifier used internally.
   */
  using JobId = vix::threadpool::TaskId;

  /**
   * @brief Invalid job identifier.
   */
  inline constexpr JobId invalid_job_id = vix::threadpool::invalid_task_id;

  /**
   * @brief Check whether a job id is valid.
   *
   * @param id Job identifier.
   * @return true if the id is valid.
   */
  [[nodiscard]] inline constexpr bool is_valid_job_id(JobId id) noexcept
  {
    return vix::threadpool::is_valid_task_id(id);
  }

  /**
   * @brief Internal threadpool handle type used by vix/game jobs.
   */
  using NativeJobHandle = vix::threadpool::TaskHandle<void>;

  /**
   * @brief Public handle for a submitted background job.
   *
   * JobHandle is the vix/game wrapper around vix::threadpool::TaskHandle<void>.
   * It keeps the game API stable while allowing the implementation to use
   * the threadpool module internally.
   */
  class JobHandle
  {
  public:
    /**
     * @brief Construct an empty job handle.
     */
    JobHandle() = default;

    /**
     * @brief Construct from a threadpool task handle.
     *
     * @param handle Internal task handle.
     */
    explicit JobHandle(NativeJobHandle handle)
        : handle_(std::move(handle))
    {
    }

    /**
     * @brief Return the job id.
     */
    [[nodiscard]] JobId id() const noexcept
    {
      return handle_.id();
    }

    /**
     * @brief Return true if the handle references a valid job.
     */
    [[nodiscard]] bool valid() const noexcept
    {
      return handle_.valid();
    }

    /**
     * @brief Return the current job status.
     */
    [[nodiscard]] vix::threadpool::TaskStatus status() const
    {
      return handle_.status();
    }

    /**
     * @brief Return true if the job reached a terminal state.
     */
    [[nodiscard]] bool done() const
    {
      return vix::threadpool::is_terminal(status());
    }

    /**
     * @brief Return true if the job is queued or running.
     */
    [[nodiscard]] bool active() const
    {
      return vix::threadpool::is_active(status());
    }

    /**
     * @brief Request cooperative cancellation for the job.
     *
     * Actual cancellation depends on the job body observing its token.
     */
    void cancel() noexcept
    {
      handle_.cancel();
    }

    /**
     * @brief Return true if cancellation was requested.
     */
    [[nodiscard]] bool cancelled() const noexcept
    {
      return handle_.cancelled();
    }

    /**
     * @brief Return true if the job result is ready.
     */
    [[nodiscard]] bool ready() const
    {
      return handle_.ready();
    }

    /**
     * @brief Wait for the job to complete.
     */
    void wait() const
    {
      handle_.wait();
    }

    /**
     * @brief Wait and consume the job result.
     */
    void get()
    {
      handle_.get();
    }

    /**
     * @brief Return the wrapped threadpool handle.
     */
    [[nodiscard]] const NativeJobHandle &native() const noexcept
    {
      return handle_;
    }

    /**
     * @brief Return the wrapped threadpool handle.
     */
    [[nodiscard]] NativeJobHandle &native() noexcept
    {
      return handle_;
    }

  private:
    /**
     * @brief Internal threadpool task handle.
     */
    NativeJobHandle handle_{};
  };

} // namespace vix::game

#endif // VIX_GAME_JOB_HANDLE_HPP
